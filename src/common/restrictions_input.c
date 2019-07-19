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

#include "c_common/restrictions_input.h"

#include "c_types/column_info_t.h"

#include "c_common/debug_macro.h"
#include "c_common/time_msg.h"
#include "c_common/get_check_data.h"


static
void fetch_restriction(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[3],
        Restriction_t *restriction) {
    /*
     * reading the restriction id
     */
    restriction->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);

    /*
     * reading the cost
     */
    if (column_found(info[1].colNumber)) {
        restriction->cost = pgr_SPI_getFloat8(tuple, tupdesc, info[1]);
    } else {
        restriction->cost = -1;
    }

    restriction->via = NULL;
    restriction->via_size = 0;

    restriction->via = pgr_SPI_getBigIntArr(
            tuple, tupdesc, info[2], &restriction->via_size);
}


void
pgr_get_restrictions(
        char *restrictions_sql,
        Restriction_t **restrictions,
        size_t *total_restrictions) {
    const int tuple_limit = 1000000;
    clock_t start_t = clock();

    PGR_DBG("pgr_get_restrictions");
    PGR_DBG("%s", restrictions_sql);

    Column_info_t info[3];

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
    }

    /* restriction id */
    info[0].name = "id";
    info[1].name = "cost";
    /* array of edges */
    info[2].name = "path";

    info[0].eType = ANY_INTEGER;
    info[1].eType = ANY_NUMERICAL;
    info[2].eType = ANY_INTEGER_ARRAY;

    info[1].strict = false;

    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(restrictions_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = true;
    (*total_restrictions) = total_tuples = 0;


    /*  on the first tuple get the column numbers */

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 3);
        }
        size_t ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("Restrictions to be processed %ld", ntuples);
        PGR_DBG("size of structure %ld",  sizeof(Restriction_t));
        if (ntuples > 0) {
            if ((*restrictions) == NULL) {
                (*restrictions) = (Restriction_t *)palloc(
                        total_tuples * sizeof(Restriction_t));
            } else {
                (*restrictions) = (Restriction_t *)repalloc(
                        (*restrictions),
                        total_tuples * sizeof(Restriction_t));
            }

            if ((*restrictions) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_restriction(&tuple, &tupdesc, info,
                        &(*restrictions)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0) {
        (*total_restrictions) = 0;
        PGR_DBG("NO restrictions");
        return;
    }

    (*total_restrictions) = total_tuples;
    PGR_DBG("Finish reading %ld restrictions, %ld",
            total_tuples,
            (*total_restrictions));
    clock_t end_t = clock();
    time_msg(" reading Restrictions", start_t, end_t);
}
