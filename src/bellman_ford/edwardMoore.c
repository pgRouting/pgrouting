/*PGR-GNU*****************************************************************
File: edwardMoore.c

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

#include "c_types/path_rt.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/arrays_input.h"
#include "c_common/pgdata_getters.h"

#include "drivers/bellman_ford/edwardMoore_driver.h"

PGDLLEXPORT Datum _pgr_edwardmoore(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_edwardmoore);

static void
process(
    char *edges_sql,
    char *combinations_sql,
    ArrayType *starts,
    ArrayType *ends,
    bool directed,

    Path_rt **result_tuples,
    size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Initializing arrays");

    size_t size_start_vidsArr = 0;
    int64_t *start_vidsArr = NULL;

    size_t size_end_vidsArr = 0;
    int64_t *end_vidsArr = NULL;

    size_t total_combinations = 0;
    II_t_rt *combinations = NULL;

    if (starts && ends) {
        start_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_vidsArr, starts);
        end_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_end_vidsArr, ends);
    } else if (combinations_sql) {
        pgr_get_combinations(combinations_sql, &combinations, &total_combinations);
        if (total_combinations == 0) {
            if (combinations)
                pfree(combinations);
            pgr_SPI_finish();
            return;
        }
    }

    (*result_tuples) = NULL;
    (*result_count) = 0;

    PGR_DBG("Load data");
    Edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_sql, &edges, &total_edges, true, false);
    PGR_DBG("Total %ld edges in query:", total_edges);

    if (total_edges == 0) {
        if (start_vidsArr)
            pfree(start_vidsArr);
        if (end_vidsArr)
            pfree(end_vidsArr);
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    do_pgr_edwardMoore(
        edges,
        total_edges,
        combinations,
        total_combinations,
        start_vidsArr, size_start_vidsArr,
        end_vidsArr, size_end_vidsArr,

        directed,

        result_tuples,
        result_count,

        &log_msg,
        &notice_msg,
        &err_msg);

    time_msg(" processing pgr_edwardMoore", start_t, clock());
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
    if (end_vidsArr)
        pfree(end_vidsArr);
    pgr_SPI_finish();
}

PGDLLEXPORT Datum _pgr_edwardmoore(PG_FUNCTION_ARGS) {
    FuncCallContext *funcctx;
    TupleDesc tuple_desc;

    /**************************************************************************/
    Path_rt *result_tuples = NULL;
    size_t result_count = 0;
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        PGR_DBG("Calling process");
        if (PG_NARGS() == 4) {
            /*
             * many to many
             */
            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                NULL,
                PG_GETARG_ARRAYTYPE_P(1),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_BOOL(3),
                &result_tuples,
                &result_count);

        } else if (PG_NARGS() == 3) {
            /*
             * combinations
             */
            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                NULL,
                NULL,
                PG_GETARG_BOOL(2),
                &result_tuples,
                &result_count);
        }


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
    result_tuples = (Path_rt *)funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple tuple;
        Datum result;
        Datum *values;
        bool *nulls;

        /**********************************************************************/
        /*
            OUT seq BIGINT,
            OUT path_seq BIGINT,
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
