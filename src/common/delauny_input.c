/*PGR-GNU*****************************************************************
File: delauny_input.c

Copyright (c) 2018 Celia Virginia Vergara Castillo
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

#include "c_common/delauny_input.h"

#include "c_types/delauny_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"



static
void pgr_fetch_row(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[4],
        Delauny_t *delauny) {
    delauny->tid = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    delauny->pid = pgr_SPI_getBigInt(tuple, tupdesc, info[1]);
    delauny->x = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    delauny->y = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);
}

/*!
 * bigint tid,
 * bigint pid,
 * float x,
 * float y,
 */
void pgr_get_delauny(
        char *sql,
        Delauny_t **delauny,
        size_t *total_delauny) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples = 0;

    Column_info_t info[4];

    int i;
    for (i = 0; i < 4; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }
    info[0].name = "tid";
    info[1].name = "pid";
    info[2].name = "x";
    info[3].name = "y";

    info[0].eType = ANY_INTEGER;
    info[1].eType = ANY_INTEGER;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*total_delauny) = total_tuples;

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 4);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*delauny) == NULL)
                (*delauny) = (Delauny_t *)
                    palloc0(total_tuples * sizeof(Delauny_t));
            else
                (*delauny) = (Delauny_t *)
                    repalloc((*delauny),
                            total_tuples * sizeof(Delauny_t));

            if ((*delauny) == NULL) {
                elog(ERROR, "Out of memory");
            }

            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_row(&tuple, &tupdesc, info,
                        &(*delauny)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);


    if (total_tuples == 0) {
        (*total_delauny) = 0;
        return;
    }

    (*total_delauny) = total_tuples;
    PGR_DBG("%ld", total_tuples);
    time_msg(" calculating Delauny triangles:", start_t, clock());
}
