/*PGR-GNU*****************************************************************
FILE: alphaShape_driver.cpp

Copyright (c) 2018 Vicky Vergara
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#include "drivers/alpha_shape/alphaShape_driver.h"

#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "cpp_common/pgr_assert.h"
#include "alphaShape/pgr_alphaShape.h"

#include "cpp_common/pgr_alloc.hpp"

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/bpoint.hpp"
#include "cpp_common/bline.hpp"
#if BOOST_Geometry_VERSION_OK
#include <boost/geometry/io/wkt/write.hpp>
#else
#include <boost/bgeometry/io/wkt/write.hpp>
#endif


namespace {
bool
data_eq(const Pgr_edge_xy_t &lhs, const Pgr_edge_xy_t &rhs, int64_t round) {
    return
        std::floor(lhs.x1 * static_cast<double>(round)) == std::floor(rhs.x1 * static_cast<double>(round))
        &&
        std::floor(lhs.y1 * static_cast<double>(round)) == std::floor(rhs.y1 * static_cast<double>(round));
}

}

void
do_alphaShape(
        Pgr_edge_xy_t *edgesArr,
        size_t edgesSize,

        double alpha,

        GeomText_t **return_tuples,
        size_t *return_count,

        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(edgesArr);
        pgassert(edgesSize > 2);
        pgassert(*return_count == 0);
        const int64_t round = 100000000000000;

        std::vector<Pgr_edge_xy_t> edges(edgesArr, edgesArr + edgesSize);
        /*
          id   | source | target | cost |     x1     |     y1     | x2 | y2
        -------+--------+--------+------+------------+------------+----+----
             1 |      1 |     -1 |    1 | 29.1296668 | 40.9434941 |  0 |  0
             1 |      2 |     -1 |    1 | 29.1267903 | 40.9343215 |  0 |  0
             1 |      3 |     -1 |    1 | 29.1296615 |  40.943359 |  0 |  0
             sort based on y1
             stable sort on x1
        */

        std::sort(edges.begin(), edges.end(),
                [&round](const Pgr_edge_xy_t &lhs, const Pgr_edge_xy_t &rhs) {
            return
                std::floor(lhs.y1 *  static_cast<double>(round)) < std::floor(rhs.y1 *  static_cast<double>(round));
        });
        std::stable_sort(edges.begin(), edges.end(),
                [&round](const Pgr_edge_xy_t &lhs, const Pgr_edge_xy_t &rhs) {
            return
                std::floor(lhs.x1 *  static_cast<double>(round)) < std::floor(rhs.x1 *  static_cast<double>(round));
        });
        log << "ending sort";



        int64_t source_id(0);
        auto prev = edges.front();
        for (auto &e : edges) {
            if (data_eq(e, prev, round)) {
                e.source = source_id;
            } else {
                e.source = ++source_id;
                prev = e;
            }
        }
        std::stable_sort(edges.begin(), edges.end(),
                [](const Pgr_edge_xy_t &lhs, const Pgr_edge_xy_t &rhs) {
            return
                lhs.id < rhs.id;
        });

        /*
         * Vertices of triangle a, b, c
         */
        Pgr_edge_xy_t *a(nullptr);
        Pgr_edge_xy_t *b(nullptr);
        size_t count(0);
        for (auto &c : edges) {
            if (count == 0) {
                a = &c;
                ++count;
                continue;
            }
            if (count == 1) {
                b = &c;
                pgassert(a->id == b->id);
                ++count;
                continue;
            }
            pgassert(a->id == b->id);
            pgassert(a->id == c.id);

            a->target = b->source;
            a->x2 = b->x1;
            a->y2 = b->y1;

            b->target = c.source;
            b->x2 = c.x1;
            b->y2 = c.y1;

            c.target = a->source;
            c.x2 = a->x1;
            c.y2 = a->y1;
            count = 0;
        }

        using Pgr_alphaShape = pgrouting::alphashape::Pgr_alphaShape;
        Pgr_alphaShape alphaShape(edges);

        auto results = alphaShape(alpha);
        log << alphaShape.get_log();

        /*
         * returning a sequence of text
         */
        if (results.empty()) {
            *return_count = 1;
            *return_tuples = pgr_alloc(*return_count, (*return_tuples));
            std::stringstream ss;
            ss << "MULTIPOLYGON EMPTY";
            (*return_tuples)[0].geom = pgr_msg(ss.str().c_str());
        } else {
            *return_count = results.size();
            *return_tuples = pgr_alloc(*return_count, (*return_tuples));
            size_t row = 0;
            for (const auto r : results) {
                std::stringstream ss;
                ss << bg::wkt(r);
                (*return_tuples)[row].geom = pgr_msg(ss.str().c_str());
                ++row;
            }
        }


        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
    }
}
