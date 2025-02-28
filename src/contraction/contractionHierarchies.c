/*PGR-GNU*****************************************************************
File: contractGraph.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) Aurélie Bousquet - 2024
Mail: aurelie.bousquet at oslandia.com

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
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"

#ifndef INT8ARRAYOID
#define INT8ARRAYOID    1016
#endif

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_types/contractionHierarchies_rt.h"
#include "drivers/contraction/contractionHierarchies_driver.h"

PGDLLEXPORT Datum _pgr_contractionhierarchies(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_contractionhierarchies);

static
void
process(char* edges_sql,
        ArrayType* forbidden,
        bool directed,
        contractionHierarchies_rt **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;

    clock_t start_t = clock();
    pgr_contractionHierarchies(
            edges_sql,
            forbidden,
            directed,
            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);
    time_msg("processing pgr_contractionHierarchies()", start_t, clock());


    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }
    pgr_global_report(&log_msg, &notice_msg, &err_msg);

    pgr_SPI_finish();
}

PGDLLEXPORT Datum
_pgr_contractionhierarchies(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    contractionHierarchies_rt  *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_ARRAYTYPE_P(1),
                PG_GETARG_BOOL(2),
                &result_tuples,
                &result_count);

        funcctx->max_calls = result_count;

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
    result_tuples = (contractionHierarchies_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        bool        *nulls;
        int16 typlen;
        size_t      call_cntr = funcctx->call_cntr;

        size_t numb = 8;
        values =(Datum *)palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));
        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        size_t contracted_vertices_size =
            (size_t)result_tuples[call_cntr].contracted_vertices_size;

        Datum* contracted_vertices_array;
        contracted_vertices_array = (Datum*) palloc(sizeof(Datum) *
                (size_t)contracted_vertices_size);

        for (i = 0; i < contracted_vertices_size; ++i) {
            PGR_DBG("Storing contracted vertex %ld",
                    result_tuples[call_cntr].contracted_vertices[i]);
            contracted_vertices_array[i] =
                Int64GetDatum(result_tuples[call_cntr].contracted_vertices[i]);
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
                contracted_vertices_array,
                (int)contracted_vertices_size,
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
        TupleDescInitEntry(tuple_desc, (AttrNumber) 3, "contracted_vertices",
                INT8ARRAYOID, -1, 0);

        values[0] = CStringGetTextDatum(result_tuples[call_cntr].type);
        values[1] = Int64GetDatum(result_tuples[call_cntr].id);
        values[2] = PointerGetDatum(arrayType);
        values[3] = Int64GetDatum(result_tuples[call_cntr].source);
        values[4] = Int64GetDatum(result_tuples[call_cntr].target);
        values[5] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[6] = Int64GetDatum(result_tuples[call_cntr].metric);
        values[7] = Int64GetDatum(result_tuples[call_cntr].vertex_order);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);

        /*
         *  cleaning up the contracted vertices array
         */
        if (result_tuples[funcctx->call_cntr].contracted_vertices) {
            pfree(result_tuples[funcctx->call_cntr].contracted_vertices);
        }
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
