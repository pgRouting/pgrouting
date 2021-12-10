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

#include <stdbool.h>
#include "c_common/postgres_connection.h"
#include "utils/array.h"


#include "drivers/trsp/trspVia_driver.h"

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

PGDLLEXPORT Datum _pgr_trspvia(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_trspvia);


static
void process(
        char* edges_sql,
        char* restrictions_sql,

        ArrayType *via_vertices,

        bool directed,

        Routes_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    size_t size_via_arr = 0;
    int64_t* via_arr = (int64_t*) pgr_get_bigIntArray(&size_via_arr, via_vertices);

    if (size_via_arr < 2) {
        if (via_arr) {pfree(via_arr); via_arr = NULL;};
        elog(WARNING, "Not enough via points");
        return;
    }

    Edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_sql, &edges, &total_edges);

    if (total_edges == 0) {
        if (via_arr) {pfree(via_arr); via_arr = NULL;};
        elog(WARNING, "No graph data found");
        pgr_SPI_finish();
        return;
    }

    Restriction_t * restrictions = NULL;
    size_t total_restrictions = 0;
    pgr_get_restrictions(restrictions_sql, &restrictions, &total_restrictions);


    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    do_trspVia(
            edges, total_edges,
            restrictions, total_restrictions,
            via_arr, size_via_arr,
            directed,
            true,  //strict
            true,  // U_turn_on_edge

            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg("processing pgr_trsp", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    pgr_SPI_finish();
}



PGDLLEXPORT Datum
_pgr_trspvia(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    size_t result_count      = 0;
    Routes_t  *result_tuples = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_ARRAYTYPE_P(2),
                PG_GETARG_BOOL(3),
                &result_tuples, &result_count);

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
    result_tuples = (Routes_t *) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;
        size_t call_cntr = funcctx->call_cntr;


        size_t numb = 10;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
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

        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
