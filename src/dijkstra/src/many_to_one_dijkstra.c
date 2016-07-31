/*PGR-GNU*****************************************************************

File: many_to_one_dijkstra.c

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

/*
 * Uncomment when needed
 */

// #define DEBUG

#include "fmgr.h"

#include "./../../common/src/debug_macro.h"
#include "./../../common/src/time_msg.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"
#include "./many_to_one_dijkstra_driver.h"

PGDLLEXPORT Datum many_to_one_dijkstra(PG_FUNCTION_ARGS);
/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */
static
void
process(
        char* edges_sql,
        int64_t *start_vidsArr,
        size_t size_start_vidsArr,
        int64_t end_vid,
        bool directed,
        bool only_cost,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
    size_t total_tuples = 0;
    pgr_get_edges(edges_sql, &edges, &total_tuples);

    if (total_tuples == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Total %ld tuples in query:", total_tuples);

    PGR_DBG("Starting processing");
    clock_t start_t = clock();

    char *err_msg = NULL;
    do_pgr_many_to_one_dijkstra(
            edges,
            total_tuples,
            start_vidsArr,
            size_start_vidsArr,
            end_vid,
            directed,
            only_cost,
            result_tuples,
            result_count,
            &err_msg);
    time_msg(" processing Dijkstra many to one", start_t, clock());
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("Returned message = %s\n", err_msg);

    free(err_msg);
    pfree(edges);
    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

PG_FUNCTION_INFO_V1(many_to_one_dijkstra);
PGDLLEXPORT Datum
many_to_one_dijkstra(PG_FUNCTION_ARGS) {
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


        /*********************************************************************/
        /*                          MODIFY AS NEEDED                         */
        // CREATE OR REPLACE FUNCTION pgr_dijkstra(
        // sql text,
        // start_vids anyarray,
        // end_vid BIGINT,
        // directed BOOLEAN default true,

        PGR_DBG("Initializing arrays");
        int64_t* start_vidsArr;
        size_t size_start_vidsArr;
        start_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_vidsArr, PG_GETARG_ARRAYTYPE_P(1));
        PGR_DBG("start_vidsArr size %ld ", size_start_vidsArr);

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                start_vidsArr, size_start_vidsArr,
                PG_GETARG_INT64(2),
                PG_GETARG_BOOL(3),
                PG_GETARG_BOOL(4),
                &result_tuples,
                &result_count);

        PGR_DBG("Cleaning arrays");
        free(start_vidsArr);
        /*                                                                   */
        /*********************************************************************/

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
        bool*        nulls;

        /*********************************************************************/
        /*                          MODIFY AS NEEDED                         */
        // OUT seq INTEGER,
        // OUT path_seq INTEGER,
        // OUT start_vid BIGINT,
        // OUT end_vid BIGINT,
        // OUT node BIGINT,
        // OUT edge BIGINT,
        // OUT cost FLOAT,
        // OUT agg_cost FLOAT)

        values = palloc(7 * sizeof(Datum));
        nulls = palloc(7 * sizeof(bool));

        size_t i;
        for (i = 0; i < 7; ++i) {
            nulls[i] = false;
        }

        // postgres starts counting from 1
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[call_cntr].node);
        values[4] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[5] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[6] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
        /*********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

