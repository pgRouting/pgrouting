/*PGR-GNU*****************************************************************
File: vrp_process.cpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
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

#include "process/vrp_process.h"

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include <string>
#include <sstream>

#include "c_types/iid_t_rt.h"
#include "c_types/schedule_rt.h"

#include "drivers/pickDeliver/pickDeliver_driver.h"

#include "cpp_common/report_messages.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

#include "drivers/vrp_driver.hpp"

void
pgr_process_vrp(
        const char *pd_orders_sql,
        const char *vehicles_sql,
        const char *matrix_sql,
        double factor,
        int max_cycles,
        int initial_solution_id,

        enum Which which,
        Schedule_rt **result_tuples,
        size_t *result_count) {
    using pgrouting::to_pg_msg;

    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);
    pgr_SPI_connect();

#if 0
    if (factor <= 0) {
        ereport(ERROR,
                (errcode(ERRCODE_INTERNAL_ERROR),
                 errmsg("Illegal value in parameter: factor"),
                 errhint("Value found: %f <= 0", factor)));
        (*result_count) = 0;
        (*result_tuples) = NULL;
        return;
    }

    if (max_cycles < 0) {
        elog(ERROR, "Illegal value in parameter: max_cycles");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        return;
    }

    if (initial_solution_id < 0 || initial_solution_id > 7) {
        elog(ERROR, "Illegal value in parameter: initial");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        return;
    }
#endif

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    clock_t start_t = clock();
    pgrouting::drivers::do_vrp(
            pd_orders_sql? pd_orders_sql : "",
            vehicles_sql? vehicles_sql : "",
            matrix_sql? matrix_sql : "",

            factor,
            max_cycles,
            initial_solution_id,

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
