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

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
#include "utils/lsyscache.h"
#include "utils/builtins.h"
#include "fmgr.h"

/*
  Uncomment when needed
*/

#define DEBUG
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./structs.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"
#include "./contractGraph_driver.h"

PGDLLEXPORT Datum
contractGraph(PG_FUNCTION_ARGS);


/********************************************************************/
/*                          MODIFY AS NEEDED                        */
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
    PGR_DBG("edges_sql %s",edges_sql);
    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
    size_t total_tuples = 0;
    // TODO decide if is a requirement (ERROR) or not
    if (num_cycles < 1) {
        //TODO if ERROR free edges_sql, and the arrays
        PGR_DBG("Required: at least one cycle\n");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    for (size_t i = 0; i < size_contraction_order; ++i) {
            if (is_valid_contraction(contraction_order[i]) != 1) {
                PGR_DBG("Error: Enter a valid Contraction Type\n");
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
#if 1
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
#endif
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
    size_t              call_cntr;
    size_t               max_calls;
    TupleDesc            tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    pgr_contracted_blob  *result_tuples = NULL;
    size_t result_count = 0;
    int64_t* contraction_order;
    int64_t* forbidden_vertices;
    size_t size_contraction_order;
    size_t size_forbidden_vertices;
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        // TODO fix the comment to current signature
        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /*
           edges_sql TEXT,
           level BIGINT,
           directed BOOLEAN DEFAULT true
         **********************************************************************/ 

        forbidden_vertices = (int64_t*)
            pgr_get_bigIntArray_allowEmpty(&size_forbidden_vertices , PG_GETARG_ARRAYTYPE_P(1));
        PGR_DBG("size_forbidden_vertices %ld",size_forbidden_vertices);

        contraction_order = (int64_t*)
            pgr_get_bigIntArray(&size_contraction_order, PG_GETARG_ARRAYTYPE_P(2));
        PGR_DBG("size_contraction_order %ld ", size_contraction_order);



#if 1
        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                forbidden_vertices,
                contraction_order,
                size_forbidden_vertices,
                size_contraction_order,
                PG_GETARG_INT32(3),
                PG_GETARG_BOOL(4),
                &result_tuples,
                &result_count);
#endif
        PGR_DBG("Cleaning arrays");
        free(contraction_order);
        free(forbidden_vertices);
        PGR_DBG("Returned %d tuples\n", (int)result_count);


        funcctx->max_calls = (uint32_t)result_count;
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
    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (pgr_contracted_blob*) funcctx->user_fctx;

    if (call_cntr < max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        Datum       *contracted_vertices_array;
        char        *nulls;


        ArrayType * arrayType; 
        int16 typlen; 
        bool typbyval; 
        char typalign;

        values =(Datum *)palloc(8 * sizeof(Datum));
        nulls = palloc(8 * sizeof(bool));

        size_t i;
        for (i = 0; i < 8; ++i) {
            nulls[i] = false;
        }

        int contracted_vertices_size = 
            (int)result_tuples[call_cntr].contracted_vertices_size;

        contracted_vertices_array = (Datum *)palloc(sizeof(Datum) * 
                (size_t)contracted_vertices_size);
        for (i = 0; i < contracted_vertices_size; ++i) {
            //PGR_DBG("Storing cv %d",result_tuples[call_cntr].contracted_vertices[i]);
            contracted_vertices_array[i] = 
                Int64GetDatum(result_tuples[call_cntr].contracted_vertices[i]);
        }

        get_typlenbyvalalign(INT4OID, &typlen, &typbyval, &typalign);
        PGR_DBG("typlen %d",typlen);
        PGR_DBG("typbyval %d",typbyval);
        PGR_DBG("typalign %c",typalign);

        arrayType =  construct_array(contracted_vertices_array,
                contracted_vertices_size,
                INT4OID,  typlen, typbyval, typalign);

        TupleDescInitEntry(tuple_desc, (AttrNumber) 7, "contracted_vertices", 
                INT4ARRAYOID, -1, 0); 

#if 0
        PGR_DBG("Storing id %d",result_tuples[call_cntr].id);
        PGR_DBG("Storing type %s",result_tuples[call_cntr].type);
        PGR_DBG("Storing source %d",result_tuples[call_cntr].source);
        PGR_DBG("Storing target %d",result_tuples[call_cntr].target);
        PGR_DBG("Storing cost %f",result_tuples[call_cntr].cost);
        PGR_DBG("Storing contracted_vertices_size %d",result_tuples[call_cntr].contracted_vertices_size);
#endif




        PGR_DBG("Storing complete\n");
        // postgres starts counting from 1
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[call_cntr].id);
        values[2] = CStringGetTextDatum(result_tuples[call_cntr].type);
        values[3] = Int64GetDatum(result_tuples[call_cntr].source);
        values[4] = Int64GetDatum(result_tuples[call_cntr].target);
        values[5] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[6] = PointerGetDatum(arrayType);
        values[7] = Int64GetDatum(result_tuples[call_cntr].contracted_vertices_size);



        /*********************************************************************/

        tuple = heap_formtuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        PGR_DBG("Freeing values");
#if 0
        if (result_tuples) {
            if (result_tuples->contracted_graph_name)
                free(result_tuples->contracted_graph_name);
            if (result_tuples->contracted_graph_blob)
                free(result_tuples->contracted_graph_blob);
            if (result_tuples->removedVertices)
                free(result_tuples->removedVertices);
            if (result_tuples->removedEdges)
                free(result_tuples->removedEdges);
            if (result_tuples->psuedoEdges)
                free(result_tuples->psuedoEdges);
            free(result_tuples);
        }

#endif
        // cleanup
        if (result_tuples) free(result_tuples);
        SRF_RETURN_DONE(funcctx);
    }
    PGR_DBG("End of Function");
}

