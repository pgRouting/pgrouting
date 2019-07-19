/*PGR-GNU*****************************************************************
File: matrixRows_input.c

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

#include "c_common/matrixRows_input.h"

/* for bool */
#   include <stdbool.h>
/* for size_t */
#   include <stddef.h>

#include "c_types/column_info_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"



static
void pgr_fetch_row(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[3],
        Matrix_cell_t *distance) {
    distance->from_vid = pgr_SPI_getBigInt(tuple, tupdesc,  info[0]);
    distance->to_vid = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
    distance->cost = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
}

/*!
 * bigint start_vid,
 * bigint end_vid,
 * float agg_cost,
 */
void pgr_get_matrixRows(
        char *sql,
        Matrix_cell_t **rows,
        size_t *total_rows) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples = 0;

    Column_info_t info[3];

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = "start_vid";
    info[1].name = "end_vid";
    info[2].name = "agg_cost";

    info[2].eType = ANY_NUMERICAL;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*total_rows) = total_tuples;

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 3);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*rows) == NULL)
                (*rows) = (Matrix_cell_t *)palloc0(
                        total_tuples * sizeof(Matrix_cell_t));
            else
                (*rows) = (Matrix_cell_t *)repalloc(
                        (*rows), total_tuples * sizeof(Matrix_cell_t));

            if ((*rows) == NULL) {
                elog(ERROR, "Out of memory");
            }

            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld matrix cell tupĺes", ntuples);

            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_row(&tuple, &tupdesc, info,
                        &(*rows)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);


    if (total_tuples == 0) {
        (*total_rows) = 0;
        PGR_DBG("NO rows");
        return;
    }

    (*total_rows) = total_tuples;
    time_msg(" reading matrix cells", start_t, clock());
}
