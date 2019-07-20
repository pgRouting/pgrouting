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

#include "c_common/coordinates_input.h"

#include "c_types/column_info_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"



static
void pgr_fetch_row(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[3],
        int64_t *default_id,
        Coordinate_t *distance) {
    if (column_found(info[0].colNumber)) {
        distance->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    } else {
        distance->id = *default_id;
        ++(*default_id);
    }
    distance->x = pgr_SPI_getFloat8(tuple, tupdesc,  info[1]);
    distance->y = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
}

/*!
 * bigint id,
 * float x,
 * float y,
 */
void pgr_get_coordinates(
        char *sql,
        Coordinate_t **coordinates,
        size_t *total_coordinates) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples = 0;

    Column_info_t info[3];

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }
    info[0].name = "id";
    info[1].name = "x";
    info[2].name = "y";

    info[0].eType = ANY_INTEGER;
    info[0].strict = false;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*total_coordinates) = total_tuples;

    int64_t default_id = 1;

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 3);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*coordinates) == NULL)
                (*coordinates) = (Coordinate_t *)
                    palloc0(total_tuples * sizeof(Coordinate_t));
            else
                (*coordinates) = (Coordinate_t *)
                    repalloc((*coordinates),
                            total_tuples * sizeof(Coordinate_t));

            if ((*coordinates) == NULL) {
                elog(ERROR, "Out of memory");
            }

            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("Processing %ld coordinates tupĺes", ntuples);

            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_row(&tuple, &tupdesc, info,
                        &default_id,
                        &(*coordinates)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);


    if (total_tuples == 0) {
        (*total_coordinates) = 0;
        PGR_DBG("NO coordinates");
        return;
    }

    (*total_coordinates) = total_tuples;
    time_msg(" reading coordinates:", start_t, clock());
}
