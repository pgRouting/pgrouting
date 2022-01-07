/*PGR-GNU*****************************************************************
File: trsp_withPointsVia.c

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

#include "drivers/withPoints/get_new_queries.h"
#include "drivers/dijkstra/vias_driver.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_types/edge_t.h"
#include "c_types/restriction_t.h"
#include "c_types/routes_t.h"

#include "c_common/edges_input.h"
#include "c_common/restrictions_input.h"
#include "c_common/arrays_input.h"
#include "c_common/combinations_input.h"
#include "c_common/points_input.h"

PGDLLEXPORT Datum _pgr_withpointsvia(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_withpointsvia);

static
void process(
        char* edges_sql,
        char* points_sql,

        ArrayType *via_vertices,

        bool directed,

        bool strict,
        bool allow_u_turn,

        bool details,
        char *driving_side,

        Routes_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("directed %d", directed);
    PGR_DBG("strict %d", strict);
    PGR_DBG("allow_u_turn %d", allow_u_turn);
    PGR_DBG("details %d", details);
    PGR_DBG("driving_side %s", driving_side);

    /* Managing Points */
    driving_side[0] = estimate_drivingSide(driving_side[0]);
    Point_on_edge_t *points = NULL;
    size_t total_points = 0;
    pgr_get_points(points_sql, &points, &total_points);

    char *edges_of_points_query = NULL;
    char *edges_no_points_query = NULL;
    get_new_queries(
            edges_sql, points_sql,
            &edges_of_points_query,
            &edges_no_points_query);

    /* Managing Via */
    size_t size_via_arr = 0;
    int64_t* via_arr = (int64_t*) pgr_get_bigIntArray(&size_via_arr, via_vertices);

    if (size_via_arr < 2) {
        if (via_arr) {pfree(via_arr); via_arr = NULL;}
        elog(WARNING, "Not enough via points");
        pgr_SPI_finish();
        return;
    }

    /* Managing the edges */
    Edge_t *edges = NULL;
    size_t total_edges = 0;

    Edge_t *edges_of_points = NULL;
    size_t total_edges_of_points = 0;


    pgr_get_edges(edges_of_points_query, &edges_of_points, &total_edges_of_points);
    pgr_get_edges(edges_no_points_query, &edges, &total_edges);

    /* Cleanup of no longer needed memory */
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
    do_dijkstra_vias(
            edges, total_edges,
            NULL, 0,  // Restrictions
            points, total_points,
            edges_of_points, total_edges_of_points,

            via_arr, size_via_arr,
            driving_side[0],

            directed,
            strict,
            allow_u_turn,

            result_tuples,
            result_count,

            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg("processing pgr_dijkstraVia", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    /* TODO free everything */
    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edges) pfree(edges);
    if (via_arr) pfree(via_arr);
    pgr_SPI_finish();
}


PGDLLEXPORT Datum
_pgr_withpointsvia(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    Routes_t  *result_tuples = 0;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************
         * pgr_dijkstraVia(edges_sql text,
         *   vertices anyarray,
         *   directed boolean default true,
         *   strict boolean default false,
         *   U_turn_on_edge boolean default false,
         **********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_BOOL(3),
                PG_GETARG_BOOL(4),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),
                text_to_cstring(PG_GETARG_TEXT_P(7)),
                &result_tuples,
                &result_count);

        /**********************************************************************/

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

        // postgres starts counting from 1
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

        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
