/*PGR-GNU*****************************************************************
File: pickDeliver.c

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
#define DEBUG

#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./customers_input.h"

#include "./pickDeliver_driver.h"

PG_FUNCTION_INFO_V1(pickDeliver);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
pickDeliver(PG_FUNCTION_ARGS);


/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process(char* customers_sql,
        int max_vehicles,
        double capacity,
        int max_cycles,
        General_vehicle_orders_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    PGR_DBG("Load data");
    Customer_t *customers_arr = NULL;
    size_t total_customers = 0;
    pgr_get_customers_data(customers_sql, &customers_arr, &total_customers);

    if (total_customers == 0) {
        PGR_DBG("No customers found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld customers in query:", total_customers);

    PGR_DBG("Starting processing");
    char *err_msg = (char *)"";
    do_pgr_pickDeliver(
            customers_arr, total_customers,
            max_vehicles,
            capacity,
            max_cycles,
            result_tuples,
            result_count,
            &err_msg);
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("Returned message = %s\n", err_msg);

    free(err_msg);
    pfree(customers_arr);
    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
pickDeliver(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    size_t              call_cntr;
    size_t               max_calls;
    TupleDesc            tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    General_vehicle_orders_t  *result_tuples = 0;
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
           orders_sql TEXT,
           max_vehicles INTEGER,
           capacity FLOAT,
           max_cycles INTEGER, 
         **********************************************************************/

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                PG_GETARG_INT32(1),
                PG_GETARG_FLOAT8(2),
                PG_GETARG_INT32(3),
                &result_tuples,
                &result_count);

        /*                                                                             */
        /*******************************************************************************/

        funcctx->max_calls = (uint32)result_count;
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
    result_tuples = (General_vehicle_orders_t*) funcctx->user_fctx;

    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        char*        nulls;

        /*******************************************************************************/
        /*                          MODIFY!!!!!                                        */
        /*  This has to match you ouput otherwise the server crashes                   */
        /*
           OUT seq INTEGER,
           OUT vehicle_seq INTEGER,
           OUT vehicle_id INTEGER,
           OUT order_id BIGINT,
           OUT cost FLOAT,
           OUT agg_cost FLOAT
         ********************************************************************************/


        values = palloc(8 * sizeof(Datum));
        nulls = palloc(8 * sizeof(char));

        size_t i;
        for(i = 0; i < 8; ++i) {
            nulls[i] = ' ';
        }


        // postgres starts counting from 1
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].vehicle_seq);
        values[2] = Int32GetDatum(result_tuples[call_cntr].vehicle_id);
        values[3] = Int32GetDatum(result_tuples[call_cntr].order_id);
        values[4] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[5] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
        /*******************************************************************************/

        tuple = heap_formtuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

