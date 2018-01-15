/*PGR-GNU*****************************************************************
File: withPoints.c

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

#include "c_common/postgres_connection.h"
#include "utils/array.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"
#include "c_common/points_input.h"
#include "drivers/withPoints/get_new_queries.h"
#include "drivers/withPoints/withPoints_driver.h"

PGDLLEXPORT Datum withPoints(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(withPoints);


static
void
process(
        char* edges_sql,
        char* points_sql,

        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        char *driving_side,
        bool details,
        bool only_cost,
        bool normal,

        General_path_element_t **result_tuples,
        size_t *result_count) {
    driving_side[0] = estimate_drivingSide(driving_side[0]);

    pgr_SPI_connect();

    size_t size_start_pidsArr = 0;
    int64_t* start_pidsArr = NULL;

    size_t size_end_pidsArr = 0;
    int64_t* end_pidsArr = NULL;

    Point_on_edge_t *points = NULL;
    size_t total_points = 0;
    pgr_get_points(points_sql, &points, &total_points);

#ifndef NDEBUG
    size_t i;
    for (i = 0; i< total_points; i++) {
        PGR_DBG("%ld ", points[i].pid);
    }
#endif
    char *edges_of_points_query = NULL;
    char *edges_no_points_query = NULL;
    get_new_queries(
            edges_sql, points_sql,
            &edges_of_points_query,
            &edges_no_points_query);


    pgr_edge_t *edges_of_points = NULL;
    size_t total_edges_of_points = 0;

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;

    if (normal) {
        pgr_get_edges(
                edges_of_points_query,
                &edges_of_points,
                &total_edges_of_points);
        pgr_get_edges(edges_no_points_query, &edges, &total_edges);

        start_pidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_pidsArr, starts);
        end_pidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_end_pidsArr, ends);
    } else {
        pgr_get_edges_reversed(
                edges_of_points_query,
                &edges_of_points,
                &total_edges_of_points);
        pgr_get_edges_reversed(edges_no_points_query, &edges, &total_edges);

        end_pidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_end_pidsArr, starts);
        start_pidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_pidsArr, ends);
    }


    pfree(edges_of_points_query);
    pfree(edges_no_points_query);
    edges_of_points_query = NULL;
    edges_no_points_query = NULL;

    if ((total_edges + total_edges_of_points) == 0) {
        pgr_SPI_finish();
        return;
    }

    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    do_pgr_withPoints(
            edges, total_edges,
            points, total_points,
            edges_of_points, total_edges_of_points,
            start_pidsArr, size_start_pidsArr,
            end_pidsArr, size_end_pidsArr,

            driving_side[0],
            details,
            directed,
            only_cost,
            normal,

            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    if (only_cost) {
        time_msg("processing pgr_withPointsCost(one to one)", start_t, clock());
    } else {
        time_msg("processing pgr_withPoints(one to one)", start_t, clock());
    }

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_count) = 0;
        (*result_tuples) = NULL;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

#if 0
    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edges) pfree(edges);
    if (points) pfree(points);
    if (edges_of_points) pfree(edges_of_points);
    if (start_pidsArr) pfree(start_pidsArr);
    if (end_pidsArr) pfree(end_pidsArr);
#endif
    pgr_SPI_finish();
}


PGDLLEXPORT Datum
withPoints(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    General_path_element_t *result_tuples = 0;
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
        // start_pid ANYARRAY,
        // end_pids ANYARRAY,
        // driving_side CHAR -- DEFAULT 'b',
        // details BOOLEAN -- DEFAULT false,
        // directed BOOLEAN -- DEFAULT true,
        // only_cost BOOLEAN DEFAULT false,
        // normal BOOLEAN DEFAULT true,


        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_BOOL(4),
                text_to_cstring(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                PG_GETARG_BOOL(8),
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


        values = palloc(8 * sizeof(Datum));
        nulls = palloc(8 * sizeof(bool));

        size_t i;
        for (i = 0; i < 8; ++i) {
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
        SRF_RETURN_DONE(funcctx);
    }
}

