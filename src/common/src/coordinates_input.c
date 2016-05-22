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

// #define DEBUG
#include "./debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/get_check_data.h"
#include "./time_msg.h"
#include "./coordinates_input.h"



static
void pgr_fetch_row(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[3],
        Coordinate_t *distance) {

    distance->id = pgr_SPI_getBigInt(tuple, tupdesc,  info[0]);
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

    size_t ntuples;
    size_t total_tuples = 0;

    Column_info_t info[3];

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }
    info[0].name = strdup("id");
    info[1].name = strdup("x");
    info[2].name = strdup("y");

    info[0].eType = ANY_INTEGER;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = TRUE;
    (*total_coordinates) = total_tuples;

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 3);

        ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*coordinates) == NULL)
                (*coordinates) = (Coordinate_t *)palloc0(total_tuples * sizeof(Coordinate_t));
            else
                (*coordinates) = (Coordinate_t *)repalloc((*coordinates), total_tuples * sizeof(Coordinate_t));

            if ((*coordinates) == NULL) {
                elog(ERROR, "Out of memory");
            }

            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("Processing %d coordinates tupĺes", ntuples);

            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_row(&tuple, &tupdesc, info,
                        &(*coordinates)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }


    if (total_tuples == 0) {
        (*total_coordinates) = 0;
        PGR_DBG("NO coordinates");
        return;
    }

    (*total_coordinates) = total_tuples;
    time_msg(" reading coordinates:", start_t, clock());

}
