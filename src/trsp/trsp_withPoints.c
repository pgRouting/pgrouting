/*PGR-GNU*****************************************************************
File: trsp_withPoints.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo
Mail: vicky at georepublic.de

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

#include "c_types/path_rt.h"
#include "c_types/point_on_edge_t.h"
#include "c_types/restriction_t.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/arrays_input.h"
#include "c_common/pgdata_getters.h"

#include "drivers/withPoints/get_new_queries.h"
#include "drivers/trsp/trsp_withPoints_driver.h"

PGDLLEXPORT Datum _pgr_trsp_withpoints(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_trsp_withpoints);


static
void
process(
        char* edges_sql,
        char* restrictions_sql,
        char* points_sql,
        char* combinations_sql,

        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        char *driving_side,
        bool details,

        Path_rt **result_tuples,
        size_t *result_count) {
    driving_side[0] = estimate_drivingSide(driving_side[0]);
    if (driving_side[0] != 'r' && driving_side[0] != 'l') {
        driving_side[0] = 'l';
    }

    pgr_SPI_connect();

    size_t size_start_pidsArr = 0;
    int64_t* start_pidsArr = NULL;

    size_t size_end_pidsArr = 0;
    int64_t* end_pidsArr = NULL;

    II_t_rt *combinations = NULL;
    size_t total_combinations = 0;

    /* managing edges */
    char *edges_of_points_query = NULL;
    char *edges_no_points_query = NULL;
    get_new_queries(
            edges_sql, points_sql,
            &edges_of_points_query,
            &edges_no_points_query);

    Edge_t *edges_of_points = NULL;
    size_t total_edges_of_points = 0;

    Edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_of_points_query, &edges_of_points, &total_edges_of_points, true, false);
    pgr_get_edges(edges_no_points_query, &edges, &total_edges, true, false);

    pfree(edges_of_points_query);
    pfree(edges_no_points_query);
    edges_of_points_query = NULL;
    edges_no_points_query = NULL;

    if ((total_edges + total_edges_of_points) == 0) {
        pgr_SPI_finish();
        return;
    }

    /* Managing departure & destination */
    if (starts && ends) {
        start_pidsArr = (int64_t*) pgr_get_bigIntArray(&size_start_pidsArr, starts);
        end_pidsArr = (int64_t*) pgr_get_bigIntArray(&size_end_pidsArr, ends);
    } else if (combinations_sql) {
        pgr_get_combinations(combinations_sql, &combinations, &total_combinations);
    }

    /* Managing Points */
    Point_on_edge_t *points = NULL;
    size_t total_points = 0;
    pgr_get_points(points_sql, &points, &total_points);

    /* Managing restrictions */
    Restriction_t *restrictions = NULL;
    size_t restrictions_size = 0;
    pgr_get_restrictions(restrictions_sql, &restrictions, &restrictions_size);


    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    do_trsp_withPoints(
            edges, total_edges,
            restrictions, restrictions_size,
            points, total_points,
            edges_of_points, total_edges_of_points,

            combinations, total_combinations,

            start_pidsArr, size_start_pidsArr,
            end_pidsArr, size_end_pidsArr,

            directed,
            driving_side[0],
            details,

            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing pgr_trps_withPoints", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_count) = 0;
        (*result_tuples) = NULL;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edges) {pfree(edges); edges = NULL;}
    if (edges_of_points) {pfree(edges_of_points); edges_of_points = NULL;}
    if (edges_of_points) {pfree(edges_of_points); edges_of_points = NULL;}
    if (start_pidsArr) {pfree(start_pidsArr); start_pidsArr = NULL;}
    if (end_pidsArr) {pfree(end_pidsArr); end_pidsArr = NULL;}
    if (combinations) {pfree(combinations); combinations = NULL;}

    pgr_SPI_finish();
}




PGDLLEXPORT Datum
_pgr_trsp_withpoints(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Path_rt *result_tuples = 0;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        if (PG_NARGS() == 8) {
            /*
             * many to many
             */

            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                NULL,  // combinations
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_ARRAYTYPE_P(4),
                PG_GETARG_BOOL(5),
                text_to_cstring(PG_GETARG_TEXT_P(6)),
                PG_GETARG_BOOL(7),
                &result_tuples,
                &result_count);

        } else if (PG_NARGS() == 7) {
            /*
             * Combinations
             */

            process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                text_to_cstring(PG_GETARG_TEXT_P(3)),
                NULL, NULL,  // starts & ends
                PG_GETARG_BOOL(5),
                text_to_cstring(PG_GETARG_TEXT_P(5)),
                PG_GETARG_BOOL(6),
                &result_tuples,
                &result_count);
        }

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
    result_tuples = (Path_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;
        size_t call_cntr = funcctx->call_cntr;

        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        int path_id = call_cntr == 0? 0 : result_tuples[call_cntr - 1].seq;
        path_id += result_tuples[call_cntr].seq == 1? 1 : 0;

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[call_cntr].end_id);
        values[4] = Int64GetDatum(result_tuples[call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[call_cntr].agg_cost);

        result_tuples[call_cntr].seq = path_id;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

