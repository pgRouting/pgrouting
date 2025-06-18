/*PGR-GNU*****************************************************************
File: metrics_process.cpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"
#if 0
#include "cpp_common/base_graph.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/pgdata_fetchers.hpp"
#endif
#include "cpp_common/get_data.hpp"
#include "cpp_common/to_postgres.hpp"

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include "process/metrics_process.h"
#include "drivers/metrics_driver.hpp"
#include "c_types/bandwidth_rt.h"

struct Metrics_rt {
    int64_t node;
    double metric_value;
};

void
pgr_process_bandwidth(
    const char* edges_sql,
    GraphBandwidth_rt** result_tuples,
    size_t* result_count,
    char** log_msg,
    char** notice_msg) {

    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);


    pgr_SPI_connect();

    *log_msg = *notice_msg = nullptr;
    char* err_msg = nullptr;

    clock_t start_t = clock();

    std::vector<GraphBandwidth_rt> results =
        pgrouting::bandwidth::compute_bandwidth(std::string(edges_sql));

    if (!results.empty()) {
        *result_count = results.size();
        (*result_tuples) = pgr_alloc(*result_count, GraphBandwidth_rt);

        for (size_t i = 0; i < *result_count; ++i) {
            (*result_tuples)[i] = results[i];
        }

        time_msg(" processing pgr_bandwidth", start_t, clock());
    } else {
        *result_count = 0;
        *result_tuples = nullptr;
    }

    pgr_global_report(log_msg, notice_msg, &err_msg);
    pgr_SPI_finish();
}
