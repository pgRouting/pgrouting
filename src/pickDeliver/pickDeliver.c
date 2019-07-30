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

#include "c_common/postgres_connection.h"
#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/orders_input.h"
#include "c_common/vehicles_input.h"
#include "c_common/matrixRows_input.h"
#include "drivers/pickDeliver/pickDeliver_driver.h"

PGDLLEXPORT Datum
_pgr_pickdeliver(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_pickdeliver);


static
void
process(
        char* pd_orders_sql,
        char* vehicles_sql,
        char* matrix_sql,
        double factor,
        int max_cycles,
        int initial_solution_id,

        General_vehicle_orders_t **result_tuples,
        size_t *result_count) {
    if (factor <= 0) {
        ereport(ERROR,
                (errcode(ERRCODE_INTERNAL_ERROR),
                 errmsg("Illegal value in parameter: factor"),
                 errhint("Value found: %f <= 0", factor)));
        (*result_count) = 0;
        (*result_tuples) = NULL;
        return;
    }

    if (max_cycles < 0) {
        elog(ERROR, "Illegal value in parameter: max_cycles");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        return;
    }

    if (initial_solution_id < 0 || initial_solution_id > 7) {
        elog(ERROR, "Illegal value in parameter: initial");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        return;
    }

    pgr_SPI_connect();

    PGR_DBG("Load orders");
    PickDeliveryOrders_t *pd_orders_arr = NULL;
    size_t total_pd_orders = 0;
    pgr_get_pd_orders_with_id(pd_orders_sql,
           &pd_orders_arr, &total_pd_orders);

    PGR_DBG("Load vehicles");
    Vehicle_t *vehicles_arr = NULL;
    size_t total_vehicles = 0;
    pgr_get_vehicles_with_id(vehicles_sql,
           &vehicles_arr, &total_vehicles);
    PGR_DBG("total vehicles %ld", total_vehicles);


#if 0
    for (size_t i = 0; i < total_pd_orders; i++) {
        PGR_DBG("%ld %f pick %f %f %ld - "
                "%f %f %f deliver %f %f %ld - %f %f %f ",
                pd_orders_arr[i].id,
                pd_orders_arr[i].demand,

                pd_orders_arr[i].pick_x,
                pd_orders_arr[i].pick_y,
                pd_orders_arr[i].pick_node_id,

                pd_orders_arr[i].pick_open_t,
                pd_orders_arr[i].pick_close_t,
                pd_orders_arr[i].pick_service_t,

                pd_orders_arr[i].deliver_x,
                pd_orders_arr[i].deliver_y,
                pd_orders_arr[i].deliver_node_id,

                pd_orders_arr[i].deliver_open_t,
                pd_orders_arr[i].deliver_close_t,
                pd_orders_arr[i].deliver_service_t);
    }

    for (size_t i = 0; i < total_vehicles; i++) {
        PGR_DBG("%ld %f %f / %ld %f %f %f %f %f / %ld %f %f %f %f %f / %ld ",
               vehicles_arr[i].id,
               vehicles_arr[i].capacity,
               vehicles_arr[i].speed,

               vehicles_arr[i].start_node_id,
               vehicles_arr[i].start_x,
               vehicles_arr[i].start_y,
               vehicles_arr[i].start_open_t,
               vehicles_arr[i].start_close_t,
               vehicles_arr[i].start_service_t,

               vehicles_arr[i].end_node_id,
               vehicles_arr[i].end_x,
               vehicles_arr[i].end_y,
               vehicles_arr[i].end_open_t,
               vehicles_arr[i].end_close_t,
               vehicles_arr[i].end_service_t,

               vehicles_arr[i].cant_v);
    }
#endif
    PGR_DBG("load matrix");
    Matrix_cell_t *matrix_cells_arr = NULL;
    size_t total_cells = 0;
    pgr_get_matrixRows(matrix_sql, &matrix_cells_arr, &total_cells);


    if (total_pd_orders == 0 || total_vehicles == 0 || total_cells == 0) {
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld orders in query:", total_pd_orders);
    PGR_DBG("Total %ld vehicles in query:", total_vehicles);
    PGR_DBG("Total %ld matrix cells in query:", total_cells);


    PGR_DBG("Starting processing");
            clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;

    do_pgr_pickDeliver(
            pd_orders_arr, total_pd_orders,
            vehicles_arr, total_vehicles,
            matrix_cells_arr, total_cells,

            factor,
            max_cycles,
            initial_solution_id,

            result_tuples,
            result_count,

            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("pgr_pickDeliver", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_count) = 0;
        (*result_tuples) = NULL;
    }
#if 1
    pgr_global_report(log_msg, notice_msg, err_msg);
#else
    pgr_global_report(notice_msg, log_msg, err_msg);
#endif

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (pd_orders_arr) pfree(pd_orders_arr);
    if (vehicles_arr) pfree(vehicles_arr);
    if (matrix_cells_arr) pfree(matrix_cells_arr);

    pgr_SPI_finish();
}



/******************************************************************************/


PGDLLEXPORT Datum
_pgr_pickdeliver(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    /**************************************************************************/
    General_vehicle_orders_t *result_tuples = 0;
    size_t result_count = 0;
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        /**********************************************************************
           orders_sql TEXT,
           vehicles_sql TEXT,
           matrix_cell_sql TEXT,
           factor FLOAT DEFAULT 1,
           max_cycles  INTEGER DEFAULT 10,
           initial_sol INTEGER DEFAULT 4,
         **********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                PG_GETARG_FLOAT8(3),
                PG_GETARG_INT32(4),
                PG_GETARG_INT32(5),
                &result_tuples,
                &result_count);

        /*********************************************************************/

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc)
                != TYPEFUNC_COMPOSITE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        }

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (General_vehicle_orders_t*) funcctx->user_fctx;

    if (funcctx->call_cntr <  funcctx->max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        bool*       nulls;
        size_t      call_cntr = funcctx->call_cntr;

        /*********************************************************************

          OUT seq INTEGER,
          OUT vehicle_number INTEGER,
          OUT vehicle_id BIGINT,
          OUT vehicle_seq INTEGER,
          OUT order_id BIGINT,
          OUT stop_type INT,
          OUT cargo FLOAT,
          OUT travel_time FLOAT,
          OUT arrival_time FLOAT,
          OUT wait_time FLOAT,
          OUT service_time FLOAT,
          OUT departure_time FLOAT

         *********************************************************************/


        size_t numb = 13;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }


        values[0] = Int32GetDatum(funcctx->call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].vehicle_seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].vehicle_id);
        values[3] = Int32GetDatum(result_tuples[call_cntr].stop_seq);
        values[4] = Int32GetDatum(result_tuples[call_cntr].stop_type + 1);
        values[5] = Int64GetDatum(result_tuples[call_cntr].stop_id);
        values[6] = Int64GetDatum(result_tuples[call_cntr].order_id);
        values[7] = Float8GetDatum(result_tuples[call_cntr].cargo);
        values[8] = Float8GetDatum(result_tuples[call_cntr].travelTime);
        values[9] = Float8GetDatum(result_tuples[call_cntr].arrivalTime);
        values[10] = Float8GetDatum(result_tuples[call_cntr].waitTime);
        values[11] = Float8GetDatum(result_tuples[call_cntr].serviceTime);
        values[12] = Float8GetDatum(result_tuples[call_cntr].departureTime);

        /*********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
