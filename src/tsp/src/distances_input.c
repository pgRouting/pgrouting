/*PGR-GNU*****************************************************************
File: distances_input.c

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
#include "./distances_input.h"



static
void pgr_fetch_distance(
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
void pgr_get_distances(
        char *sql,
        Matrix_cell_t **distances,
        size_t *total_distances) {

    const int tuple_limit = 1000000;

    size_t ntuples;
    size_t total_tuples = 0;

    Column_info_t info[3];

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = strdup("start_vid");
    info[1].name = strdup("end_vid");
    info[2].name = strdup("agg_cost");

    info[2].eType = ANY_NUMERICAL;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = TRUE;
    (*total_distances) = total_tuples;

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 5);

        ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*distances) == NULL)
                (*distances) = (Matrix_cell_t *)palloc0(total_tuples * sizeof(Matrix_cell_t));
            else
                (*distances) = (Matrix_cell_t *)repalloc((*distances), total_tuples * sizeof(Matrix_cell_t));

            if ((*distances) == NULL) {
                elog(ERROR, "Out of memory");
            }

            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %d edge tupĺes", ntuples);

            for (size_t t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_distance(&tuple, &tupdesc, info,
                        &(*distances)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }


    if (total_tuples == 0) {
        (*total_distances) = 0;
        PGR_DBG("NO distances");
        return;
    }

    (*total_distances) = total_tuples;
    PGR_DBG("Finish reading %ld edges, %ld", total_tuples, (*total_distances));
}
