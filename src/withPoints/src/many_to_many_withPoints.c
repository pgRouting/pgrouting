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

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif


#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/time_msg.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"
#include "./../../common/src/points_input.h"
#include "./get_new_queries.h"
#include "./many_to_many_withPoints_driver.h"

PGDLLEXPORT Datum many_to_many_withPoints(PG_FUNCTION_ARGS);


/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process(
        char* edges_sql,
        char* points_sql,

        int64_t *start_pidsArr,
        size_t size_start_pidsArr,

        int64_t *end_pidsArr,
        size_t size_end_pidsArr,

        bool directed,
        char *driving_side,
        bool details,
        bool only_cost,
        General_path_element_t **result_tuples,
        size_t *result_count) {

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

    free(edges_of_points_query);
    free(edges_no_points_query);

    if ((total_edges + total_edges_of_points) == 0) {
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    char *err_msg = NULL;
    char *log_msg = NULL;
    clock_t start_t = clock();
    do_pgr_many_to_many_withPoints(
            edges, total_edges,
            points, total_points,
            edges_of_points, total_edges_of_points,
            start_pidsArr, size_start_pidsArr,
            end_pidsArr, size_end_pidsArr,
            driving_side[0],
            details,
            directed,
            only_cost,
            result_tuples,
            result_count,
            &err_msg);
    time_msg("Processing withPoints many to many", start_t, clock());
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("LOG: %s\n", err_msg);
    if (log_msg) free(log_msg);

    if (err_msg) {
        free(start_pidsArr);
        free(end_pidsArr);
        free(*result_tuples);
        elog(ERROR, "%s", err_msg);
        free(err_msg);
    }
    pfree(edges);
    pgr_SPI_finish();
}

/*                                                                             */
/*******************************************************************************/

PG_FUNCTION_INFO_V1(many_to_many_withPoints);
PGDLLEXPORT Datum
many_to_many_withPoints(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t              call_cntr;
    uint32_t               max_calls;
    TupleDesc            tuple_desc;

    /*******************************************************************************/
    /*                          MODIFY AS NEEDED                                   */
    /*                                                                             */
    General_path_element_t *result_tuples = 0;
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
        // end_pids ANYARRAY,
        // driving_side CHAR -- DEFAULT 'b',
        // details BOOLEAN -- DEFAULT false,
        // directed BOOLEAN -- DEFAULT true,
        // only_cost BOOLEAN DEFAULT false,


        int64_t* start_pidsArr;
        size_t size_start_pidsArr;
        start_pidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_pidsArr, PG_GETARG_ARRAYTYPE_P(2));

        int64_t* end_pidsArr;
        size_t size_end_pidsArr;
        end_pidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_end_pidsArr, PG_GETARG_ARRAYTYPE_P(3));

        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                pgr_text2char(PG_GETARG_TEXT_P(1)),
                start_pidsArr, size_start_pidsArr,
                end_pidsArr, size_end_pidsArr,
                PG_GETARG_BOOL(4),
                pgr_text2char(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                &result_tuples,
                &result_count);

        if (start_pidsArr) free(start_pidsArr);
        if (end_pidsArr) free(end_pidsArr);
        /*                                                                             */
        /*******************************************************************************/

        funcctx->max_calls = (uint32_t)result_count;
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
        bool*        nulls;

        /*******************************************************************************/
        /*                          MODIFY AS NEEDED                                   */
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


        // postgres starts counting from 1
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[call_cntr].end_id);
        values[4] = Int64GetDatum(result_tuples[call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
        /*******************************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

