/*PGR-GNU*****************************************************************
File: points_input.c

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

#include "c_common/points_input.h"

/* for bool */
#   include <stdbool.h>
/* for size_t */
#   include <stddef.h>


#include "c_types/column_info_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"



static
void fetch_point(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[4],
        int64_t *default_pid,
        char default_side,
        Point_on_edge_t *point) {
    if (column_found(info[0].colNumber)) {
        point->pid = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    } else {
        point->pid = *default_pid;
        ++(*default_pid);
    }

    point->edge_id = pgr_SPI_getBigInt(tuple, tupdesc, info[1]);
    point->fraction = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);

    if (column_found(info[3].colNumber)) {
        point->side =
            (char)pgr_SPI_getChar(tuple, tupdesc, info[3], false, default_side);
    } else {
        point->side = default_side;
    }
}


// pid, edge_id, fraction, [side]
void
pgr_get_points(
        char *points_sql,
        Point_on_edge_t **points,
        size_t *total_points) {
    const int tuple_limit = 1000;

    size_t total_tuples;
    Column_info_t info[4];

    int i;
    for (i = 0; i < 4; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }

    info[0].name = "pid";
    info[1].name = "edge_id";
    info[2].name = "fraction";
    info[3].name = "side";

    info[0].strict = false;
    info[3].strict = false;
    info[2].eType = ANY_NUMERICAL;
    info[3].eType = CHAR1;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(points_sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = true;
    (*total_points) = total_tuples = 0;

    int64_t default_pid = 1;
    char default_side = 'b';

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0) {
            /* on the first tuple get the column information */
            pgr_fetch_column_info(info, 4);
        }

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*points) == NULL)
                (*points) = (Point_on_edge_t *)
                    palloc0(total_tuples * sizeof(Point_on_edge_t));
            else
                (*points) = (Point_on_edge_t *)
                    repalloc((*points), total_tuples * sizeof(Point_on_edge_t));

            if ((*points) == NULL) {
                elog(ERROR, "Out of memory");
            }

            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            size_t t;

            PGR_DBG("processing %ld points tuples", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_point(&tuple, &tupdesc, info,
                        &default_pid, default_side,
                        &(*points)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }
    SPI_cursor_close(SPIportal);


    if (total_tuples == 0) {
        (*total_points) = 0;
        PGR_DBG("NO points");
        return;
    }

    (*total_points) = total_tuples;
    PGR_DBG("Finish reading %ld points, %ld", total_tuples, (*total_points));
}
