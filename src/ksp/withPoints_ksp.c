/*PGR-GNU*****************************************************************
File: withPoints_ksp.c

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Abhinav Jain
Mail: this.abhinav at gmail.com

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
#include "drivers/withPoints/get_new_queries.h"
#include "process/kPathsWithPoints_process.h"

PGDLLEXPORT Datum _pgr_withpointsksp_v4(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_withpointsksp_v4);

PGDLLEXPORT Datum _pgr_withpointsksp_v4(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Path_rt  *result_tuples = 0;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        if (PG_NARGS() == 9) {
            pgr_process_kPathsWithPoints(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                NULL,

                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),

                PG_GETARG_INT32(4),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),

                text_to_cstring(PG_GETARG_TEXT_P(5))[0],
                PG_GETARG_BOOL(8),

                NULL, NULL,

                KSPWITHPOINTS,
                &result_tuples,
                &result_count);
        } else if (PG_NARGS() == 8) {
            pgr_process_kPathsWithPoints(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),

                NULL, NULL,

                PG_GETARG_INT32(3),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),

                text_to_cstring(PG_GETARG_TEXT_P(4))[0],
                PG_GETARG_BOOL(7),

                NULL, NULL,

                KSPWITHPOINTS,
                &result_tuples,
                &result_count);
        }


        funcctx->max_calls = result_count;
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context that cannot accept type record")));
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
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].start_id);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].end_id);
        values[5] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[6] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[7] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[8] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        result_tuples[funcctx->call_cntr].start_id = path_id;
        result_tuples[funcctx->call_cntr].end_id = result_tuples[funcctx->call_cntr].edge < 0? 1 : seq + 1;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

/* Deprecated code starts here
 * This code is used on v3.8 and under
 *
 * TODO(v4.2) define SHOWMSG
 * TODO(v4.3) change to WARNING
 * TODO(v5) Move to legacy
 */
PGDLLEXPORT Datum _pgr_withpointsksp(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_withpointsksp);

PGDLLEXPORT Datum _pgr_withpointsksp(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Path_rt  *result_tuples = 0;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

#ifdef SHOWMSG
        ereport(NOTICE, (
                    errcode(ERRCODE_WARNING_DEPRECATED_FEATURE),
                    errmsg("A stored procedure is using deprecated C internal function '%s'", __func__),
                    errdetail("Library function '%s' was deprecated in pgRouting %s", __func__, "4.0.0"),
                    errhint("Consider upgrade pgRouting")));
#endif

        if (PG_NARGS() == 10) {
            pgr_process_kPathsWithPoints(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                NULL,

                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),

                PG_GETARG_INT32(4),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),

                text_to_cstring(PG_GETARG_TEXT_P(5))[0],
                PG_GETARG_BOOL(8),

                NULL, NULL,

                OLDKSPWITHPOINTS,
                &result_tuples,
                &result_count);
        } else if (PG_NARGS() == 8) {
            pgr_process_kPathsWithPoints(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),

                NULL, NULL,

                PG_GETARG_INT32(3),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),

                text_to_cstring(PG_GETARG_TEXT_P(4))[0],
                PG_GETARG_BOOL(7),

                NULL, NULL,

                OLDKSPWITHPOINTS,
                &result_tuples,
                &result_count);
        } else if (PG_NARGS() == 9) {
            /* this is for the old signature */
            int64_t departure = PG_GETARG_INT64(2);
            int64_t destination = PG_GETARG_INT64(3);

            pgr_process_kPathsWithPoints(
                    text_to_cstring(PG_GETARG_TEXT_P(0)),
                    text_to_cstring(PG_GETARG_TEXT_P(1)),
                    NULL,

                    NULL, NULL,

                    PG_GETARG_INT32(4),
                    PG_GETARG_BOOL(5),
                    PG_GETARG_BOOL(6),

                    text_to_cstring(PG_GETARG_TEXT_P(7))[0],
                    PG_GETARG_BOOL(8),

                    &departure, &destination,

                    OLDKSPWITHPOINTS,
                    &result_tuples,
                    &result_count);
        }


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
    result_tuples = (Path_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        size_t n = (PG_NARGS() == 9)? 7 : 9;

        values = palloc(n * sizeof(Datum));
        nulls = palloc(n * sizeof(bool));

        size_t i;
        for (i = 0; i < n; ++i) {
            nulls[i] = false;
        }

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
        if (PG_NARGS() != 9) {
            values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].start_id);
            values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].end_id);
        }
        values[n - 4] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[n - 3] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[n - 2] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[n - 1] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        result_tuples[funcctx->call_cntr].start_id = path_id;
        result_tuples[funcctx->call_cntr].end_id = result_tuples[funcctx->call_cntr].edge < 0? 1 : seq + 1;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
