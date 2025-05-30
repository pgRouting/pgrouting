/*PGR-GNU*****************************************************************
File: shortestPath_process.cpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copying this file (or a derivative) within pgRouting code add the following:

Generated with Template by:
Copyright (c) 2025 pgRouting developers
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

#include "process/shortestPath_process.h"

#include <string>

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include "c_types/path_rt.h"
#include "cpp_common/assert.hpp"
#include "drivers/shortestPath_driver.hpp"

namespace {

std::string
get_name(int32_t which, bool is_only_cost, bool is_near, bool is_withPoints, bool is_matrix) {
    std::string base;
    switch  (which) {
        case 0 : { base = "pgr_dijkstra"; break;}
        case 1 : { base = "pgr_withPoints"; break;}
        default : base = "unknown";
    }
  base = which > 1 && is_withPoints? base + "_withPoints" : base;
    return "Processing " + base + (is_near? "Near" : "") + (is_only_cost? "Cost" : "") + (is_matrix? "Matrix" : "");
}

}  // namespace

/**
 which function is determined by the parameters

 This is c++ code, linked as C code, because pgr_process_shortestPath is called from C code
 */
void pgr_process_shortestPath(
        const char *edges_sql, const char *points_sql, const char *combinations_sql,
        ArrayType *starts, ArrayType *ends,
        bool directed, bool only_cost, bool normal,
        int64_t n_goals, bool global,
        char *driving_side, bool details,
        int32_t which,
        Path_rt **result_tuples, size_t *result_count) {
    pgassert(edges_sql);
    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);
    pgr_SPI_connect();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    bool is_matrix = false;

    clock_t start_t = clock();
    do_shortestPath(
            edges_sql? edges_sql : "",
            points_sql? points_sql : "",
            combinations_sql? combinations_sql : "",
            starts, ends,
            directed, only_cost, normal,
            n_goals, global,
            driving_side[0], details,
            which,
            result_tuples, result_count,
            &is_matrix, &log_msg, &notice_msg, &err_msg);

    auto fn_name = get_name(which,  only_cost, n_goals > 0, points_sql != nullptr, is_matrix);
    time_msg(fn_name.c_str(), start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}
