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
#include "./pdp.h"


Datum vrppdtw(PG_FUNCTION_ARGS);


// #define DEBUG 1
#include "../../common/src/debug_macro.h"
#include "../../common/src/postgres_connection.h"


// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000


static
int
fetch_customer_columns(SPITupleTable *tuptable, Customer *c , int vehicle_count , int capacity) {
    PGR_DBG("Customer Data", NULL);

    c->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
    PGR_DBG(" id done ", NULL);
    c->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
    PGR_DBG("x done", NULL);
    c->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");
    PGR_DBG("y done", NULL);
    c->demand = SPI_fnumber(SPI_tuptable->tupdesc, "demand");
    PGR_DBG("demand done", NULL);
    c->Etime = SPI_fnumber(SPI_tuptable->tupdesc, "etime");
    PGR_DBG("etime done", NULL);
    c->Ltime = SPI_fnumber(SPI_tuptable->tupdesc, "ltime");
    PGR_DBG("ltime done", NULL);
    c->Stime = SPI_fnumber(SPI_tuptable->tupdesc, "stime");
    PGR_DBG("stime done", NULL);
    c->Pindex = SPI_fnumber(SPI_tuptable->tupdesc, "pindex");
    PGR_DBG("pindex done", NULL);
    c->Dindex = SPI_fnumber(SPI_tuptable->tupdesc, "dindex");
    PGR_DBG("dindex done", NULL);
    if (c->id == SPI_ERROR_NOATTRIBUTE ||
            c->x == SPI_ERROR_NOATTRIBUTE ||
            c->y == SPI_ERROR_NOATTRIBUTE ||
            c->demand == SPI_ERROR_NOATTRIBUTE ||
            c->Ltime == SPI_ERROR_NOATTRIBUTE ||
            c->Stime == SPI_ERROR_NOATTRIBUTE ||
            c->Pindex == SPI_ERROR_NOATTRIBUTE ||
            c->Dindex == SPI_ERROR_NOATTRIBUTE ||
            c->Etime == SPI_ERROR_NOATTRIBUTE) {
        elog(ERROR, "Error, query must return columns "
                "'id', 'x','y','demand', 'Etime',  'Ltime', 'Stime', 'Pindex',  and 'Dindex'");
        return -1;
    }

    PGR_DBG("Returned from here  ");
    return 0;
}


static
void fetch_customer(HeapTuple *tuple, TupleDesc *tupdesc, Customer *c_all, Customer *c_single) {
    Datum binval;
    bool isnull;
    PGR_DBG("Hey baby in fetch_customer", NULL);

    binval = SPI_getbinval(*tuple, *tupdesc, c_all->id, &isnull);
    PGR_DBG("fetching first thing");
    if (isnull) elog(ERROR, "id contains a null value");
    c_single->id = DatumGetInt32(binval);
    PGR_DBG("id =  %d", c_single->id);


    PGR_DBG("fetching second  thing", NULL);
    binval = SPI_getbinval(*tuple, *tupdesc, c_all->x, &isnull);
    if (isnull)
        elog(ERROR, "x contains a null value");
    c_single->x = DatumGetInt32(binval);

    binval = SPI_getbinval(*tuple, *tupdesc, c_all->y, &isnull);
    if (isnull) elog(ERROR, "y contains a null value");
    c_single->y = DatumGetInt32(binval);


    binval = SPI_getbinval(*tuple, *tupdesc, c_all->demand, &isnull);
    if (isnull) elog(ERROR, "demand contains a null value");
    c_single->demand = DatumGetInt32(binval);


    binval = SPI_getbinval(*tuple, *tupdesc, c_all->Etime, &isnull);
    if (isnull) elog(ERROR, "Etime contains a null value");
    c_single->Etime = DatumGetInt32(binval);


    binval = SPI_getbinval(*tuple, *tupdesc, c_all->Ltime, &isnull);
    if (isnull) elog(ERROR, "Ltime contains a null value");
    c_single->Ltime = DatumGetInt32(binval);


    binval = SPI_getbinval(*tuple, *tupdesc, c_all->Stime, &isnull);
    if (isnull) elog(ERROR, "Stime contains a null value");
    c_single->Stime = DatumGetInt32(binval);


    binval = SPI_getbinval(*tuple, *tupdesc, c_all->Pindex, &isnull);
    if (isnull) elog(ERROR, "pindex contains a null value");
    c_single->Pindex = DatumGetInt32(binval);

    binval = SPI_getbinval(*tuple, *tupdesc, c_all->Dindex, &isnull);
    if (isnull) elog(ERROR, "dindex contains a null value");
    c_single->Dindex = DatumGetInt32(binval);

    return;
}




// Note:: edge_colums = total , //ly customer_t = total ....

static int compute_shortest_path(
        char* sql,
        int  vehicle_count,
        int capacity ,
        path_element **results,
        int *length_results_struct) {
    void *SPIplan;
    Portal SPIportal;
    bool moredata = TRUE;
    int ntuples;
    Customer *customer_single = NULL;
    int total_tuples = 0;
    Customer customer_all = {-1, -1, -1, -1 , -1, -1, -1 , -1 , -1, -1, -1, 0};

    char *err_msg;
    int ret = -1;
    // register int z;

    PGR_DBG("start shortest_path\n");

    pgr_SPI_connect();
    SPIplan = pgr_SPI_prepare(sql);
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

        PGR_DBG("Checking ");

        if (customer_all.id == -1) {
            if (fetch_customer_columns(SPI_tuptable, &customer_all,
                        vehicle_count, capacity) == -1) {
                pgr_SPI_finish();
                return -1;
            }
        }

        ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("Calculated total_tuples  ntuples=%d   total_tuples =%d ", ntuples, total_tuples);

        if (customer_single == NULL)
            customer_single = palloc(total_tuples * sizeof(Customer));
        else
            customer_single = repalloc(customer_single, total_tuples * sizeof(Customer));

        if (customer_single == NULL) {
            elog(ERROR, "Out of memory");
            pgr_SPI_finish();
            return -1;
        }


        if (ntuples > 0) {
            int t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_customer(&tuple, &tupdesc,
                        &customer_all,
                        &customer_single[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }


    PGR_DBG("Calling Solver Instance\n");

    ret = Solver(customer_single, total_tuples, vehicle_count,
            capacity, &err_msg, results, length_results_struct);

    if (ret < -2) {
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
                    errmsg("Error computing path: %s", err_msg)));
    }


    PGR_DBG("*length_results_count  = %i\n", *length_results_struct);
    PGR_DBG("ret = %i\n", ret);
#if def DEBUG
    int vb;
    for (vb=1; vb < *length_results_struct; vb++) {
        PGR_DBG("results[%d].seq=%d  ", vb, (*results)[vb].seq);
        PGR_DBG("results[%d].rid=%d  ", vb, (*results)[vb].rid);
        PGR_DBG("results[%d].nid=%d \n", vb, (*results)[vb].nid);
    }
#endif

    pfree(customer_single);
    free(err_msg);
    pgr_SPI_finish();
    return 0;
}



PG_FUNCTION_INFO_V1(vrppdtw);
Datum
vrppdtw(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    int                  call_cntr;
    int                  max_calls;
    TupleDesc            tuple_desc;
    path_element     *results = 0;


    /* stuff done only on the first call of the function */

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        int length_results_struct = 0;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        results = (path_element *)palloc(sizeof(path_element)*((length_results_struct) + 1));

        PGR_DBG("Calling compute_shortes_path");

        compute_shortest_path(
                pgr_text2char(PG_GETARG_TEXT_P(0)),  // customers sql
                PG_GETARG_INT32(1),  // vehicles  count
                PG_GETARG_INT32(2),  // capacity count
                &results, &length_results_struct);

        PGR_DBG("Back from solve_vrp, length_results: %d", length_results_struct);

        /* total number of tuples to be returned */
        funcctx->max_calls = length_results_struct;
        funcctx->user_fctx = results;

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = BlessTupleDesc(tuple_desc);
        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    results = (path_element *) funcctx->user_fctx;

    /* do when there is more left to send */
    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        char* nulls;

        PGR_DBG("Till hereee ", NULL);
        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(char));

        nulls[0] = ' ';
        nulls[1] = ' ';
        nulls[2] = ' ';
        nulls[3] = ' ';
        values[0] = Int32GetDatum(results[call_cntr].seq);
        values[1] = Int32GetDatum(results[call_cntr].rid);
        values[2] = Int32GetDatum(results[call_cntr].nid);
        values[3] = Int32GetDatum(results[call_cntr].cost);
        tuple = heap_formtuple(tuple_desc, values, nulls);

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        /* clean up (this is not really necessary) */
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        /* do when there is no more left */
        free(results);
        SRF_RETURN_DONE(funcctx);
    }
}
