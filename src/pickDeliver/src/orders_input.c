/*PGR-GNU*****************************************************************
File: restrictions_input.c

Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#include "postgres.h"
#include "executor/spi.h"

#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/get_check_data.h"
#include "./orders_input.h"


static
void fetch_order(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[9],
        Order_t *order) {


    order->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    order->x = pgr_SPI_getFloat8(tuple, tupdesc,  info[1]);
    order->y = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    order->demand = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);
    order->Etime = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);
    order->Ltime = pgr_SPI_getFloat8(tuple, tupdesc, info[5]);
    order->Stime = pgr_SPI_getFloat8(tuple, tupdesc, info[6]);
    order->Pindex = pgr_SPI_getBigInt(tuple, tupdesc, info[7]);
    order->Dindex = pgr_SPI_getBigInt(tuple, tupdesc, info[8]);
    order->Ddist = 0;
}



void
pgr_get_orders_data(
        char *orders_sql,
        Order_t **orders,
        size_t *total_orders) {
    const int tuple_limit = 1000000;

    PGR_DBG("pgr_get_orders_data");
    PGR_DBG("%s", orders_sql);

    Column_info_t info[9];

    int i;
    for (i = 0; i < 9; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }

    /*!
      int64_t id;
      double x;
      double y;
      double demand;
      double Etime;
      double Ltime;
      double Stime;
      int64_t Pindex;
      int64_t Dindex;
      double Ddist;
      */

    info[0].name = strdup("id");
    info[1].name = strdup("x");
    info[2].name = strdup("y");
    info[3].name = strdup("demand");
    info[4].name = strdup("etime");
    info[5].name = strdup("ltime");
    info[6].name = strdup("stime");
    info[7].name = strdup("pindex");
    info[8].name = strdup("dindex");

    info[0].eType = ANY_INTEGER;
    info[7].eType = ANY_INTEGER;
    info[8].eType = ANY_INTEGER;


    size_t ntuples;
    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(orders_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = TRUE;
    (*total_orders) = total_tuples = 0;

    /*  on the first tuple get the column numbers */

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 9);
        }
        ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("SPI_processed %ld", ntuples);
        if (ntuples > 0) {
            if ((*orders) == NULL)
                (*orders) = (Order_t *)palloc0(total_tuples * sizeof(Order_t));
            else
                (*orders) = (Order_t *)repalloc((*orders), total_tuples * sizeof(Order_t));

            if ((*orders) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_order(&tuple, &tupdesc, info,
                        &(*orders)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    if (total_tuples == 0) {
        (*total_orders) = 0;
        PGR_DBG("NO orders");
        return;
    }

    (*total_orders) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_orders));
}
