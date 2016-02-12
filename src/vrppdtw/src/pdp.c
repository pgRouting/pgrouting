/*PGR-GNU*****************************************************************

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

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
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"

#define DEBUG 
#include "../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "../../common/src/postgres_connection.h"
#include "./customers_input.h"
#include "./pdp_solver.h"


PG_FUNCTION_INFO_V1(vrppdtw);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
vrppdtw(PG_FUNCTION_ARGS);



static
int compute_shortest_path(
        char* sql,
        int64_t vehicle_count,
        double capacity,
        path_element **results,
        size_t *length_results_struct) {
    PGR_DBG("start shortest_path\n");

    pgr_SPI_connect();
    char *err_msg = NULL;
    size_t total_customers = 0;
    Customer *customers = NULL;
    PGR_DBG("Calling pgr_get_customers\n");
    pgr_get_customers(sql, &customers, &total_customers);
#if 1
    PGR_DBG("DATA returned\n");
    size_t i;
    for (i = 0; i < total_customers ; ++i) {
        PGR_DBG("%zu: %lld\t %f\t%f\t%f\t %f\t%f\t%f\t %lld\t%lld\t  %f", i,
                customers[i].id,
                customers[i].x,
                customers[i].y,
                customers[i].demand,
                customers[i].Etime,
                customers[i].Ltime,
                customers[i].Stime,
                customers[i].Pindex,
                customers[i].Dindex,
                customers[i].Ddist
               );
    }
#endif
#if 1
    PGR_DBG("Not Calling Solver Instance\n");
    pfree(customers);
    pgr_SPI_finish();
    return 0;
#endif
    PGR_DBG("Calling Solver Instance\n");
    int ret = Solver(
            customers, 
            total_customers, 
            vehicle_count,
            capacity, 
            &err_msg, 
            results, 
            length_results_struct);
#if 1
    // if (err_msg) free(err_msg);
    // pfree(customers);
    pgr_SPI_finish();
    return 0;
#endif

    if (err_msg) PGR_DBG("%s\n",err_msg);
    if (err_msg) free(err_msg);
    pfree(customers);
    pgr_SPI_finish();
    return 0;

#if 0
    if (ret < 0) {
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
                    errmsg("Error computing path: %s", err_msg)));
    }
#endif

    PGR_DBG("*length_results_count  = %zu\n", *length_results_struct);
    PGR_DBG("ret = %lld\n", ret);

    pfree(customers);
    if (err_msg) free(err_msg);
    pgr_SPI_finish();
    return 0;
}



#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
vrppdtw(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t               call_cntr;
    uint32_t               max_calls;
    TupleDesc            tuple_desc;

    path_element     *result_tuples = NULL;
    size_t result_count = 0;


    /* stuff done only on the first call of the function */

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        PGR_DBG("Calling compute_shortes_path");

        compute_shortest_path(
                pgr_text2char(PG_GETARG_TEXT_P(0)),  // customers sql
                PG_GETARG_INT64(1),  // vehicles  count
                PG_GETARG_FLOAT8(2),  // capacity 
                &result_tuples,
                &result_count);

        PGR_DBG("Back from solve_vrp, length_results: %zu", result_count);

        /* total number of tuples to be returned */
        funcctx->max_calls = (uint32_t)result_count;
        funcctx->user_fctx = result_tuples;

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (path_element *) funcctx->user_fctx;

    /* do when there is more left to send */
    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum       *values;
        char        *nulls;

        PGR_DBG("Till hereee ");
        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(char));

        nulls[0] = ' ';
        nulls[1] = ' ';
        nulls[2] = ' ';
        nulls[3] = ' ';
        values[0] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[1] = Int64GetDatum(result_tuples[call_cntr].rid);
        values[2] = Int64GetDatum(result_tuples[call_cntr].nid);
        values[3] = Float8GetDatum(result_tuples[call_cntr].cost);

        tuple = heap_formtuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        /* do when there is no more left */
        if (result_tuples) free(result_tuples);
        SRF_RETURN_DONE(funcctx);
    }
}
