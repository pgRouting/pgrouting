/*PGR-GNU*****************************************************************
File: transitiveClosure.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Hang Wu
mail: nike0good@gmail.com

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
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"

#ifndef INT8ARRAYOID
#define INT8ARRAYOID    1016
#endif

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_types/transitiveClosure_rt.h"
#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"
#include "drivers/transitiveClosure/transitiveClosure_driver.h"

PGDLLEXPORT Datum _pgr_transitiveclosure(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_transitiveclosure);


static
void
process(char* edges_sql,

        transitiveClosure_rt **result_tuples,
        size_t *result_count) {
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
    do_pgr_transitiveClosure(
            edges, total_edges,

            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing pgr_transitiveClosure()", start_t, clock());


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
_pgr_transitiveclosure(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    transitiveClosure_rt  *result_tuples = NULL;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        /**********************************************************************/
        /*
           edges_sql TEXT,

         **********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                &result_tuples,
                &result_count);


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
    result_tuples = (transitiveClosure_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        bool        *nulls;
        int16 typlen;
        size_t      call_cntr = funcctx->call_cntr;

        /**********************************************************************/
        size_t numb = 3;
        values =(Datum *)palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));
        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        size_t target_array_size =
            (size_t)result_tuples[call_cntr].target_array_size;

        Datum* target_array_array;
        target_array_array = (Datum*) palloc(sizeof(Datum) *
                (size_t)target_array_size);

        for (i = 0; i < target_array_size; ++i) {
            PGR_DBG("Storing target_array vertex %ld",
                    result_tuples[call_cntr].target_array[i]);
            target_array_array[i] =
                Int64GetDatum(result_tuples[call_cntr].target_array[i]);
        }

        bool typbyval;
        char typalign;
        get_typlenbyvalalign(INT8OID, &typlen, &typbyval, &typalign);
        ArrayType* arrayType;
        /*
         * https://doxygen.postgresql.org/arrayfuncs_8c.html

         ArrayType* construct_array(
         Datum*     elems,
         int     nelems,
         Oid     elmtype, int elmlen, bool elmbyval, char elmalign
         )
         */
        arrayType =  construct_array(
                target_array_array,
                (int)target_array_size,
                INT8OID,  typlen, typbyval, typalign);
        /*
           void TupleDescInitEntry(
           TupleDesc   desc,
           AttrNumber      attributeNumber,
           const char *    attributeName,
           Oid     oidtypeid,
           int32   typmod,
           int     attdim
           )
           */
        TupleDescInitEntry(tuple_desc, (AttrNumber) 3, "target_array",
                INT8ARRAYOID, -1, 0);

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[call_cntr].vid);
        values[2] = PointerGetDatum(arrayType);

        /*********************************************************************/
        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);

        /*
         *  cleaning up the target_array array
         */
        if (result_tuples[funcctx->call_cntr].target_array) {
            pfree(result_tuples[funcctx->call_cntr].target_array);
        }
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
