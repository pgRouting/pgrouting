/*PGR-GNU*****************************************************************
File: testXYedges.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
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

/*
  Uncomment when needed
*/
// #define DEBUG

#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/edges_input.h"
#include "./../../common/src/arrays_input.h"

#include "./testXYedges_driver.h"

PG_FUNCTION_INFO_V1(testXYedges);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
testXYedges(PG_FUNCTION_ARGS);


/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process( char* edges_sql,
        bool *result_bool) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    Pgr_edge_xy_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges_xy(edges_sql, &edges, &total_edges);

    if (total_edges == 0) {
        PGR_DBG("No edges found");
        (*result_bool) = true;
        pfree(edges);
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld tuples in query:", total_edges);
    size_t i;
    for (i = 0; i < total_edges; ++i) {
        PGR_DBG("id = %li \t source = %li \t target = %ld  cost = %lf reverse_cost = %lf",
               edges[i].id, edges[i].source, edges[i].target, edges[i].cost, edges[i].reverse_cost);
        PGR_DBG("    (x1,y1) = (%.32lf ,%.32lf) (x2,y2) = (%.32lf,.%.32lf)",
               edges[i].x1, edges[i].y1, edges[i].x2, edges[i].y2);
    }


    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    char *log_msg = NULL;
    (*result_bool) = do_pgr_testXYedges(
            edges,
            total_edges,
            &log_msg,
            &err_msg);

    pfree(edges);
    PGR_DBG("Returned log message = %s\n", log_msg);
    if (log_msg) {
        elog(DEBUG1, "%s", log_msg);
        free(log_msg);
    }
    PGR_DBG("Returned error message = %s\n", err_msg);

    if (err_msg) {
        elog(ERROR, "%s", err_msg);
        free(err_msg);
    }

    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
testXYedges(PG_FUNCTION_ARGS) {
#if 0
    FuncCallContext     *funcctx;
    uint32_t            call_cntr;
    uint32_t            max_calls;
    TupleDesc           tuple_desc;
#endif
    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    bool  result_bool = NULL;
    /*                                                                        */
    /**************************************************************************/

#if 0
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
#endif

        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /*
           edges_sql TEXT
         **********************************************************************/


        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                &result_bool);

        PG_RETURN_BOOL(result_bool);
        /*                                                                             */
        /*******************************************************************************/

#if 0
        funcctx->max_calls = 1;
        funcctx->user_fctx = result_bool;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }
#endif

#if 0
    funcctx = SRF_PERCALL_SETUP();
    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    result_bool = (bool*) funcctx->user_fctx;
#endif
#if 0
    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        char*        nulls;

        /*******************************************************************************/
        /*                          MODIFY!!!!!                                        */
        /*  This has to match you ouput otherwise the server crashes                   */
        /*
           OUT status BOOLEAN
         ********************************************************************************/


        values = palloc(1 * sizeof(Datum));
        nulls = palloc(1 * sizeof(char));

        size_t i;
        for(i = 0; i < 1; ++i) {
            nulls[i] = ' ';
        }


        // postgres starts counting from 1
        values[0] = Int32GetDatum(*result_bool);
        /*******************************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
#endif
}

