/*PGR-GNU*****************************************************************
File: bellman_ford.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Sourabh Garg
Mail: sourabh.garg.mat@gmail.com


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
#include "utils/array.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"

#include "drivers/bellman_ford/bellman_ford_neg_driver.h"  // the link to the C++ code of the function

PGDLLEXPORT Datum bellman_ford_neg(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(bellman_ford_neg);

static
void
process(
        char* edges_sql,
        char* neg_edges_sql,
        ArrayType *starts,
        ArrayType *ends,
        bool directed,
        bool only_cost,

        General_path_element_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Initializing arrays");

    size_t size_start_vidsArr = 0;
    int64_t* start_vidsArr = (int64_t*)
        pgr_get_bigIntArray(&size_start_vidsArr, starts);
    PGR_DBG("start_vidsArr size %ld ", size_start_vidsArr);

    size_t size_end_vidsArr = 0;
    int64_t* end_vidsArr = (int64_t*)
        pgr_get_bigIntArray(&size_end_vidsArr, ends);
    PGR_DBG("end_vidsArr size %ld ", size_end_vidsArr);


    (*result_tuples) = NULL;
    (*result_count) = 0;

    PGR_DBG("Load data");
    pgr_edge_t *positive_edges = NULL;
    size_t total_positive_edges = 0;

    pgr_get_edges(edges_sql, &positive_edges, &total_positive_edges);
    PGR_DBG(
            "Total positive weighted edges in query: %ld",
            total_positive_edges);

    pgr_edge_t *negative_edges = NULL;
    size_t total_negative_edges = 0;

    pgr_get_edges(neg_edges_sql, &negative_edges, &total_negative_edges);
    PGR_DBG(
            "Total negative weighted edges in query: %ld",
            total_negative_edges);

    size_t total_edges = total_positive_edges + total_negative_edges;

    if (total_edges == 0) {
        if (end_vidsArr) pfree(end_vidsArr);
        if (start_vidsArr) pfree(start_vidsArr);
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    do_pgr_bellman_ford_neg(
            positive_edges,
            total_positive_edges,
            negative_edges,
            total_negative_edges,
            start_vidsArr,
            size_start_vidsArr,
            end_vidsArr,
            size_end_vidsArr,
            directed,
            only_cost,

            result_tuples,
            result_count,

            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg(" processing pgr_bellman_ford", start_t, clock());
    PGR_DBG("Returning %ld tuples", *result_count);

    if (err_msg) {
        if (*result_tuples) pfree(*result_tuples);
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (positive_edges) pfree(positive_edges);
    if (negative_edges) pfree(negative_edges);
    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);

    if (end_vidsArr) pfree(end_vidsArr);
    if (start_vidsArr) pfree(start_vidsArr);
    pgr_SPI_finish();
}


PGDLLEXPORT Datum bellman_ford_neg(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    /**************************************************************************/
    General_path_element_t  *result_tuples = NULL;
    size_t result_count = 0;
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*
        pgr_bellman_ford(
            edge_sql TEXT,
            start_vids ANYARRAY,
            end_vids ANYARRAY,
            directed BOOLEAN DEFAULT true)
        */
        /**********************************************************************/


        PGR_DBG("Calling process");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_BOOL(4),
                PG_GETARG_BOOL(5),
                &result_tuples,
                &result_count);

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
    result_tuples = (General_path_element_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        /**********************************************************************/
        /*
            OUT seq INTEGER,
            OUT path_seq INTEGER,
            OUT start_vid BIGINT,
            OUT end_vid BIGINT,
            OUT node BIGINT,
            OUT edge BIGINT,
            OUT cost FLOAT,
            OUT agg_cost FLOAT
        */
        /**********************************************************************/
        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));


        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[funcctx->call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].end_id);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        /**********************************************************************/

        PGR_DBG("Clean up code");

        /**********************************************************************/

        SRF_RETURN_DONE(funcctx);
    }
}
