/*PGR-GNU*****************************************************************
File: alphaShape_driver.cpp

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

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

/**
 * this file is not compiled with boost 1.53 or less
 */
#include "drivers/alpha_shape/alphaShape_driver.h"

#include <cmath>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include <boost/geometry/io/wkt/write.hpp>

#include "c_types/geom_text_rt.h"
#include "cpp_common/assert.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "alphaShape/alphaShape.hpp"

#include "cpp_common/alloc.hpp"

#include "cpp_common/base_graph.hpp"
#include "cpp_common/bpoint.hpp"
#include "cpp_common/bline.hpp"


namespace {
bool
data_eq(const Edge_xy_t &lhs, const Edge_xy_t &rhs, int64_t round) {
    return
        std::floor(lhs.x1 * static_cast<double>(round)) == std::floor(rhs.x1 * static_cast<double>(round))
        &&
        std::floor(lhs.y1 * static_cast<double>(round)) == std::floor(rhs.y1 * static_cast<double>(round));
}

}

void
pgr_do_alphaShape(
        char* edges_sql,
        double alpha,

        GeomText_t **return_tuples,
        size_t *return_count,

        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char* hint = nullptr;

    try {
        pgassert(*return_count == 0);
        const int64_t round = 100000000000000;

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges_xy(std::string(edges_sql), true);

        if (edges.size() < 3) {
            throw std::string("Less than 3 vertices. pgr_alphaShape needs at least 3 vertices.");
        }
        hint = nullptr;

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
                [&](const Edge_xy_t &lhs, const Edge_xy_t &rhs) {
            return
                std::floor(lhs.y1 *  static_cast<double>(round)) < std::floor(rhs.y1 *  static_cast<double>(round));
        });
        std::stable_sort(edges.begin(), edges.end(),
                [&](const Edge_xy_t &lhs, const Edge_xy_t &rhs) {
            return
                std::floor(lhs.x1 *  static_cast<double>(round)) < std::floor(rhs.x1 *  static_cast<double>(round));
        });


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
                [](const Edge_xy_t &lhs, const Edge_xy_t &rhs) {
            return
                lhs.id < rhs.id;
        });

        /*
         * Vertices of triangle a, b, c
         */
        Edge_xy_t *a(nullptr);
        Edge_xy_t *b(nullptr);
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
            (*return_tuples)[0].geom = to_pg_msg(ss.str());
        } else {
            *return_count = results.size();
            *return_tuples = pgr_alloc(*return_count, (*return_tuples));
            size_t row = 0;
            for (const auto &r : results) {
                std::stringstream ss;
                ss << bg::wkt(r);
                (*return_tuples)[row].geom = to_pg_msg(ss.str());
                ++row;
            }
        }


        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
