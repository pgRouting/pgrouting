/*PGR-GNU*****************************************************************
File: max_flow_many_to_many.c

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

#include "c_common/postgres_connection.h"
#include "utils/array.h"


#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"
#include "drivers/max_flow/max_flow_driver.h"

PGDLLEXPORT Datum
max_flow_many_to_many(PG_FUNCTION_ARGS);

static
void
process(
        char *edges_sql,
        ArrayType *starts,
        ArrayType *ends,
        int algorithm,
        bool only_flow,
        pgr_flow_t **result_tuples,
        size_t *result_count) {
    if (algorithm < 1 || algorithm > 3) {
        elog(ERROR, "Unknown algorithm");
    }

    pgr_SPI_connect();

    size_t size_source_verticesArr = 0;
    int64_t* source_vertices =
        pgr_get_bigIntArray(&size_source_verticesArr, starts);

    size_t size_sink_verticesArr = 0;
    int64_t* sink_vertices =
        pgr_get_bigIntArray(&size_sink_verticesArr, ends);

    pgr_edge_t *edges = NULL;

    size_t total_edges = 0;

    /* NOTE:
     * For flow, cost and reverse_cost are really capacity and reverse_capacity
     */
    pgr_get_flow_edges(edges_sql, &edges, &total_edges);

    if (total_edges == 0) {
        if (source_vertices) pfree(source_vertices);
        if (sink_vertices) pfree(sink_vertices);
        pgr_SPI_finish();
        return;
    }


    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char *err_msg = NULL;

    do_pgr_max_flow(
            edges, total_edges,
            source_vertices, size_source_verticesArr,
            sink_vertices, size_sink_verticesArr,
            algorithm,
            only_flow,

            result_tuples, result_count,

            &log_msg,
            &notice_msg,
            &err_msg);

    if (only_flow) {
        time_msg("pgr_maxFlow(many to many)",
                start_t, clock());
    } else if (algorithm == 1) {
        time_msg("pgr_maxFlowPushRelabel(many to many)",
                start_t, clock());
    } else if (algorithm == 3) {
        time_msg("pgr_maxFlowEdmondsKarp(many to many)",
                start_t, clock());
    } else {
        time_msg("pgr_maxFlowBoykovKolmogorov(many to many)",
                start_t, clock());
    }


    if (edges) pfree(edges);
    if (source_vertices) pfree(source_vertices);
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


PG_FUNCTION_INFO_V1(max_flow_many_to_many);
PGDLLEXPORT Datum
max_flow_many_to_many(PG_FUNCTION_ARGS) {
    FuncCallContext *funcctx;
    TupleDesc tuple_desc;

    /**************************************************************************/
    pgr_flow_t *result_tuples = 0;
    size_t result_count = 0;
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_ARRAYTYPE_P(1),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_INT32(3),
                PG_GETARG_BOOL(4),
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
    result_tuples = (pgr_flow_t *) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple tuple;
        Datum result;
        Datum *values;
        bool *nulls;
        size_t call_cntr = funcctx->call_cntr;

        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(bool));

        size_t i;
        for (i = 0; i < 6; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[2] = Int64GetDatum(result_tuples[call_cntr].source);
        values[3] = Int64GetDatum(result_tuples[call_cntr].target);
        values[4] = Int64GetDatum(result_tuples[call_cntr].flow);
        values[5] = Int64GetDatum(result_tuples[call_cntr].residual_capacity);
        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

