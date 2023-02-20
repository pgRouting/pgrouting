/*PGR-GNU*****************************************************************
File: process_allpairs.cpp

Function's developer:
Copyright (c) 2023 Celia Virginia Vergara Castillo
Mail: vicky_vergara at hotmail.com

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

#include "allpairs/process_allpairs.h"

#include <string>

extern "C" {
#include "c_common/postgres_connection.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/edges_input.h"
}

#include "c_types/iid_t_rt.h"
#include "c_common/debug_macro.h"

#include "drivers/allpairs/johnson_driver.h"
#include "drivers/allpairs/floydWarshall_driver.h"

/**
 which = 0 -> johnson
 which = 1 -> floydWarshall
 */
void process_allpairs(
        char* edges_sql,
        bool directed,
        int which,
        IID_t_rt **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    Edge_t *edges = NULL;
    size_t total_tuples = 0;
    pgr_get_edges_no_id(edges_sql, &edges, &total_tuples);

    if (total_tuples == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld tuples in query:", total_tuples);

    PGR_DBG("Starting processing");
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    clock_t start_t = clock();
    if (which == 0) {
        do_johnson(edges, total_tuples, directed, result_tuples, result_count, &log_msg, &err_msg);
        time_msg(std::string(" processing pgr_johnson").c_str(), start_t, clock());
    } else {
        do_floydWarshall(edges, total_tuples, directed, result_tuples, result_count, &log_msg, &err_msg);
        time_msg(std::string(" processing pgr_floydWarshall").c_str(), start_t, clock());
    }


    if (err_msg && (*result_tuples)) {
        free(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);


    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);

    pfree(edges);
    pgr_SPI_finish();
}
