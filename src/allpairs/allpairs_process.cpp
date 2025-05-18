/*PGR-GNU*****************************************************************
File: process_allpairs.cpp

Function's developer:
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky_vergara at erosion.dev

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

#include "process/allpairs_process.h"

#include <string>

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
}

#include "c_types/iid_t_rt.h"
#include "cpp_common/assert.hpp"
#include "drivers/allpairs_driver.hpp"

/**
 which = 0 -> johnson
 which = 1 -> floydWarshall

 This is c++ code, linked as C code, because process_allpairs is called from C code
 */
void pgr_process_allpairs(
        const char* edges_sql,
        bool directed,
        int which,
        IID_t_rt **result_tuples,
        size_t *result_count) {
    pgassert(!(*result_tuples));
    pgassert(*result_count == 0);
    pgr_SPI_connect();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    clock_t start_t = clock();
    do_allpairs(
            edges_sql, directed,
            which,
            result_tuples, result_count,
            &log_msg, &err_msg);

    if (which == 0) {
        time_msg(std::string(" processing pgr_johnson").c_str(), start_t, clock());
    } else {
        time_msg(std::string(" processing pgr_floydWarshall").c_str(), start_t, clock());
    }


    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}
