/*PGR-GNU*****************************************************************
File: withPoints_ksp.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
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
#include "drivers/yen/withPoints_ksp_driver.h"

PGDLLEXPORT Datum _pgr_withpointsksp_v4(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_withpointsksp_v4);

static
void
process(
        char* edges_sql,
        char* points_sql,
        char* combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        int64_t* start_vid,
        int64_t* end_vid,

        int p_k,
        char *driving_side,

        bool directed,
        bool heap_paths,
        bool details,

        Path_rt **result_tuples,
        size_t *result_count) {
    if (p_k < 0) {
        /* TODO add error message */
        return;
    }

    size_t k = (size_t)p_k;

    if (start_vid) {
        driving_side[0] = (char) tolower(driving_side[0]);
        if (!((driving_side[0] == 'r')
                    || (driving_side[0] == 'l'))) {
            driving_side[0] = 'b';
        }
    } else {
        driving_side[0] = (char) tolower(driving_side[0]);
        if (directed) {
            if (!((driving_side[0] == 'r') || (driving_side[0] == 'l'))) {
                pgr_throw_error("Invalid value of 'driving side'", "Valid values are for directed graph are: 'r', 'l'");
                return;
            }
        } else if (!(driving_side[0] == 'b')) {
            pgr_throw_error("Invalid value of 'driving side'", "Valid values are for undirected graph is: 'b'");
            return;
        }
    }

    pgr_SPI_connect();

    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    char *edges_of_points_query = NULL;
    char *edges_no_points_query = NULL;
    get_new_queries(
            edges_sql, points_sql,
            &edges_of_points_query,
            &edges_no_points_query);


    clock_t start_t = clock();

    pgr_do_withPointsKsp(
            edges_no_points_query,
            points_sql,
            edges_of_points_query,
            combinations_sql,
            starts, ends,

            start_vid, end_vid,
            k,

            directed,
            heap_paths,
            driving_side[0],
            details,

            result_tuples,
            result_count,

            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg(" processing withPointsKSP", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}

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
            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                NULL,
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),
                NULL, NULL,
                PG_GETARG_INT32(4),
                text_to_cstring(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                PG_GETARG_BOOL(8),
                &result_tuples,
                &result_count);
        } else if (PG_NARGS() == 8) {
            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                NULL, NULL,
                NULL, NULL,
                PG_GETARG_INT32(3),
                text_to_cstring(PG_GETARG_TEXT_P(4)),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
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

/*
 * TODO (v5) v5 remove deprecated code
 * TODO (v4 last micro) warn about deprecated code
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

        if (PG_NARGS() == 10) {
            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                NULL,
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),
                NULL, NULL,
                PG_GETARG_INT32(4),
                text_to_cstring(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                PG_GETARG_BOOL(8),
                &result_tuples,
                &result_count);
        } else if (PG_NARGS() == 8) {
            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                NULL, NULL,
                NULL, NULL,
                PG_GETARG_INT32(3),
                text_to_cstring(PG_GETARG_TEXT_P(4)),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                &result_tuples,
                &result_count);
        } else if (PG_NARGS() == 9) {
            /* this is for the old signature */
            int64_t departure = PG_GETARG_INT64(2);
            int64_t destination = PG_GETARG_INT64(3);

            process(
                    text_to_cstring(PG_GETARG_TEXT_P(0)),
                    text_to_cstring(PG_GETARG_TEXT_P(1)),
                    NULL,
                    NULL, NULL,
                    &departure,
                    &destination,
                    PG_GETARG_INT32(4),
                    text_to_cstring(PG_GETARG_TEXT_P(7)),
                    PG_GETARG_BOOL(5),
                    PG_GETARG_BOOL(6),
                    PG_GETARG_BOOL(8),
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
