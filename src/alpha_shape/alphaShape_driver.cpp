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

#include "alphaShape/pgr_alphaShape.hpp"
#include "cpp_common/pgr_base_graph.hpp"


void
do_alphaShape(
        Pgr_edge_xy_t *edgesArr,
        size_t edgesSize,

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
        pgassert(edgesSize > 2);

        std::vector<Pgr_edge_xy_t> edges(edgesArr, edgesArr + edgesSize);

        using Pgr_alphaShape = pgrouting::alphashape::Pgr_alphaShape;

        Pgr_alphaShape alphaShape(edges);

        log << "LOG STARTS **********\n";
        log << alphaShape.get_log();
        log << "\nLOG ENDS **********\n";
        log << alphaShape;
        log << "LOG ENDS **********\n";

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
