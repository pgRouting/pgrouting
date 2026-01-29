/*PGR-GNU*****************************************************************
File: metrics_process.cpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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

#include "process/metrics_process.h"

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/time_msg.h"
}

#include <string>
#include <sstream>

#include "c_types/iid_t_rt.h"

#include "cpp_common/report_messages.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

#include "drivers/metrics_driver.hpp"

/**
 which = 0 -> bandwidth
 */
uint64_t pgr_process_metrics(
        const char* edges_sql,
        int which) {
    pgassert(edges_sql);

    pgr_SPI_connect();

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    clock_t start_t = clock();
    auto result = do_metrics(
            edges_sql? edges_sql : "",
            which,
            log, err);

    if (which == 0) {
        time_msg(std::string(" processing pgr_bandwidth").c_str(), start_t, clock());
    }

    pgrouting::report_messages(log, notice, err);
    pgr_SPI_finish();
    return result;
}
