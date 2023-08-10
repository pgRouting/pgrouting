/*PGR-GNU*****************************************************************
File: edgeBetweennessCentrality_driver.cpp
Generated with Template by:

Copyright (c) 2023 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2023 Sanskar Bhushan
Mail: sbdtu5498@gmail.com
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

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/pgdata_getters.h"

#include "c_types/bc_rt.h"
#include "c_types/edge_t.h"

#include "drivers/metrics/edgeBetweennessCentrality_driver.h"

PGDLLEXPORT Datum _pgr_edgeBetweennessCentrality(PG_FUNCTION_ARGS);

static
void
process(
        char* edges_sql,

        bool directed,

        EBC_rt **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    (*result_tuples) = NULL;
    (*result_count) = 0;

    Edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_sql, &edges, &total_edges, true, false);

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;

    do_pgr_edgeBetweennessCentrality(
            edges, total_edges,

            directed,

            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing pgr_edgeBetweennessCentrality", start_t, clock());

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

    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(_pgr_edgeBetweennessCentrality);
PGDLLEXPORT Datum _pgr_edgeBetweennessCentrality(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    /**********************************************************************/
    EBC_rt *result_tuples = NULL;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        /***********************************************************************
         *
         *   pgr_edgeBetweennessCentrality(
         *       edges_sql TEXT,
         *       directed BOOLEAN DEFAULT true
         *   );
         *
         **********************************************************************/
        PGR_DBG("Calling process");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_BOOL(1),
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
    result_tuples = (EBC_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        /***********************************************************************
         *
         *   OUT seq BIGINT
         *   OUT id BIGINT
         *   OUT source BIGINT
         *   OUT target BIGINT
         *   OUT cost FLOAT
         *   OUT absolute_bc FLOAT
         *   OUT relative_bc FLOAT
         *
         **********************************************************************/

        values = palloc(7 * sizeof(Datum));
        nulls = palloc(7 * sizeof(bool));


        size_t i;
        for (i = 0; i < 7; ++i) {
            nulls[i] = false;
        }

        values[0] = Int64GetDatum(result_tuples[funcctx->call_cntr].seq);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].id);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].source);
        values[3] = Int64GetDatum(result_tuples[funcctx->call_cntr].target);
        values[4] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        values[5] = Float8GetDatum(result_tuples[funcctx->call_cntr].absolute_bc);
        values[6] = Float8GetDatum(result_tuples[funcctx->call_cntr].relative_bc);

        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
