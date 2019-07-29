/*PGR-GNU*****************************************************************
File: johnson.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#include <stdbool.h>
#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/edges_input.h"

#include "drivers/allpairs/johnson_driver.h"

PGDLLEXPORT Datum _pgr_johnson(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_johnson);

/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */
static
void process(
        char* edges_sql,
        bool directed,
        Matrix_cell_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
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
    do_pgr_johnson(
            edges,
            total_tuples,
            directed,
            result_tuples,
            result_count,
            &log_msg,
            &err_msg);
    time_msg(" processing Johnson", start_t, clock());

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
/*                                                                            */
/******************************************************************************/

PGDLLEXPORT Datum
_pgr_johnson(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    Matrix_cell_t *result_tuples = NULL;
    size_t result_count = 0;
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /*********************************************************************/
        /*                          MODIFY AS NEEDED                         */
        // CREATE OR REPLACE FUNCTION pgr_johnson(
        // edges_sql TEXT,
        // directed BOOLEAN,

        PGR_DBG("Calling process");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_BOOL(1),
                &result_tuples,
                &result_count);

        /*                                                                   */
        /*********************************************************************/

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc)
                != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (Matrix_cell_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool         *nulls;

        /*********************************************************************/
        /*                          MODIFY AS NEEDED                         */
        // OUT seq BIGINT,
        // OUT from_vid BIGINT,
        // OUT to_vid BIGINT,
        // OUT cost float)

        values = palloc(3 * sizeof(Datum));
        nulls = palloc(3 * sizeof(bool));

        // postgres starts counting from 1
        values[0] = Int64GetDatum(result_tuples[funcctx->call_cntr].from_vid);
        nulls[0] = false;
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].to_vid);
        nulls[1] = false;
        values[2] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        nulls[2] = false;

        /*********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

