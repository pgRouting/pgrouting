/*PGR-GNU*****************************************************************
File: testXYedges.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Rohith Reddy
Mail: 

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

#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"

#include "c_common/edges_input.h"

#include "./test_c_edges_driver.h"

PGDLLEXPORT Datum test_c_edges(PG_FUNCTION_ARGS);


static
void
process(char *edges_sql,
        bool *result_bool) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_sql, &edges, &total_edges);

    if (total_edges == 0) {
        PGR_DBG("No edges found");
        (*result_bool) = true;
        pfree(edges);
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld tuples in query:", total_edges);
    size_t i;
    for (i = 0; i < total_edges; ++i) {
        PGR_DBG("id = %li \t source = %li \t target = %ld  cost = %lf reverse_cost = %lf",
               edges[i].id, edges[i].source, edges[i].target, edges[i].cost, edges[i].reverse_cost);
        /*PGR_DBG("    (x1,y1) = (%.32lf ,%.32lf) (x2,y2) = (%.32lf,.%.32lf)",
               edges[i].x1, edges[i].y1, edges[i].x2, edges[i].y2);*/
    }


    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    char *log_msg = NULL;
    (*result_bool) = do_pgr_test_c_edges(
            edges,
            total_edges,
            &log_msg,
            &err_msg);

    pfree(edges);
    PGR_DBG("Returned log message = %s\n", log_msg);
    if (log_msg) {
        elog(DEBUG1, "%s", log_msg);
        free(log_msg);
    }
    PGR_DBG("Returned error message = %s\n", err_msg);

    if (err_msg) {
        pgr_SPI_finish();
        elog(ERROR, "%s", err_msg);
        free(err_msg);
    }

    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(test_c_edges);
PGDLLEXPORT Datum
test_c_edges(PG_FUNCTION_ARGS) {
    bool  result_bool = false;
    process(
            text_to_cstring(PG_GETARG_TEXT_P(0)),
            &result_bool);

    PG_RETURN_BOOL(result_bool);
}

