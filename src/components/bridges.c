/*PGR-GNU*****************************************************************
File: bridges.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Maoguang Wang
Mail: xjtumg1007@gmail.com


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

/** @file bridges.c */

#include <stdbool.h>
#include "c_common/postgres_connection.h"


#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/edges_input.h"

#include "drivers/components/bridges_driver.h"

PGDLLEXPORT Datum _pgr_bridges(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_bridges);


static
void
process(
        char* edges_sql,
        int64_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    (*result_tuples) = NULL;
    (*result_count) = 0;

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_sql, &edges, &total_edges);

    if (total_edges == 0) {
        pgr_SPI_finish();
        return;
    }

    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    do_pgr_bridges(
            edges,
            total_edges,

            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg(" processing pgr_bridges", start_t, clock());

    if (err_msg) {
        if (*result_tuples) pfree(*result_tuples);
    }
    pgr_global_report(log_msg, notice_msg, err_msg);

    if (edges) pfree(edges);
    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum _pgr_bridges(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    int64_t *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                &result_tuples,
                &result_count);


#if PGSQL_VERSION > 94
        funcctx->max_calls = (uint32_t)result_count;
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
    result_tuples = (int64_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;


        values = palloc(2 * sizeof(Datum));
        nulls = palloc(2 * sizeof(bool));


        size_t i;
        for (i = 0; i < 2; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr]);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}

