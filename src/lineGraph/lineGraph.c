/*PGR-GNU*****************************************************************
File: lineGraph.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

/** @file lineGraph.c
 * @brief Connecting code with postgres.
 *
 * This file is fully documented for understanding
 *  how the postgres connectinon works
 *
 * TODO Remove unnecessary comments before submiting the function.
 * some comments are in form of PGR_DBG message
 */

/**
 *  postgres_connection.h
 *
 *  - should always be first in the C code
 */
#include <stdbool.h>
#include "c_common/postgres_connection.h"


/* for macro PGR_DBG */
#include "c_common/debug_macro.h"
/* for pgr_global_report */
#include "c_common/e_report.h"
/* for time_msg & clock */
#include "c_common/time_msg.h"
/* for functions to get edges information */
#include "c_common/edges_input.h"

#include "drivers/lineGraph/lineGraph_driver.h"  // the link to the C++ code of the function

PGDLLEXPORT Datum _pgr_linegraph(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_linegraph);


/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */
static
void
process(
        char* edges_sql,
        bool directed,
        Line_graph_rt **result_tuples,
        size_t *result_count) {
    /*
     *  https://www.postgresql.org/docs/current/static/spi-spi-connect.html
     */
    PGR_DBG("\nSQL QUERY: %s\n", edges_sql);
    if (directed) {
        PGR_DBG("\nDirectedGraph\n");
    } else {
      PGR_DBG("\nUndirectedGraph\n");
    }
    pgr_SPI_connect();

    (*result_tuples) = NULL;
    (*result_count) = 0;

    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_sql, &edges, &total_edges);
    PGR_DBG("Total %ld edges in query:", total_edges);

    if (total_edges == 0) {
        PGR_DBG("No edges found");
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    do_pgr_lineGraph(
            edges,
            total_edges,
            directed,
            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg(" processing pgr_lineGraph", start_t, clock());
    PGR_DBG("Returning %ld tuples", *result_count);

    if (err_msg) {
        if (*result_tuples) pfree(*result_tuples);
    }
    pgr_global_report(log_msg, notice_msg, err_msg);

    if (edges) pfree(edges);
    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);

    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

PGDLLEXPORT Datum _pgr_linegraph(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    Line_graph_rt  *result_tuples = NULL;
    size_t result_count = 0;
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /*
           TEXT,
    directed BOOLEAN DEFAULT true,
         **********************************************************************/


        PGR_DBG("Calling process");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_BOOL(1),
                &result_tuples,
                &result_count);


        /*                                                                    */
        /**********************************************************************/

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc)
                != TYPEFUNC_COMPOSITE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        }

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (Line_graph_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        values = palloc(5 * sizeof(Datum));
        nulls = palloc(5 * sizeof(bool));


        size_t i;
        for (i = 0; i < 5; ++i) {
            nulls[i] = false;
        }

        // postgres starts counting from 1
        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].source);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].target);
        values[3] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[4] = Float8GetDatum(result_tuples[
            funcctx->call_cntr].reverse_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */

        PGR_DBG("Clean up code");

        /**********************************************************************/

        SRF_RETURN_DONE(funcctx);
    }
}
