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


#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/edges_input.h"
#include "c_common/restrictions_input.h"

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

        General_path_element_t **path,
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


    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;


    pgr_get_edges(edges_sql, &edges, &total_edges);

    Restriction_t *restrictions = NULL;
    size_t total_restrictions = 0;

    pgr_get_restrictions(restrictions_sql, &restrictions,
        &total_restrictions);

    if (total_edges == 0) {
        PGR_DBG("No edges found");
        pgr_SPI_finish();
        return;
    }

    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;
    do_pgr_turnRestrictedPath(
            edges,
            total_edges,
            restrictions,
            total_restrictions,
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
    pgr_global_report(log_msg, notice_msg, err_msg);

    if (edges) {pfree(edges); edges = NULL;}
    if (log_msg) {pfree(log_msg); log_msg = NULL;}
    if (notice_msg) {pfree(notice_msg); notice_msg = NULL;}
    if (err_msg) {pfree(err_msg); err_msg = NULL;}
    if (restrictions) {pfree(restrictions); edges = NULL;}

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
_pgr_turnrestrictedpath(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    General_path_element_t  *path = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*
           TEXT,   -- edges_sql
           TEXT,   -- restrictions_sql
           BIGINT, -- start_vertex
           BIGINT, -- end_vertex
           INTEGER,-- K cycles
           directed BOOLEAN DEFAULT true,
           heap_paths BOOLEAN DEFAULT false,
           stop_on_first BOOLEAN DEFAULT true,

           OUT seq INTEGER,
           OUT path_seq INTEGER,
           OUT node BIGINT,
           OUT edge BIGINT,
           OUT cost FLOAT,
           OUT agg_cost FLOAT)
         **********************************************************************/


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

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
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
    path = (General_path_element_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /*
               OUT seq INTEGER,
               OUT path_seq INTEGER,
               OUT node BIGINT,
               OUT edge BIGINT,
               OUT cost FLOAT,
               OUT agg_cost FLOAT
         ***********************************************************************/

        size_t v_count = 7;

        values = palloc(v_count * sizeof(Datum));
        nulls = palloc(v_count * sizeof(bool));


        size_t i;
        for (i = 0; i < v_count; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int32GetDatum(path[funcctx->call_cntr].start_id + 1);
        values[2] = Int32GetDatum(path[funcctx->call_cntr].seq);
        values[3] = Int64GetDatum(path[funcctx->call_cntr].node);
        values[4] = Int64GetDatum(path[funcctx->call_cntr].edge);
        values[5] = Float8GetDatum(path[funcctx->call_cntr].cost);
        values[6] = Float8GetDatum(path[funcctx->call_cntr].agg_cost);


        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
