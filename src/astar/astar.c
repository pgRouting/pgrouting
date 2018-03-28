/*PGR-GNU*****************************************************************
File: astarOneToOne.c

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
#include "c_common/check_parameters.h"

#include "drivers/astar/astar_driver.h"

PGDLLEXPORT Datum astarManyToMany(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(astarManyToMany);

#if 0
void
check_parameters(
        int heuristic,
        double factor,
        double epsilon) {
    if (heuristic > 5 || heuristic < 0) {
        ereport(ERROR,
                (errmsg("Unknown heuristic"),
                 errhint("Valid values: 0~5")));
    }
    if (factor <= 0) {
        ereport(ERROR,
                (errmsg("Factor value out of range"),
                 errhint("Valid values: positive non zero")));
    }
    if (epsilon < 1) {
        ereport(ERROR,
                (errmsg("Epsilon value out of range"),
                 errhint("Valid values: 1 or greater than 1")));
    }
}
#endif

static
void
process(char* edges_sql,
        ArrayType *starts,
        ArrayType *ends,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        bool normal,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    check_parameters(heuristic, factor, epsilon);

    pgr_SPI_connect();

    int64_t* start_vidsArr = NULL;
    size_t size_start_vidsArr = 0;

    int64_t* end_vidsArr = NULL;
    size_t size_end_vidsArr = 0;

    Pgr_edge_xy_t *edges = NULL;
    size_t total_edges = 0;

    if (normal) {
        pgr_get_edges_xy(edges_sql, &edges, &total_edges);
        start_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_vidsArr, starts);
        end_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_end_vidsArr, ends);
    } else {
        pgr_get_edges_xy_reversed(edges_sql, &edges, &total_edges);
        end_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_end_vidsArr, starts);
        start_vidsArr = (int64_t*)
            pgr_get_bigIntArray(&size_start_vidsArr, ends);
    }

    if (total_edges == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    clock_t start_t = clock();
    do_pgr_astarManyToMany(
            edges, total_edges,
            start_vidsArr, size_start_vidsArr,
            end_vidsArr, size_end_vidsArr,
            directed,
            heuristic,
            factor,
            epsilon,
            only_cost,
            normal,
            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    if (only_cost) {
        time_msg("processing pgr_astarCost(many to many)", start_t, clock());
    } else {
        time_msg("processing pgr_astar(many to many)", start_t, clock());
    }


    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edges) pfree(edges);
    if (start_vidsArr) pfree(start_vidsArr);
    if (end_vidsArr) pfree(end_vidsArr);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
astarManyToMany(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    /**********************************************************************/
    General_path_element_t  *result_tuples = NULL;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*
           edges_sql TEXT,
           start_vids ARRAY[ANY_INTEGER], -- anyarray
           end_vids ARRAY[ANY_INTEGER], -- anyarray
           directed BOOLEAN DEFAULT true,
           heuristic INTEGER DEFAULT 0,
           factor FLOAT DEFAULT 1.0,
           epsilon FLOAT DEFAULT 1.0,

         **********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_ARRAYTYPE_P(1),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_BOOL(3),
                PG_GETARG_INT32(4),
                PG_GETARG_FLOAT8(5),
                PG_GETARG_FLOAT8(6),
                PG_GETARG_BOOL(7),
                PG_GETARG_BOOL(8),
                &result_tuples,
                &result_count);


#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
        funcctx->max_calls = (uint32_t) result_count;
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

        /*********************************************************************
          OUT seq INTEGER,
          OUT path_seq INTEGER,
          OUT start_vid BIGINT,
          OUT end_vid BIGINT,
          OUT node BIGINT,
          OUT edge BIGINT,
          OUT cost FLOAT,
          OUT agg_cost FLOAT
         **********************************************************************/


        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
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


        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
