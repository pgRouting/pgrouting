/*PGR-GNU*****************************************************************
File: xyz_tsp.c

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

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

// #define DEBUG

#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./distances_input.h"
#include "./tsp_driver.h"

// #include "./one_to_one_dijkstra_driver.h"


PG_FUNCTION_INFO_V1(xyd_tsp);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
xyd_tsp(PG_FUNCTION_ARGS);

/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */
static
void
process(
        char* distances_sql,
        int64_t start_vid,
        int64_t end_vid,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    Matrix_cell_t *distances = NULL;
    size_t total_distances = 0;
    pgr_get_distances(distances_sql, &distances, &total_distances);

    if (total_distances == 0) {
        PGR_DBG("No distances found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld tuples in query:", total_distances);

    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    do_pgr_tsp(
            distances,
            total_distances,
            start_vid,
            end_vid,
            result_tuples,
            result_count,
            &err_msg);
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("Returned message = %s\n", err_msg);

    free(err_msg);
    pfree(distances);
    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
xyd_tsp(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t              call_cntr;
    uint32_t               max_calls;
    TupleDesc            tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    General_path_element_t  *result_tuples = 0;
    size_t result_count = 0;
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        // CREATE OR REPLACE FUNCTION pgr_dijkstra(
        // sql text,
        // start_vid BIGINT,
        // end_vid BIGINT,
        // directed BOOLEAN default true,

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                PG_GETARG_INT64(1),
                PG_GETARG_INT64(2),
                &result_tuples,
                &result_count);

        /*                                                                    */
        /**********************************************************************/

        funcctx->max_calls = (uint32_t) result_count;
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
    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (General_path_element_t*) funcctx->user_fctx;

    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        char*        nulls;

        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        // OUT seq INTEGER,
        // OUT node BIGINT,
        // OUT cost FLOAT,
        // OUT agg_cost FLOAT

        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(char));


        size_t i;
        for (i = 0; i < 4; ++i) {
            nulls[i] = ' ';
        }

        // postgres starts counting from 1
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[call_cntr].node);
        values[2] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[3] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
        /**********************************************************************/

        tuple = heap_formtuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

