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

#include "./../../common/src/postgres_connection.h"

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "funcapi.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
#include "utils/lsyscache.h"
#include "utils/builtins.h"

#ifndef INT8ARRAYOID
#define INT8ARRAYOID    1016
#endif
/*
  Uncomment when needed
*/

// #define DEBUG
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"
#include "./contractGraph_driver.h"

PGDLLEXPORT Datum contractGraph(PG_FUNCTION_ARGS);


/********************************************************************/
/*                          MODIFY AS NEEDED                        */

static
bool
is_valid_contraction(int64_t number) {
    switch (number) {
        case 1:
        case 2:
            return true;
            break;
        default:
            return false;
            break;
    }
}

static
void
process(char* edges_sql,
        int64_t *forbidden_vertices,
        int64_t *contraction_order,
        size_t size_forbidden_vertices,
        size_t size_contraction_order,
        int num_cycles,
        bool directed,
        pgr_contracted_blob **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();
    PGR_DBG("num_cycles %d ", num_cycles);
    PGR_DBG("directed %d ", directed);
    PGR_DBG("edges_sql %s", edges_sql);
    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
    size_t total_tuples = 0;

    // TODO(Rohith) decide if is a requirement (ERROR) or not
    if (num_cycles < 1) {
        // TODO(Rohith) if ERROR free edges_sql, and the arrays
        PGR_DBG("Required: at least one cycle\n");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    for (size_t i = 0; i < size_contraction_order; ++i) {
        if (!is_valid_contraction(contraction_order[i])) {
            PGR_DBG("Error: Invalid Contraction Type found\n");
            (*result_count) = 0;
            (*result_tuples) = NULL;
            pgr_SPI_finish();
            return;
        }
    }
    pgr_get_edges(edges_sql, &edges, &total_tuples);
    PGR_DBG("finished Loading");
    if (total_tuples == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld tuples in query:", total_tuples);
    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    do_pgr_contractGraph(
            edges,
            total_tuples,
            forbidden_vertices,
            size_forbidden_vertices,
            contraction_order,
            size_contraction_order,
            num_cycles,
            directed,
            result_tuples,
            result_count,
            &err_msg);
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("Returned message = %s\n", err_msg);
    free(err_msg);
    pfree(edges);
    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

PG_FUNCTION_INFO_V1(contractGraph);
PGDLLEXPORT Datum
contractGraph(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    pgr_contracted_blob  *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /*
           edges_sql TEXT,
           contraction_order BIGINT[],
           max_cycles integer DEFAULT 1,
           forbidden_vertices BIGINT[] DEFAULT ARRAY[]::BIGINT[],
           directed BOOLEAN DEFAULT true
         **********************************************************************/ 

        int64_t* contraction_order = NULL;
        int64_t* forbidden_vertices = NULL;
        size_t size_contraction_order = 0;
        size_t size_forbidden_vertices = 0;

        forbidden_vertices = (int64_t*)
            pgr_get_bigIntArray_allowEmpty(
                    &size_forbidden_vertices,
                    PG_GETARG_ARRAYTYPE_P(3));
        PGR_DBG("size_forbidden_vertices %ld", size_forbidden_vertices);

        contraction_order = (int64_t*)
            pgr_get_bigIntArray(
                    &size_contraction_order,
                    PG_GETARG_ARRAYTYPE_P(1));
        PGR_DBG("size_contraction_order %ld ", size_contraction_order);

        PGR_DBG("Calling process");
        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                forbidden_vertices,
                contraction_order,
                size_forbidden_vertices,
                size_contraction_order,
                PG_GETARG_INT32(2),
                PG_GETARG_BOOL(4),
                &result_tuples,
                &result_count);

        PGR_DBG("Cleaning arrays");
        if (contraction_order == NULL) PGR_DBG("empty contraction_order");
        if (forbidden_vertices == NULL) PGR_DBG("empty forbidden_vertices");

        pfree(contraction_order);
        if (forbidden_vertices) pfree(forbidden_vertices);
        PGR_DBG("Returned %ld tuples\n", result_count);
        /**********************************************************************/ 
#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (pgr_contracted_blob*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        Datum       *contracted_vertices_array;
        char        *nulls;
        ArrayType * arrayType;
        int16 typlen;
        bool typbyval;
        char typalign;

        values =(Datum *)palloc(7 * sizeof(Datum));
        nulls = palloc(7 * sizeof(bool));
        int i;
        for (i = 0; i < 7; ++i) {
            nulls[i] = false;
        }

        int contracted_vertices_size =
            (int)result_tuples[funcctx->call_cntr].contracted_vertices_size;

        contracted_vertices_array = (Datum *)palloc(sizeof(Datum) *
                (size_t)contracted_vertices_size);

        for (i = 0; i < contracted_vertices_size; ++i) {
            PGR_DBG("Storing contracted vertex %ld",
                    result_tuples[funcctx->call_cntr].contracted_vertices[i]);
            contracted_vertices_array[i] =
                Int64GetDatum(result_tuples[funcctx->call_cntr].contracted_vertices[i]);
        }

        get_typlenbyvalalign(INT8OID, &typlen, &typbyval, &typalign);
        arrayType =  construct_array(contracted_vertices_array,
                contracted_vertices_size,
                INT8OID,  typlen, typbyval, typalign);
        TupleDescInitEntry(tuple_desc, (AttrNumber) 4, "contracted_vertices",
                INT8ARRAYOID, -1, 0);

#if 1
        PGR_DBG("%ld | %s | %ld | %ld | %f | %d",
                result_tuples[funcctx->call_cntr].id,
                result_tuples[funcctx->call_cntr].type,
                result_tuples[funcctx->call_cntr].source,
                result_tuples[funcctx->call_cntr].target,
                result_tuples[funcctx->call_cntr].cost,
                result_tuples[funcctx->call_cntr].contracted_vertices_size);
#endif
        PGR_DBG("Storing complete\n");
        // postgres starts counting from 1
        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = CStringGetTextDatum(result_tuples[funcctx->call_cntr].type);
        values[2] = Int64GetDatum(result_tuples[funcctx->call_cntr].id);
        values[3] = PointerGetDatum(arrayType);
        values[4] = Int64GetDatum(result_tuples[funcctx->call_cntr].source);
        values[5] = Int64GetDatum(result_tuples[funcctx->call_cntr].target);
        values[6] = Float8GetDatum(result_tuples[funcctx->call_cntr].cost);
        /*********************************************************************/
        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        // cleaning up the contracted vertices array
        if (result_tuples[funcctx->call_cntr].contracted_vertices) {
            pfree(result_tuples[funcctx->call_cntr].contracted_vertices);
        }
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
