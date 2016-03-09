/*PGR-GNU*****************************************************************
File: customers_input.c

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

// #define DEBUG
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/get_check_data.h"
#include "./customers_input.h"




static
void pgr_fetch_customer(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[9],
        Customer *customer) {

    customer->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    customer->x = pgr_SPI_getFloat8(tuple, tupdesc,  info[1]);
    customer->y = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    customer->demand = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);
    customer->Etime = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);
    customer->Ltime = pgr_SPI_getFloat8(tuple, tupdesc, info[5]);
    customer->Stime = pgr_SPI_getFloat8(tuple, tupdesc, info[6]);
    customer->Pindex = pgr_SPI_getBigInt(tuple, tupdesc, info[7]);
    customer->Dindex = pgr_SPI_getBigInt(tuple, tupdesc, info[8]);
    customer->Ddist = 0;
}

void
pgr_get_customers(
        char *sql,
        Customer **customers,
        size_t *total_customers) {
    const int tuple_limit = 1000;

    size_t ntuples;
    size_t total_tuples = 0;

    Column_info_t info[9];

    int i;
    for (i = 0; i < 9; ++i) {
        info[i].colNumber = -1;
        info[i].type = -1;
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


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = TRUE;
    (*total_customers) = total_tuples;


    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 9);

        ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*customers) == NULL)
                (*customers) = (Customer *)palloc0(total_tuples * sizeof(Customer));
            else
                (*customers) = (Customer *)repalloc((*customers), total_tuples * sizeof(Customer));

            if ((*customers) == NULL) {
                elog(ERROR, "Out of memory");
            }

            int t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %d customer tupĺes", ntuples);

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_customer(&tuple, &tupdesc, info,
                        &(*customers)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    if (total_tuples == 0) {
        (*total_customers) = 0;
        PGR_DBG("NO _customers");
        return;
    }


    (*total_customers) = total_tuples;
    PGR_DBG("Finish reading %ld customers, %ld", total_tuples, (*total_customers));
}

