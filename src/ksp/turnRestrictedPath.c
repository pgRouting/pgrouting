/*PGR-GNU*****************************************************************
File: turnRestrictedPath.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 vicky Vergara
Mail: vicky at erosion.dev

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
#include "c_common/time_msg.h"
#include "c_common/e_report.h"
#include "drivers/yen/turnRestrictedPath_driver.h"

PGDLLEXPORT Datum _pgr_turnrestrictedpath_v4(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_turnrestrictedpath_v4);


static
void
process(
        char* edges_sql,
        char* restrictions_sql,
        char* combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        int p_k,
        bool directed,
        bool heap_paths,
        bool stop_on_first,
        bool strict,

        Path_rt **result_tuples,
        size_t *result_count) {
    (*result_tuples) = NULL;
    (*result_count) = 0;

    if (p_k < 0) {
        pgr_throw_error("Invalid value of 'K'", "Valid value are greater than 0");
    }

    pgr_SPI_connect();

    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    clock_t start_t = clock();
    pgr_do_turnRestrictedPath(
            edges_sql,
            restrictions_sql,
            combinations_sql,
            starts, ends,
            (size_t) p_k,

            directed,
            heap_paths,
            stop_on_first,
            strict,

            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg(" processing pgr_turnRestrictedPath", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
_pgr_turnrestrictedpath_v4(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Path_rt  *result_tuples = 0;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                NULL,
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_INT32(4),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                PG_GETARG_BOOL(8),
                &result_tuples,
                &result_count);

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

        size_t n = 9;
        values = palloc(n * sizeof(Datum));
        nulls = palloc(n * sizeof(bool));

        size_t i;
        for (i = 0; i < n; ++i) {
            nulls[i] = false;
        }

        Path_rt data = result_tuples[funcctx->call_cntr];

        int64_t path_id = 1;
        if (funcctx->call_cntr != 0) {
            if (result_tuples[funcctx->call_cntr - 1].edge == -1) {
                path_id = result_tuples[funcctx->call_cntr - 1].start_id + 1;
            } else {
                path_id = result_tuples[funcctx->call_cntr - 1].start_id;
            }
        }
        int64_t seq = funcctx->call_cntr == 0?  1 : result_tuples[funcctx->call_cntr - 1].end_id;

        values[0] = Int32GetDatum((int32_t)funcctx->call_cntr + 1);
        values[1] = Int32GetDatum((int32_t)path_id);
        values[2] = Int32GetDatum((int32_t)seq);
        values[3] = Int64GetDatum(data.start_id);
        values[4] = Int64GetDatum(data.end_id);
        values[5] = Int64GetDatum(data.node);
        values[6] = Int64GetDatum(data.edge);
        values[7] = Float8GetDatum(data.cost);
        values[8] = Float8GetDatum(data.agg_cost);

        result_tuples[funcctx->call_cntr].start_id = path_id;
        result_tuples[funcctx->call_cntr].end_id = data.edge < 0? 1 : seq + 1;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
