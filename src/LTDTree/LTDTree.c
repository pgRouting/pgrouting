/*PGR-GNU*****************************************************************
File: LTDTree.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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


/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */

#include <stdbool.h>

#include "c_common/postgres_connection.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"

#ifndef INT8ARRAYOID
#define INT8ARRAYOID    1016
#endif

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_types/pgr_ltdtree_rt.h"
#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"
#include "drivers/LTDTree/LTDTree_driver.h"

PGDLLEXPORT Datum _pgr_ltdtree(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_ltdtree);


static
void
process(char* edges_sql,
        int64_t root_vertex,
        pgr_ltdtree_rt **result_tuples,
        size_t *result_count) {
    //result_count is a pointer it means we do not need to return
    // but it will count, initially it is 0
    pgr_SPI_connect();

    size_t total_edges = 0;
    pgr_edge_t* edges = NULL;
    pgr_get_edges(edges_sql, &edges, &total_edges);
    if (total_edges == 0) {
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    do_pgr_LTDTree(
            edges, total_edges,
            root_vertex,
            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing pgr_LTDTree()", start_t, clock());


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

PGDLLEXPORT Datum
_pgr_ltdtree(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/

    pgr_ltdtree_rt *result_tuples =NULL;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        /**********************************************************************/


        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)), //Converting sql to string
                PG_GETARG_INT64(1), //2nd parameter //BIGINT to int_64
                &result_tuples,
                &result_count);


        /**********************************************************************/
#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count; //result_count is updated in process function call
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
        funcctx->user_fctx = result_tuples; //
        if (get_call_result_type(fcinfo, NULL, &tuple_desc)
                != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        funcctx->tuple_desc = tuple_desc; //contains tuple description
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (pgr_ltdtree_rt*) funcctx->user_fctx; //converting structure

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple   tuple; //We will set all the values
        Datum       result;
        Datum       *values;
        bool        *nulls;
        int16 typlen;
        size_t call_cntr = funcctx->call_cntr;


        size_t numb = 3; //Number of columns in outputs
        values =(Datum *)palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));
        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }
            //Set your outputs from result_tuple
            values[0] = Int32GetDatum(call_cntr + 1); /*TODO Chek for the sequence*/
            values[1] = Int64GetDatum(result_tuples[call_cntr].vid);
	    values[2] = Int64GetDatum(result_tuples[call_cntr].idom);
            tuple = heap_form_tuple(tuple_desc, values, nulls);
            result = HeapTupleGetDatum(tuple);
            SRF_RETURN_NEXT(funcctx, result);
        }else {   /* do when there is no more left */
            SRF_RETURN_DONE(funcctx);
        }

}
