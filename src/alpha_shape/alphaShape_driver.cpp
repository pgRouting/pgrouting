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

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"

#include "cpp_common/bpoint.hpp"
#include "alphaShape/pgr_alphaShape.hpp"



void
do_alphaShape(
        Pgr_edge_xy_t *edgesArr,
        size_t edgesTotal,

        double alpha,

        Pgr_point_t **return_tuples,
        size_t *res_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(edgesArr);
        pgassert(edgesTotal > 2);

#if 0
        using Bpoint = pgrouting::Bpoint;
        using Pgr_alphaShape = pgrouting::alphashape::Pgr_alphaShape;
        std::vector<Point> points;
        std::vector<Bpoint> bpoints;

        for (std::size_t j = 0; j < pointsTotal; ++j) {
            Point p(pointsArr[j].x, pointsArr[j].y);
            points.push_back(p);
            bpoints.push_back(Bpoint(pointsArr[j].x, pointsArr[j].y));
        }

        std::vector<Delauny_t> delauny(delaunyArr, delaunyArr + delaunyTotal);
        for (const auto d : delauny) {
            log << d.tid << ")" << d.pid << "," << d.x << " " << d.y << "\n";
        }

        size_t i(0);
        for (const auto p : bpoints) {
            log << i++ << ")" << boost::geometry::wkt(p) << "\n";
            pgr_msg(log.str().c_str());
        }

        log << "points: ";
        i = 0;
        for (const auto p : points) {
            log << i++ << ")" << p << "\n";
        }

        Pgr_alphaShape alphaShape(delauny);

        log << "LOG STARTS **********\n";
        log << alphaShape.get_log();
        log << "\nLOG ENDS **********\n";
        log << alphaShape;
        log << "LOG ENDS **********\n";
#endif

        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*res_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*res_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*res_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
    }
}
