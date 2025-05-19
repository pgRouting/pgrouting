/*PGR-GNU*****************************************************************
File: driving_distance_withPoints.c

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Yige Huang
Mail: square1ge at gmail.com
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

#include "c_types/mst_rt.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "drivers/withPoints/get_new_queries.h"
#include "drivers/driving_distance/driving_distance_withPoints_driver.h"

PGDLLEXPORT Datum _pgr_withpointsddv4(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_withpointsddv4);

static
void
process(
        char* edges_sql,
        char* points_sql,
        ArrayType* starts,
        double distance,

        bool directed,
        char *driving_side,
        bool details,
        bool equicost,

        MST_rt **result_tuples,
        size_t *result_count) {
    char d_side = estimate_drivingSide(driving_side[0]);
    if (d_side == ' ') {
        pgr_throw_error("Invalid value of 'driving side'", "Valid value are 'r', 'l', 'b'");
        return;
    } else if (directed && !(d_side == 'r' || d_side == 'l')) {
        pgr_throw_error("Invalid value of 'driving side'", "Valid values are for directed graph are: 'r', 'l'");
        return;
    } else if (!directed && !(d_side == 'b')) {
        pgr_throw_error("Invalid value of 'driving side'", "Valid values are for undirected graph is: 'b'");
        return;
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
    pgr_do_withPointsDD(
            edges_no_points_query,
            points_sql,
            edges_of_points_query,
            starts,
            distance,
            d_side,

            directed,
            details,
            equicost,

            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg(" processing withPointsDD", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_count) = 0;
        (*result_tuples) = NULL;
    }

    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
_pgr_withpointsddv4(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    MST_rt *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_FLOAT8(3),

                PG_GETARG_BOOL(5),
                text_to_cstring(PG_GETARG_TEXT_P(4)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),

                &result_tuples, &result_count);

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
    result_tuples = (MST_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int64GetDatum((int64_t)funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].depth);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].from_v);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].pred);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);

        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
