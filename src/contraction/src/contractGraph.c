/*PGR-GNU*****************************************************************
File: contractGraph.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Rohith Reddy
Mail:

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
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"

#ifndef INT8ARRAYOID
#define INT8ARRAYOID    1016
#endif

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_types/contracted_rt.h"
#include "c_common/edges_input.h"
#include "c_common/arrays_input.h"
#include "drivers/contraction/contractGraph_driver.h"

PGDLLEXPORT Datum contractGraph(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(contractGraph);


static
void
process(char* edges_sql,
        ArrayType* order,
        int num_cycles,
        ArrayType* forbidden,

        bool directed,
        contracted_rt **result_tuples,
        size_t *result_count) {
    /*
     * nothing to do
     */
    if (num_cycles < 1) return;

    pgr_SPI_connect();

    size_t size_forbidden_vertices = 0;
    int64_t* forbidden_vertices =
        pgr_get_bigIntArray_allowEmpty(
                &size_forbidden_vertices,
                forbidden);
    PGR_DBG("size_forbidden_vertices %ld", size_forbidden_vertices);

    size_t size_contraction_order = 0;
    int64_t* contraction_order =
        pgr_get_bigIntArray(
                &size_contraction_order,
                order);
    PGR_DBG("size_contraction_order %ld ", size_contraction_order);


    size_t total_edges = 0;
    pgr_edge_t* edges = NULL;
    pgr_get_edges(edges_sql, &edges, &total_edges);
    if (total_edges == 0) {
        if (forbidden_vertices) pfree(forbidden_vertices);
        if (contraction_order) pfree(contraction_order);
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char* err_msg = NULL;
    do_pgr_contractGraph(
            edges, total_edges,
            forbidden_vertices, size_forbidden_vertices,
            contraction_order, size_contraction_order,
            num_cycles,
            directed,
            result_tuples, result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing pgr_contraction()", start_t, clock());


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
    if (forbidden_vertices) pfree(forbidden_vertices);
    if (contraction_order) pfree(contraction_order);
    pgr_SPI_finish();
}

PGDLLEXPORT Datum
contractGraph(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    contracted_rt  *result_tuples = NULL;
    size_t result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        /**********************************************************************/
        /*
           edges_sql TEXT,
           contraction_order BIGINT[],
           max_cycles integer DEFAULT 1,
           forbidden_vertices BIGINT[] DEFAULT ARRAY[]::BIGINT[],
           directed BOOLEAN DEFAULT true,

         **********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_ARRAYTYPE_P(1),
                PG_GETARG_INT32(2),
                PG_GETARG_ARRAYTYPE_P(3),
                PG_GETARG_BOOL(4),
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
    result_tuples = (contracted_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        bool        *nulls;
        int16 typlen;
        size_t      call_cntr = funcctx->call_cntr;

        /**********************************************************************/
        size_t numb = 7;
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
        TupleDescInitEntry(tuple_desc, (AttrNumber) 4, "contracted_vertices",
                INT8ARRAYOID, -1, 0);

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = CStringGetTextDatum(result_tuples[call_cntr].type);
        values[2] = Int64GetDatum(result_tuples[call_cntr].id);
        values[3] = PointerGetDatum(arrayType);
        values[4] = Int64GetDatum(result_tuples[call_cntr].source);
        values[5] = Int64GetDatum(result_tuples[call_cntr].target);
        values[6] = Float8GetDatum(result_tuples[call_cntr].cost);

        /*********************************************************************/
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
