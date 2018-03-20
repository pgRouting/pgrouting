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

#include "c_common/orders_input.h"

#include "c_types/column_info_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"


static
void fetch_pd_orders(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[14],
        bool matrix_version,
        PickDeliveryOrders_t *pd_order) {
    pd_order->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    pd_order->demand = pgr_SPI_getFloat8(tuple, tupdesc, info[1]);

    /*
     * the pickups
     */
    pd_order->pick_x = matrix_version ?
        0 : pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    pd_order->pick_y =  matrix_version ?
        0 : pgr_SPI_getFloat8(tuple, tupdesc, info[3]);
    pd_order->pick_open_t = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);
    pd_order->pick_close_t = pgr_SPI_getFloat8(tuple, tupdesc, info[5]);
    pd_order->pick_service_t = column_found(info[6].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[6]) : 0;

    /*
     * the deliveries
     */
    pd_order->deliver_x =  matrix_version ?
        0 : pgr_SPI_getFloat8(tuple, tupdesc, info[7]);
    pd_order->deliver_y =  matrix_version ?
        0 : pgr_SPI_getFloat8(tuple, tupdesc, info[8]);
    pd_order->deliver_open_t = pgr_SPI_getFloat8(tuple, tupdesc, info[9]);
    pd_order->deliver_close_t = pgr_SPI_getFloat8(tuple, tupdesc, info[10]);
    pd_order->deliver_service_t = column_found(info[11].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[11]) : 0;

    pd_order->pick_node_id = matrix_version ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[12]) : 0;
    pd_order->deliver_node_id = matrix_version ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[13]) : 0;
}



static
void
pgr_get_pd_orders_general(
        char *pd_orders_sql,
        PickDeliveryOrders_t **pd_orders,
        size_t *total_pd_orders,
        bool with_id) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    PGR_DBG("pgr_get_pd_orders_data");
    PGR_DBG("%s", pd_orders_sql);

    Column_info_t info[14];

    int i;
    for (i = 0; i < 14; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }

    info[0].name = "id";
    info[1].name = "demand";
    info[2].name = "p_x";
    info[3].name = "p_y";
    info[4].name = "p_open";
    info[5].name = "p_close";
    info[6].name = "p_service";
    info[7].name = "d_x";
    info[8].name = "d_y";
    info[9].name = "d_open";
    info[10].name = "d_close";
    info[11].name = "d_service";
    info[12].name = "p_node_id";
    info[13].name = "d_node_id";

    info[0].eType = ANY_INTEGER;
    info[12].eType = ANY_INTEGER;
    info[13].eType = ANY_INTEGER;

    /* service is optional*/
    info[6].strict = false;
    info[11].strict = false;
    /* nodes are going to be ignored*/
    info[12].strict = false;
    info[13].strict = false;

    if (with_id) {
        /* (x,y) values are ignored*/
        info[2].strict = false;
        info[3].strict = false;
        info[7].strict = false;
        info[8].strict = false;
        /* nodes are compulsory*/
        info[12].strict = true;
        info[13].strict = true;
    }



    size_t ntuples;
    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(pd_orders_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = true;
    (*total_pd_orders) = total_tuples = 0;

    /* on the first tuple get the column numbers */

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 14);
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
                fetch_pd_orders(&tuple, &tupdesc, info, with_id,
                        &(*pd_orders)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0) {
        (*total_pd_orders) = 0;
        PGR_DBG("NO orders");
        return;
    }

    (*total_pd_orders) = total_tuples;
    if (with_id) {
        PGR_DBG("Finish reading %ld orders for matrix", (*total_pd_orders));
    } else {
        PGR_DBG("Finish reading %ld orders for eucledian", (*total_pd_orders));
    }
    time_msg("reading edges", start_t, clock());
}

void
pgr_get_pd_orders(
        char *pd_orders_sql,
        PickDeliveryOrders_t **pd_orders,
        size_t *total_pd_orders) {
    pgr_get_pd_orders_general(pd_orders_sql, pd_orders, total_pd_orders, false);
}

void
pgr_get_pd_orders_with_id(
        char *pd_orders_sql,
        PickDeliveryOrders_t **pd_orders,
        size_t *total_pd_orders) {
    pgr_get_pd_orders_general(pd_orders_sql, pd_orders, total_pd_orders, true);
}
