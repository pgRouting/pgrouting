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

#include "./../../common/src/postgres_connection.h"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "funcapi.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "utils/array.h"

#include "./../../common/src/debug_macro.h"
#include "./../../common/src/time_msg.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"
#include "./../../common/src/points_input.h"
#include "./../../withPoints/src/get_new_queries.h"
#include "./withPoints_dd_driver.h"


PGDLLEXPORT Datum many_withPointsDD(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(many_withPointsDD);

static 
void process(
        char* edges_sql,
        char* points_sql,
        int64_t *start_pids_arr, size_t size_arr,
        double distance,

        bool directed,
        char *driving_side,
        bool details,
        bool equicost, 

        General_path_element_t **result_tuples,
        size_t *result_count  ){
    driving_side[0] = estimate_drivingSide(driving_side[0]);
    PGR_DBG("estimated driving side:%c", driving_side[0]);

    pgr_SPI_connect();

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
    pgr_get_edges(edges_of_points_query, &edges_of_points, &total_edges_of_points);

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_no_points_query, &edges, &total_edges);

    PGR_DBG("freeing allocated memory not used anymore");
    free(edges_of_points_query);
    free(edges_no_points_query);

    if ((total_edges + total_edges_of_points) == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    clock_t start_t = clock();
    int errcode = do_pgr_many_withPointsDD(
            edges,              total_edges,
            points,             total_points,
            edges_of_points,    total_edges_of_points,
            start_pids_arr,     size_arr,
            distance,

            directed,
            driving_side[0],
            details,
            equicost, 

            result_tuples,
            result_count,
            &err_msg);
    time_msg(" processing withPointsDD many starts", start_t, clock());

    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("Returned message = %s\n", err_msg);

    if (!err_msg) free(err_msg);

    pfree(edges);
    pfree(edges_of_points);
    pfree(points);

    pgr_SPI_finish();

    if (errcode) {
        pgr_send_error(errcode);
    }
}



PGDLLEXPORT Datum
many_withPointsDD(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc               tuple_desc;

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

        int64_t *sourcesArr= NULL;
        size_t num = 0;

        sourcesArr = (int64_t*) pgr_get_bigIntArray(&num, PG_GETARG_ARRAYTYPE_P(2));
        PGR_DBG("sourcesArr size %ld ", num);

        PGR_DBG("Calling driving_many_to_dist_driver");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),  // edges_sql
                text_to_cstring(PG_GETARG_TEXT_P(1)),  // points_sql
                sourcesArr, num,                     // start_pids array
                PG_GETARG_FLOAT8(3),                 // distance

                PG_GETARG_BOOL(4),                   // directed
                text_to_cstring(PG_GETARG_TEXT_P(5)),  // driving side
                PG_GETARG_BOOL(6),                   // details
                PG_GETARG_BOOL(7),                   // equicost
                &result_tuples, &result_count);

        pfree(sourcesArr);

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
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

    tuple_desc = funcctx->tuple_desc;
    result_tuples = (General_path_element_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;

        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(bool));

        nulls[0] = false;
        nulls[1] = false;
        nulls[2] = false;
        nulls[3] = false;
        nulls[4] = false;
        nulls[5] = false;
        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].start_id);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].node);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].edge);
        values[4] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[5] = Float8GetDatum(result_tuples[funcctx->call_cntr].agg_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        /* clean up (this is not really necessary) */
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

