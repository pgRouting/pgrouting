/*PGR-GNU*****************************************************************
File: trspVia_withPoints.c

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo

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
#include "c_types/routes_t.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/arrays_input.h"
#include "c_common/pgdata_getters.h"
#include "drivers/withPoints/get_new_queries.h"
#include "drivers/trsp/trspVia_withPoints_driver.h"

PGDLLEXPORT Datum _pgr_trspvia_withpoints(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_trspvia_withpoints);

static
void
process(
        char* edges_sql,
        char* restrictions_sql,
        char* points_sql,
        ArrayType *viasArr,
        bool directed,

        bool strict,
        bool U_turn_on_edge,

        char *driving_side,
        bool details,

        Routes_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    /*
     * Processing Via
     */
    size_t size_via = 0;
    int64_t* via = (int64_t*) pgr_get_bigIntArray(&size_via, viasArr);

    // TODO(vicky) figure out what happens when one point or 0 points are given

    /*
     * Processing Points
     */
    driving_side[0] = estimate_drivingSide(driving_side[0]);
    if (driving_side[0] != 'r' && driving_side[0] != 'l') {
        driving_side[0] = 'l';
    }

    Point_on_edge_t *points = NULL;
    size_t total_points = 0;
    pgr_get_points(points_sql, &points, &total_points);

    char *edges_of_points_query = NULL;
    char *edges_no_points_query = NULL;
    get_new_queries(edges_sql, points_sql, &edges_of_points_query, &edges_no_points_query);

    /*
     * Processing Edges
     */
    Edge_t *edges_of_points = NULL;
    size_t total_edges_of_points = 0;

    Edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_no_points_query, &edges, &total_edges, true, false);
    pgr_get_edges(edges_of_points_query, &edges_of_points, &total_edges_of_points, true, false);

    {pfree(edges_of_points_query); edges_of_points_query = NULL;}
    {pfree(edges_no_points_query); edges_no_points_query = NULL;}

    if ((total_edges + total_edges_of_points) == 0) {
        if (edges) {pfree(edges); edges = NULL;}
        if (edges_of_points) {pfree(edges_of_points); edges_of_points = NULL;}
        if (via) {pfree(via); via = NULL;}
        pgr_SPI_finish();
        return;
    }

    /*
     * Processing restrictions
     */
    Restriction_t * restrictions = NULL;
    size_t size_restrictions = 0;

    pgr_get_restrictions(restrictions_sql, &restrictions, &size_restrictions);

    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    do_trspVia_withPoints(
            edges, total_edges,
            restrictions, size_restrictions,
            points, total_points,
            edges_of_points, total_edges_of_points,
            via, size_via,

            directed,

            driving_side[0],
            details,

            strict,
            U_turn_on_edge,

            result_tuples, result_count,
            &log_msg, &notice_msg, &err_msg);
    time_msg("processing pgr_trsp_withPointsVia", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) {pfree(log_msg); log_msg = NULL;}
    if (notice_msg) {pfree(notice_msg); notice_msg = NULL;}
    if (err_msg) {pfree(err_msg); err_msg = NULL;}
    if (edges) {pfree(edges); edges = NULL;}
    if (via) {pfree(via); via = NULL;}
    if (restrictions) {pfree(restrictions); restrictions = NULL;}
    if (edges_of_points) {pfree(edges_of_points); edges_of_points = NULL;}
    if (points) {pfree(points); points = NULL;}
    pgr_SPI_finish();
}


PGDLLEXPORT Datum
_pgr_trspvia_withpoints(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Routes_t  *result_tuples = 0;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_BOOL(4),

                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),

                text_to_cstring(PG_GETARG_TEXT_P(7)),
                PG_GETARG_BOOL(8),

                &result_tuples,
                &result_count);

        funcctx->max_calls = result_count;
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
    result_tuples = (Routes_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;
        size_t       call_cntr = funcctx->call_cntr;

        size_t numb_out = 10;
        values = palloc(numb_out * sizeof(Datum));
        nulls = palloc(numb_out * sizeof(bool));
        size_t i;
        for (i = 0; i< numb_out; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].path_id);
        values[2] = Int32GetDatum(result_tuples[call_cntr].path_seq + 1);
        values[3] = Int64GetDatum(result_tuples[call_cntr].start_vid);
        values[4] = Int64GetDatum(result_tuples[call_cntr].end_vid);
        values[5] = Int64GetDatum(result_tuples[call_cntr].node);
        values[6] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[7] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[8] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
        values[9] = Float8GetDatum(result_tuples[call_cntr].route_agg_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
