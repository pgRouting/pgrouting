/*PGR-GNU*****************************************************************
File: breadthFirstSearch.c

Generated with Template by:
Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Gudesa Venkata Sai Akhil
Mail: gvs.akhil1997@gmail.com


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

#include "drivers/breadthFirstSearch/breadthFirstSearch_driver.h"

PGDLLEXPORT Datum _pgr_breadthfirstsearch(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_breadthfirstsearch);

static void
process(
    char *edges_sql,
    ArrayType *starts,
    int64_t max_depth,
    bool directed,

    pgr_mst_rt **result_tuples,
    size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Initializing arrays");

    size_t size_start_vidsArr = 0;
    int64_t *start_vidsArr = (int64_t *)
        pgr_get_bigIntArray(&size_start_vidsArr, starts);
    PGR_DBG("start_vidsArr size %ld ", size_start_vidsArr);


    (*result_tuples) = NULL;
    (*result_count) = 0;

    PGR_DBG("Load data");
    Edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_sql, &edges, &total_edges);
    PGR_DBG("Total %ld edges in query:", total_edges);

    if (total_edges == 0) {
        if (start_vidsArr)
            pfree(start_vidsArr);
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    do_pgr_breadthFirstSearch(
        edges,
        total_edges,
        start_vidsArr, size_start_vidsArr,
        max_depth,
        directed,

        result_tuples,
        result_count,

        &log_msg,
        &notice_msg,
        &err_msg);

    time_msg(" processing pgr_breadthFirstSearch", start_t, clock());
    PGR_DBG("Returning %ld tuples", *result_count);

    if (err_msg) {
        if (*result_tuples)
            pfree(*result_tuples);
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (edges)
        pfree(edges);
    if (log_msg)
        pfree(log_msg);
    if (notice_msg)
        pfree(notice_msg);
    if (err_msg)
        pfree(err_msg);

    if (start_vidsArr)
        pfree(start_vidsArr);
    pgr_SPI_finish();
}

PGDLLEXPORT Datum _pgr_breadthfirstsearch(PG_FUNCTION_ARGS) {
    FuncCallContext *funcctx;
    TupleDesc tuple_desc;

    /**************************************************************************/
    pgr_mst_rt *result_tuples = NULL;
    size_t result_count = 0;
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        /**********************************************************************/
        /*
        pgr_breadthFirstSearch(
            edge_sql TEXT,
            start_vids ANYARRAY,
            max_depth BIGINT DEFAULT 9223372036854775807,
            directed BOOLEAN DEFAULT true)
        */
        /**********************************************************************/

        PGR_DBG("Calling process");
        process(
            text_to_cstring(PG_GETARG_TEXT_P(0)),
            PG_GETARG_ARRAYTYPE_P(1),
            PG_GETARG_INT64(2),
            PG_GETARG_BOOL(3),
            &result_tuples,
            &result_count);

        /**********************************************************************/

        funcctx->max_calls = result_count;

        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE) {
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
    result_tuples = (pgr_mst_rt *)funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple tuple;
        Datum result;
        Datum *values;
        bool *nulls;

        /**********************************************************************/
        /*
            OUT seq BIGINT,
            OUT depth BIGINT,
            OUT start_vid BIGINT,
            OUT node BIGINT,
            OUT edge BIGINT,
            OUT cost FLOAT,
            OUT agg_cost FLOAT
        */
        /**********************************************************************/
        size_t numb = 7;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].depth);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].from_v);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[5] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[6] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

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
