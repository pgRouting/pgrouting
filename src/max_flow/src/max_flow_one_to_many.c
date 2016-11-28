/*PGR-GNU*****************************************************************
File: max_flow_one_to_many.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "./../../common/src/postgres_connection.h"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "funcapi.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "utils/array.h"

#include "./../../common/src/debug_macro.h"
#include "./../../common/src/e_report.h"
#include "./../../common/src/time_msg.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"
#include "./max_flow_many_to_many_driver.h"

PGDLLEXPORT Datum
max_flow_one_to_many(PG_FUNCTION_ARGS);

/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */
static
void
process(
        char *edges_sql,
        int64_t source_vertex,
        ArrayType *ends,
        char *algorithm,
        bool only_flow,
        pgr_flow_t **result_tuples,
        size_t *result_count) {

    if (!(strcmp(algorithm, "push_relabel") == 0
                || strcmp(algorithm, "edmonds_karp") == 0
                || strcmp(algorithm, "boykov_kolmogorov") == 0)) {
        elog(ERROR, "Unknown algorithm");
    }

    pgr_SPI_connect();

    size_t size_sink_verticesArr = 0;
    int64_t* sink_vertices =
        pgr_get_bigIntArray(&size_sink_verticesArr, ends);

    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;

    size_t total_tuples = 0;

    /* NOTE:
     * For flow, cost and reverse_cost are really capacity and reverse_capacity
     */
    pgr_get_flow_edges(edges_sql, &edges, &total_tuples);

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
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    do_pgr_max_flow_many_to_many(
            edges, total_tuples,
            &source_vertex, 1,
            sink_vertices, size_sink_verticesArr,
            algorithm,
            only_flow,

            result_tuples, result_count,

            &log_msg,
            &notice_msg,
            &err_msg);

    if (strcmp(algorithm, "push_relabel") == 0) {
        time_msg("processing pgr_maxFlowPushRelabel(one to many)", start_t, clock());
    } else if (strcmp(algorithm, "edmonds_karp") == 0) {
        time_msg("processing pgr_maxFlowEdmondsKarp(one to many)", start_t, clock());
    } else {
        time_msg("processing pgr_maxFlowBoykovKolmogorov(one to many)", start_t, clock());
    }


    if (edges) pfree(edges);
    if (sink_vertices) pfree(sink_vertices);

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);

    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

PG_FUNCTION_INFO_V1(max_flow_one_to_many);
PGDLLEXPORT Datum
max_flow_one_to_many(PG_FUNCTION_ARGS) {
    FuncCallContext *funcctx;
    TupleDesc tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    pgr_flow_t *result_tuples = 0;
    size_t result_count = 0;
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_INT64(1),
                PG_GETARG_ARRAYTYPE_P(2),
                text_to_cstring(PG_GETARG_TEXT_P(3)),
                PG_GETARG_BOOL(4),
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
    result_tuples = (pgr_flow_t *) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple tuple;
        Datum result;
        Datum *values;
        bool *nulls;

        /**********************************************************************/

        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(bool));

        size_t i;
        for (i = 0; i < 6; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].source);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].target);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].flow);
        values[5] = Int64GetDatum(result_tuples[funcctx->call_cntr].residual_capacity);

        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

