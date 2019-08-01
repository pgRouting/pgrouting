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
#include "drivers/driving_distance/drivedist_driver.h"


PGDLLEXPORT Datum _pgr_drivingdistance(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_drivingdistance);


static
void process(
        char* sql,
        ArrayType *starts,
        float8 distance,
        bool directed,
        bool equicost,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    size_t size_start_vidsArr = 0;
    int64_t* start_vidsArr = pgr_get_bigIntArray(&size_start_vidsArr, starts);

    pgr_edge_t *edges = NULL;
    size_t total_tuples = 0;
    pgr_get_edges(sql, &edges, &total_tuples);

    if (total_tuples == 0) {
        return;
    }

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    do_pgr_driving_many_to_dist(
            edges, total_tuples,
            start_vidsArr, size_start_vidsArr,
            distance,
            directed,
            equicost,
            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing pgr_drivingDistance()",
            start_t, clock());

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

    pgr_SPI_finish();
}


PGDLLEXPORT Datum
_pgr_drivingdistance(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    General_path_element_t* result_tuples = 0;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;

        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        /**********************************************************************/

        PGR_DBG("Calling driving_many_to_dist_driver");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_ARRAYTYPE_P(1),
                PG_GETARG_FLOAT8(2),
                PG_GETARG_BOOL(3),
                PG_GETARG_BOOL(4),
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

