/*PGR-GNU*****************************************************************
File: spanningTree_process.cpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2026 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#include "process/spanningTree_process.h"

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include <string>
#include <sstream>

#include "c_types/mst_rt.h"
#include "cpp_common/report_messages.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

#include "drivers/spanningTree_driver.hpp"

void pgr_process_spanningTree(
        const char* edges_sql,
        ArrayType* roots,

        bool directed,
        int64_t max_depth,
        double distance,
        bool equicost,

        const char* fn_suffix,
        enum Which which,
        MST_rt **result_tuples, size_t *result_count) {
    pgassert(edges_sql);
    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);

    std::string suffix = fn_suffix? std::string(fn_suffix) : "";
    int val = 0;

    switch (which) {
        case PRIM:
        case KRUSKAL:
            if (suffix == "DD") {
                val = 1;
            } else if (suffix == "DFS") {
                val = 2;
            } else if (suffix == "BFS") {
                val = 3;
            }

            which = static_cast<Which>(static_cast<int>(which) + val);

            switch (val) {
                case 1:
                    if (distance < 0) {
                        pgr_throw_error("Negative value found on 'distance'", "Must be non negative");
                    }
                    break;
                case 2:
                case 3:
                    if (max_depth < 0) {
                        pgr_throw_error("Negative value found on 'max_depth'", "Must be non negative");
                    }
                    break;
                default:
                    break;
            }
            break;

        case BFS:
        case DFS:
            if (max_depth < 0) {
                pgr_throw_error("Negative value found on 'max_depth'", "Must be non negative");
            }
            break;

        case DIJKSTRADD:
            if (distance < 0) {
                pgr_throw_error("Negative value found on 'distance'", "Must be positive");
            }
            break;

        default:
            break;
    }

    pgr_SPI_connect();

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    clock_t start_t = clock();
    pgrouting::drivers::do_spanningTree(
            edges_sql? edges_sql : "",
            roots,

            directed,
            max_depth,
            distance,
            equicost,

            which,
            (*result_tuples), (*result_count),
            log, notice, err);

    auto name = std::string(" processing ") + pgrouting::get_name(which);
    time_msg(name.c_str(), start_t, clock());

    if (!err.str().empty() && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = nullptr;
        (*result_count) = 0;
    }

    pgrouting::report_messages(log, notice, err);
    pgr_SPI_finish();
}
