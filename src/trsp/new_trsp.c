/*PGR-GNU*****************************************************************

File: new_trsp.c

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

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


#include "drivers/trsp/trsp_driver.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_types/pgr_edge_t.h"
#include "c_types/restriction_t.h"
#include "c_types/general_path_element_t.h"

#include "c_common/edges_input.h"
#include "c_common/restrictions_input.h"
#include "c_common/arrays_input.h"

PGDLLEXPORT Datum turn_restriction(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(turn_restriction);


static
void compute_trsp(
        char* edges_sql,
        char* restrictions_sql,

        ArrayType *starts,
        ArrayType *ends,

        bool directed,

        General_path_element_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_sql, &edges, &total_edges);

    Restriction_t * restrictions = NULL;
    size_t total_restrictions = 0;
    pgr_get_restrictions(restrictions_sql, &restrictions, &total_restrictions);

    int64_t* start_vidsArr = NULL;
    size_t size_start_vidsArr = 0;
    start_vidsArr = (int64_t*)
        pgr_get_bigIntArray(&size_start_vidsArr, starts);

    int64_t* end_vidsArr = NULL;
    size_t size_end_vidsArr = 0;
    end_vidsArr = (int64_t*)
        pgr_get_bigIntArray(&size_end_vidsArr, ends);

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    do_trsp(
            edges,
            total_edges,

            restrictions,
            total_restrictions,

            start_vidsArr, size_start_vidsArr,
            end_vidsArr, size_end_vidsArr,

            directed,

            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg("processing _pgr_trsp", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    pgr_SPI_finish();
}



PGDLLEXPORT Datum
turn_restriction(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    size_t result_count             = 0;
    General_path_element_t  *result_tuples   = NULL;

    // stuff done only on the first call of the function
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        PGR_DBG("Calling compute_trsp");
        compute_trsp(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_BOOL(4),
                &result_tuples, &result_count);

        //-----------------------------------------------

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif

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
    result_tuples = (General_path_element_t *) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        // do when there is more left to send
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;
        size_t call_cntr = funcctx->call_cntr;


        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[call_cntr].end_id);
        values[4] = Int64GetDatum(result_tuples[call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[call_cntr].agg_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        result = HeapTupleGetDatum(tuple);

        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

