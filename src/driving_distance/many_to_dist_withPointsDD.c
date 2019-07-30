/*PGR-GNU*****************************************************************
File: many_to_dist_driving_distance.c

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_Vergara@hotmail.com

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

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"
#include "c_common/points_input.h"

#include "drivers/withPoints/get_new_queries.h"
#include "drivers/driving_distance/withPoints_dd_driver.h"


PGDLLEXPORT Datum _pgr_withpointsdd(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_withpointsdd);

static
void process(
        char* edges_sql,
        char* points_sql,
        ArrayType* starts,
        double distance,

        bool directed,
        char *driving_side,
        bool details,
        bool equicost,

        General_path_element_t **result_tuples,
        size_t *result_count) {
    driving_side[0] = estimate_drivingSide(driving_side[0]);
    PGR_DBG("estimated driving side:%c", driving_side[0]);

    pgr_SPI_connect();

    size_t total_starts = 0;
    int64_t* start_pidsArr = pgr_get_bigIntArray(&total_starts, starts);
    PGR_DBG("sourcesArr size %ld ", total_starts);

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
    pgr_get_edges(
            edges_of_points_query, &edges_of_points, &total_edges_of_points);

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_no_points_query, &edges, &total_edges);

    PGR_DBG("freeing allocated memory not used anymore");
    pfree(edges_of_points_query);
    pfree(edges_no_points_query);

    if ((total_edges + total_edges_of_points) == 0) {
        if (edges) pfree(edges);
        if (edges_of_points) pfree(edges_of_points);
        if (points) pfree(points);
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    do_pgr_many_withPointsDD(
            edges,              total_edges,
            points,             total_points,
            edges_of_points,    total_edges_of_points,
            start_pidsArr,      total_starts,
            distance,

            directed,
            driving_side[0],
            details,
            equicost,

            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg(" processing withPointsDD many starts", start_t, clock());

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
    if (edges_of_points) pfree(edges_of_points);
    if (points) pfree(points);
    if (start_pidsArr) pfree(start_pidsArr);
    pgr_SPI_finish();
}



PGDLLEXPORT Datum
_pgr_withpointsdd(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc               tuple_desc;

    /**********************************************************************/
    General_path_element_t  *result_tuples = 0;
    size_t result_count = 0;
    /**********************************************************************/


    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        // CREATE OR REPLACE FUNCTION pgr_withPointsDD(
        // edges_sql TEXT,
        // points_sql TEXT,
        // start_pids anyarray,
        // distance FLOAT,
        //
        // directed BOOLEAN -- DEFAULT true,
        // driving_side CHAR -- DEFAULT 'b',
        // details BOOLEAN -- DEFAULT false,
        // equicost BOOLEAN -- DEFAULT false,


        PGR_DBG("Calling driving_many_to_dist_driver");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_FLOAT8(3),

                PG_GETARG_BOOL(4),
                text_to_cstring(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                &result_tuples, &result_count);

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
        Datum *values;
        bool* nulls;

        /**********************************************************************/
        size_t numb = 6;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].start_id);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[4] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[5] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);

        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

