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

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

/*
  Uncomment when needed
*/
#define DEBUG

#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/points_input.h"
#include "./one_to_one_withPoints_driver.h"

PG_FUNCTION_INFO_V1(one_to_one_withPoints);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
one_to_one_withPoints(PG_FUNCTION_ARGS);


/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process(
        char* edges_sql,
        char* points_sql,
        int64_t start_pid,
        int64_t end_pid,
        char driving_side,
        bool directed,
        General_path_element_t **result_tuples,
        size_t *result_count) {

    driving_side = tolower(driving_side);
    if (! ((driving_side == 'r')
                || (driving_side == 'l'))) {
        driving_side = 'b'; 
    }

    pgr_SPI_connect();

    PGR_DBG("load the points");
    Point_on_edge_t *points = NULL;
    int64_t total_points = 0;
    pgr_get_points(points_sql, &points, &total_points);

#ifdef DEBUG
    int i = 0;
    for (i = 0; i < total_points; i ++) {
       PGR_DBG("%ld\t%ld\t%f\t%c",points[i].pid, points[i].edge_id, points[i].fraction, points[i].side);
    }
#endif

    PGR_DBG("load the edges that match the points");
    pgr_edge_t *edges_of_points = NULL;
    int64_t total_edges_of_points = 0;
    PGR_DBG("  -- change the query");
    char *edges_of_points_query = NULL;
#ifdef DEBUG
    for (i = 0; i < total_edges_of_points; i ++) {
       PGR_DBG("%ld\t%ld\t%ld\t%f\t%f",
               edges_of_points[i].id,
               edges_of_points[i].source,
               edges_of_points[i].target,
               edges_of_points[i].cost,
               edges_of_points[i].reverse_cost);
    }
#endif

    PGR_DBG("load the edges that match the points");
    PGR_DBG("Total %ld edges in query:", total_edges_of_points);

    PGR_DBG("load the edges that dont match the points");
    pgr_edge_t *edges = NULL;
    int64_t total_edges = 0;
    PGR_DBG("  -- change the query");
    char *edges_no_points = NULL;
    pgr_get_data_5_columns(edges_sql, &edges, &total_edges);
#ifdef DEBUG
    for (i = 0; i < total_edges; i ++) {
       PGR_DBG("%ld\t%ld\t%ld\t%f\t%f",
               edges[i].id,
               edges[i].source,
               edges[i].target,
               edges[i].cost,
               edges[i].reverse_cost);
    }
#endif
    PGR_DBG("Total %ld edges in query:", total_edges);

    if ( (total_edges + total_edges_of_points) == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    char *err_msg = (char *)"";
    do_pgr_withPoints(
            edges,
            total_edges,
            points,
            total_points,
            edges_of_points,
            total_edges_of_points,
            start_pid,
            end_pid,
            directed,
            result_tuples,
            result_count,
            &err_msg);
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("Returned message = %s\n", err_msg);

    free(err_msg);
    pfree(edges);
    pgr_SPI_finish();
}
/*                                                                             */
/*******************************************************************************/

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
one_to_one_withPoints(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    size_t              call_cntr;
    size_t               max_calls;
    TupleDesc            tuple_desc;

    /*******************************************************************************/
    /*                          MODIFY AS NEEDED                                   */
    /*                                                                             */
    General_path_element_t  *result_tuples = 0;
    size_t result_count = 0;
    /*                                                                             */
    /*******************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /*******************************************************************************/
        /*                          MODIFY AS NEEDED                                   */
        // CREATE OR REPLACE FUNCTION pgr_withPoint(
        // edges_sql TEXT,
        // points_sql TEXT,
        // start_pid BIGINT,
        // end_pid BIGINT,
        // driving_side CHAR DEFAULT 'b',
        // directed BOOLEAN DEFAULT true,

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                pgr_text2char(PG_GETARG_TEXT_P(1)),
                PG_GETARG_INT64(2),
                PG_GETARG_INT64(3),
                PG_GETARG_CHAR(4),
                PG_GETARG_BOOL(5),
                &result_tuples,
                &result_count);

        /*                                                                             */
        /*******************************************************************************/

        funcctx->max_calls = result_count;
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (General_path_element_t*) funcctx->user_fctx;

    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        char*        nulls;

        /*******************************************************************************/
        /*                          MODIFY AS NEEDED                                   */
        // OUT seq BIGINT,
        // OUT path_seq,
        // OUT node BIGINT,
        // OUT edge BIGINT,
        // OUT cost FLOAT,
        // OUT agg_cost FLOAT)


        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(char));

        size_t i;
        for(i = 0; i < 6; ++i) {
            nulls[i] = ' ';
        }


        // postgres starts counting from 1
        values[0] = Int64GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[call_cntr].seq + 1);
        values[3] = Int64GetDatum(result_tuples[call_cntr].vertex);
        values[4] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[5] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[6] = Float8GetDatum(result_tuples[call_cntr].tot_cost);
        /*******************************************************************************/

        tuple = heap_formtuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

