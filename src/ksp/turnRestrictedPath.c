/*PGR-GNU*****************************************************************
File: turnRestrictedPath.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 vicky Vergara
Mail: vicky@georepublic.de

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

/** @file turnRestrictedPath.c */

#include <stdbool.h>
#include "c_common/postgres_connection.h"


#include "c_types/path_rt.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"


#include "drivers/yen/turnRestrictedPath_driver.h"

PGDLLEXPORT Datum _pgr_turnrestrictedpath(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_turnrestrictedpath);


static
void
process(
        char* edges_sql,
        char *restrictions_sql,
        int64_t start_vid,
        int64_t end_vid,

        int p_k,
        bool directed,
        bool heap_paths,
        bool stop_on_first,
        bool strict,

        Path_rt **path,
        size_t *result_count) {
    (*path) = NULL;
    (*result_count) = 0;

    if (p_k < 0) {
        return;
    }

    size_t k = (size_t)p_k;

    if (start_vid == end_vid) {
        PGR_DBG("Source and target are the same");
        return;
    }

    pgr_SPI_connect();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    clock_t start_t = clock();
    pgr_do_turnRestrictedPath(
            edges_sql,
            restrictions_sql,

            start_vid,
            end_vid,
            k,
            directed,
            heap_paths,
            stop_on_first,
            strict,

            path,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg(" processing pgr_turnRestrictedPath", start_t, clock());

    if (err_msg) {
        if (*path) pfree(*path);
    }
    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
_pgr_turnrestrictedpath(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    Path_rt  *path = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        PGR_DBG("Calling process");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_INT64(2),
                PG_GETARG_INT64(3),
                PG_GETARG_INT32(4),
                PG_GETARG_BOOL(5),
                PG_GETARG_BOOL(6),
                PG_GETARG_BOOL(7),
                PG_GETARG_BOOL(8),
                &path,
                &result_count);

        funcctx->max_calls = result_count;

        funcctx->user_fctx = path;
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
    path = (Path_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        size_t v_count = 7;

        values = palloc(v_count * sizeof(Datum));
        nulls = palloc(v_count * sizeof(bool));


        size_t i;
        for (i = 0; i < v_count; ++i) {
            nulls[i] = false;
        }

        int64_t seq = funcctx->call_cntr == 0?  1 : path[funcctx->call_cntr - 1].start_id;

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int32GetDatum(path[funcctx->call_cntr].start_id + 1);
        values[2] = Int32GetDatum(seq);
        values[3] = Int64GetDatum(path[funcctx->call_cntr].node);
        values[4] = Int64GetDatum(path[funcctx->call_cntr].edge);
        values[5] = Float8GetDatum(path[funcctx->call_cntr].cost);
        values[6] = Float8GetDatum(path[funcctx->call_cntr].agg_cost);

        path[funcctx->call_cntr].start_id = path[funcctx->call_cntr].edge < 0? 1 : seq + 1;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
