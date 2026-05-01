/*PGR-GNU*****************************************************************
File: shortestPath_process.cpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copying this file (or a derivative) within pgRouting code add the following:

Generated with Template by:
Copyright (c) 2025-2026 pgRouting developers
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

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include <string>
#include <sstream>

#include "c_types/path_rt.h"
#include "cpp_common/report_messages.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

#include "drivers/shortestPath_driver.hpp"


void pgr_process_shortestPath(
        const char *edges_sql,
        const char *points_sql,
        const char *combinations_sql,

        ArrayType *starts, ArrayType *ends,

        bool directed,
        bool only_cost,
        bool normal,

        int64_t n_goals,
        bool global,
        char driving_side,
        bool details,
        enum Which which,
        Path_rt **result_tuples, size_t *result_count) {
    pgassert(edges_sql);
    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);
    pgr_SPI_connect();

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    bool is_matrix = false;

    clock_t start_t = clock();
    pgrouting::drivers::do_shortestPath(
            edges_sql? edges_sql : "",
            points_sql? points_sql : "",
            combinations_sql? combinations_sql : "",
            starts, ends,
            directed,
            only_cost, normal,
            n_goals, global,
            driving_side, details,
            which,
            is_matrix,
            (*result_tuples), (*result_count),
            log, notice, err);

    auto name = std::string(" processing ") + pgrouting::get_name(which, only_cost, n_goals > 0, is_matrix);
    time_msg(name.c_str(), start_t, clock());

    if (!err.str().empty() && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = nullptr;
        (*result_count) = 0;
    }

    pgrouting::report_messages(log, notice, err);
    pgr_SPI_finish();
}
