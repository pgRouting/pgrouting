/*PGR-GNU*****************************************************************
File: pd_orders_input.c

Copyright (c) 2016 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "./postgres_connection.h"

#include "./debug_macro.h"
#include "./pgr_types.h"
#include "./get_check_data.h"
#include "./orders_input.h"


static
void fetch_pd_orders(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[12],
        double default_pick_window_t,
        double default_deliver_window_t,
        double default_pick_service_t,
        double default_deliver_service_t,
        PickDeliveryOrders_t *pd_order) {
    pd_order->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    PGR_DBG("id=%ld",  pd_order->id);
    pd_order->demand = pgr_SPI_getFloat8(tuple, tupdesc, info[1]);
    pd_order->pick_x = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    pd_order->pick_y = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);
    pd_order->pick_open_t = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);

    if (column_found(info[5].colNumber)) {
        pd_order->pick_close_t = pgr_SPI_getFloat8(tuple, tupdesc, info[5]);
    } else {
        pd_order->pick_close_t = pd_order->pick_open_t + default_pick_window_t;
    }
    if (column_found(info[6].colNumber)) {
        pd_order->pick_service_t = pgr_SPI_getFloat8(tuple, tupdesc, info[6]);
    } else {
        pd_order->pick_service_t = default_pick_service_t;
    }

    pd_order->deliver_x = pgr_SPI_getFloat8(tuple, tupdesc, info[7]);
    pd_order->deliver_y = pgr_SPI_getFloat8(tuple, tupdesc, info[8]);
    pd_order->deliver_open_t = pgr_SPI_getFloat8(tuple, tupdesc, info[9]);
    if (column_found(info[10].colNumber)) {
        pd_order->deliver_close_t = pgr_SPI_getFloat8(tuple, tupdesc, info[10]);
    } else {
        pd_order->pick_close_t =
            pd_order->deliver_open_t + default_deliver_window_t;
    }
    if (column_found(info[11].colNumber)) {
        pd_order->deliver_service_t =
            pgr_SPI_getFloat8(tuple, tupdesc, info[11]);
    } else {
        pd_order->deliver_service_t = default_deliver_service_t;
    }
}



void
pgr_get_pd_orders(
        char *pd_orders_sql,
        double default_pick_window_t,
        double default_deliver_window_t,
        double default_pick_service_t,
        double default_deliver_service_t,
        PickDeliveryOrders_t **pd_orders,
        size_t *total_pd_orders) {
    const int tuple_limit = 1000000;

    PGR_DBG("pgr_get_pd_orders_data");
    PGR_DBG("%s", pd_orders_sql);

    Column_info_t info[12];

    int i;
    for (i = 0; i < 12; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }

    info[0].name = strdup("id");
    info[1].name = strdup("demand");
    info[2].name = strdup("pick_x");
    info[3].name = strdup("pick_y");
    info[4].name = strdup("pick_open");
    info[5].name = strdup("pick_close");
    info[6].name = strdup("pick_service");
    info[7].name = strdup("deliver_x");
    info[8].name = strdup("deliver_y");
    info[9].name = strdup("deliver_open");
    info[10].name = strdup("deliver_close");
    info[11].name = strdup("deliver_service");

    info[0].eType = ANY_INTEGER;

    info[5].strict = false;
    info[6].strict = false;
    info[10].strict = false;
    info[11].strict = false;


    size_t ntuples;
    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(pd_orders_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = TRUE;
    (*total_pd_orders) = total_tuples = 0;

    /* on the first tuple get the column numbers */

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 12);
        }
        ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("SPI_processed %ld", ntuples);
        if (ntuples > 0) {
            if ((*pd_orders) == NULL)
                (*pd_orders) = (PickDeliveryOrders_t *)palloc0(
                        total_tuples * sizeof(PickDeliveryOrders_t));
            else
                (*pd_orders) = (PickDeliveryOrders_t *)repalloc(
                        (*pd_orders),
                        total_tuples * sizeof(PickDeliveryOrders_t));

            if ((*pd_orders) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_pd_orders(&tuple, &tupdesc, info,
                        default_pick_window_t,
                        default_deliver_window_t,
                        default_pick_service_t,
                        default_deliver_service_t,
                        &(*pd_orders)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0) {
        (*total_pd_orders) = 0;
        PGR_DBG("NO orders");
        return;
    }

    (*total_pd_orders) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_pd_orders));
}
