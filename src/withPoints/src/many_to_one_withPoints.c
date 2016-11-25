/*PGR-GNU*****************************************************************
File: one_to_many_withPoints.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

#include <funcapi.h>

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
#include "./../../common/src/points_input.h"
#include "./get_new_queries.h"
#include "./many_to_many_withPoints_driver.h"

PGDLLEXPORT Datum many_to_one_withPoints(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(many_to_one_withPoints);


static
void
process(
        char* edges_sql,
        char* points_sql,
        ArrayType *starts,
        int64_t end_pid,
        bool directed,
        char* driving_side,
        bool details,
        bool only_cost,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    driving_side[0] = estimate_drivingSide(driving_side[0]);
    PGR_DBG("estimated driving side:%c", driving_side[0]);

    pgr_SPI_connect();

    size_t size_start_pidsArr = 0;
    int64_t* start_pidsArr =
        pgr_get_bigIntArray(&size_start_pidsArr, starts);

    Point_on_edge_t *points = NULL;
    size_t total_points = 0;
    pgr_get_points(points_sql, &points, &total_points);


    char *edges_of_points_query = NULL;
    char *edges_no_points_query = NULL;
    get_new_queries(
            edges_sql, points_sql,
            &edges_of_points_query,
            &edges_no_points_query);


    pgr_edge_t *edges_of_points = NULL;
    size_t total_edges_of_points = 0;
    pgr_get_edges_reversed(
            edges_of_points_query, &edges_of_points, &total_edges_of_points);


    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges_reversed(edges_no_points_query, &edges, &total_edges);

    free(edges_of_points_query);
    free(edges_no_points_query);

    if ((total_edges + total_edges_of_points) == 0) {
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    do_pgr_many_to_many_withPoints(
            edges, total_edges,
            points, total_points,
            edges_of_points, total_edges_of_points,
            &end_pid, 1,
            start_pidsArr, size_start_pidsArr,
            driving_side[0],
            details,
            directed,
            only_cost,
            false,
            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    if (only_cost) {
        time_msg("processing pgr_withPointsCost(many to one)",
                start_t, clock());
    } else {
        time_msg("processing pgr_withPoints(many to one)", start_t, clock());
    }

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_count) = 0;
        (*result_tuples) = NULL;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edges) pfree(edges);
    if (start_pidsArr) pfree(start_pidsArr);

    pgr_SPI_finish();
}


PGDLLEXPORT Datum
many_to_one_withPoints(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    General_path_element_t *result_tuples = NULL;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        // CREATE OR REPLACE FUNCTION pgr_withPoint(
        // edges_sql TEXT,
        // points_sql TEXT,
        // start_pid BIGINT,
        // end_pids ANYARRAY,
        // driving_side CHAR -- DEFAULT 'b',
        // details BOOLEAN -- DEFAULT false,
        // directed BOOLEAN -- DEFAULT true,
        // only_cost BOOLEAN DEFAULT false,

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_INT64(3),
                PG_GETARG_BOOL(4),
                text_to_cstring(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
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
                != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

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
        // OUT seq BIGINT,
        // OUT path_seq,
        // OUT node BIGINT,
        // OUT edge BIGINT,
        // OUT cost FLOAT,
        // OUT agg_cost FLOAT)


        values = palloc(7 * sizeof(Datum));
        nulls = palloc(7 * sizeof(bool));

        size_t i;
        for (i = 0; i < 7; ++i) {
            nulls[i] = false;
        }


        // postgres starts counting from 1
        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[funcctx->call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[5] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[6] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
