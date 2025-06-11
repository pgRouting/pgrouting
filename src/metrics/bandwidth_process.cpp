/*PGR-GNU*****************************************************************
File: bandwidth_process.cpp
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510@gmail.com

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

#include "process/bandwidth_process.h"

#include <string>

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include "cpp_common/edge_t.hpp"
#include "c_types/graph_bandwidth_rt.h"
#include "cpp_common/assert.hpp"
#include "drivers/metrics/bandwidth_driver.h"

void pgr_process_bandwidth(
        const Edge_t* edges,
        size_t total_edges,
        GraphBandwidth_rt **result_tuples,
        size_t *result_count) {
    pgassert(edges);
    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);

    pgr_SPI_connect();
    char* log_msg = nullptr;
    char* notice_msg = nullptr;
    char* err_msg = nullptr;

    clock_t start_t = clock();

    do_bandwidth(
        edges,
        total_edges,
        result_tuples,
        result_count,
        &log_msg,
        &err_msg
    );

    time_msg("processing pgr_bandwidth", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = nullptr;
        (*result_count) = 0;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}
