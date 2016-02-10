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
// TODO(rohith) Check style.
// #include "utils/lsyscache.h"
 #include "utils/builtins.h"

/*
  Uncomment when needed
*/


#include "fmgr.h"
   #define DEBUG 1
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/edges_input.h"
#include "./../../contraction/src/structs.h"
#include "./contractGraph_driver.h"
// #include "contract_function.h"
// #include "./connection.h"

PG_FUNCTION_INFO_V1(contractGraph);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
contractGraph(PG_FUNCTION_ARGS);


/********************************************************************/
/*                          MODIFY AS NEEDED                        */
static
void
process(char* edges_sql,
        int64_t level,
        bool directed,
        pgr_contracted_blob **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    pgr_edge_t *edges = NULL;
    int64_t total_tuples = 0;
    pgr_get_data_5_columns(edges_sql, &edges, &total_tuples);

    if (total_tuples == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld tuples in query:", total_tuples);

    PGR_DBG("Starting processing");
     PGR_DBG("Graphh is %d",directed);
    char *err_msg = NULL;
    do_pgr_contractGraph(
            edges,
            total_tuples,
            level,
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

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
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
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /*
           edges_sql TEXT,
    level BIGINT,
    directed BOOLEAN DEFAULT true
         **********************************************************************/ 

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                PG_GETARG_INT64(1),
                PG_GETARG_BOOL(2),
                &result_tuples,
                &result_count);

        /*                                                                             */
        /*******************************************************************************/

        funcctx->max_calls = result_count;
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
        char        *nulls;

        /******************************************************************/
        /*                          MODIFY!!!!!                           */
        /*  This has to match you ouput otherwise the server crashes      */
        /*
           OUT contracted_graph_name TEXT,
    	   OUT contracted_graph_blob TEXT,
    	   OUT removedVertices TEXT,
    	   OUT removedEdges TEXT,
           OUT psuedoEdges TEXT
        *******************************************************************/


        values =(Datum *)palloc(5 * sizeof(Datum));
        // values = (char **) palloc(5 * sizeof(char *));
        nulls = palloc(5 * sizeof(char));

        size_t i;
        for (i = 0; i < 5; ++i) {
            nulls[i] = ' ';
        }

        PGR_DBG("Storing graphname %s",result_tuples->contracted_graph_name);
        PGR_DBG("Storing blob %s",result_tuples->contracted_graph_blob);
        PGR_DBG("Storing rv %s",result_tuples->removedVertices);
        PGR_DBG("Storing re %s",result_tuples->removedEdges);
        PGR_DBG("Storing pe %s",result_tuples->psuedoEdges);
        // postgres starts counting from 1
        values[0] = CStringGetTextDatum(result_tuples->contracted_graph_name);
        values[1] = CStringGetTextDatum(result_tuples->contracted_graph_blob);
        values[2] = CStringGetTextDatum(result_tuples->removedVertices);
        values[3] = CStringGetTextDatum(result_tuples->removedEdges);
        values[4] = CStringGetTextDatum(result_tuples->psuedoEdges);
        
        /*********************************************************************/

        tuple = heap_formtuple(tuple_desc, values, nulls);
         PGR_DBG("heap_formtuple OK");
        result = HeapTupleGetDatum(tuple);
         PGR_DBG("HeapTupleGetDatum OK");
        SRF_RETURN_NEXT(funcctx, result);
        PGR_DBG("Returning values");
    } else {
        // cleanup
         PGR_DBG("Freeing values");
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

        SRF_RETURN_DONE(funcctx);
    }
    PGR_DBG("End of Function");
}

