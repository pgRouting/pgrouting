/*PGR-GNU*****************************************************************
File: dijkstra.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakr at yahoo.com, estebanzimanyi at gmail.com

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

#include "c_types/path_rt.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "drivers/dijkstra/dijkstra_driver.h"

PG_MODULE_MAGIC;

PGDLLEXPORT Datum _pgr_dijkstra(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_dijkstra);

static
void
process(
        char *edges_sql,
        char *combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        bool only_cost,
        bool normal,
        int64_t n_goals,
        bool global,

        Path_rt **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    clock_t start_t = clock();
    pgr_do_dijkstra(
            edges_sql,
            combinations_sql,
            starts, ends,

            directed,
            only_cost,
            normal,
            n_goals,
            global,

            result_tuples,
            result_count,

            &log_msg,
            &notice_msg,
            &err_msg);

    if (only_cost) {
        if (n_goals > 0) {
            time_msg("processing pgr_dijkstraNearCost", start_t, clock());
        } else {
            time_msg("processing pgr_dijkstraCost", start_t, clock());
        }
    } else {
        if (n_goals > 0) {
            time_msg("processing pgr_dijkstraNear", start_t, clock());
        } else {
            time_msg("processing pgr_dijkstra", start_t, clock());
        }
    }


    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
_pgr_dijkstra(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Path_rt  *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        if (PG_NARGS() == 7) {
            /* kept for backwards compatibility
             * TODO remove on 4.0.0 */
            process(
                    text_to_cstring(PG_GETARG_TEXT_P(0)),
                    NULL,
                    PG_GETARG_ARRAYTYPE_P(1),
                    PG_GETARG_ARRAYTYPE_P(2),
                    PG_GETARG_BOOL(3),
                    PG_GETARG_BOOL(4),
                    PG_GETARG_BOOL(5),
                    PG_GETARG_INT64(6),
                    true,
                    &result_tuples,
                    &result_count);

        } else if (PG_NARGS() == 5) {
            /* kept for backwards compatibility
             * TODO remove on 4.0.0 */
            process(
                    text_to_cstring(PG_GETARG_TEXT_P(0)),
                    text_to_cstring(PG_GETARG_TEXT_P(1)),
                    NULL, NULL,
                    PG_GETARG_BOOL(2),
                    PG_GETARG_BOOL(3),
                    true, 0, true,
                    &result_tuples,
                    &result_count);

        } else if (PG_NARGS() == 8) {
            process(
                    text_to_cstring(PG_GETARG_TEXT_P(0)),
                    NULL,
                    PG_GETARG_ARRAYTYPE_P(1),
                    PG_GETARG_ARRAYTYPE_P(2),
                    PG_GETARG_BOOL(3),
                    PG_GETARG_BOOL(4),
                    PG_GETARG_BOOL(5),
                    PG_GETARG_INT64(6),
                    PG_GETARG_BOOL(7),
                    &result_tuples,
                    &result_count);

        } else /* (PG_NARGS() == 6) */ {
            process(
                    text_to_cstring(PG_GETARG_TEXT_P(0)),
                    text_to_cstring(PG_GETARG_TEXT_P(1)),
                    NULL, NULL,
                    PG_GETARG_BOOL(2),
                    PG_GETARG_BOOL(3),
                    true,
                    PG_GETARG_INT64(4),
                    PG_GETARG_BOOL(5),
                    &result_tuples,
                    &result_count);
        }

        funcctx->max_calls = result_count;
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
    result_tuples = (Path_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;
        size_t       call_cntr = funcctx->call_cntr;

        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        int64_t seq = call_cntr == 0?  1 : result_tuples[call_cntr - 1].start_id;

        values[0] = Int32GetDatum((int32_t)call_cntr + 1);
        values[1] = Int32GetDatum((int32_t)seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[call_cntr].end_id);
        values[4] = Int64GetDatum(result_tuples[call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[call_cntr].agg_cost);

        result_tuples[call_cntr].start_id = result_tuples[call_cntr].edge < 0? 1 : seq + 1;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
