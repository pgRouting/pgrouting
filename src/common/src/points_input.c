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

#include "postgres.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "executor/spi.h"


// #define DEBUG
#include "./debug_macro.h"
#include "pgr_types.h"
#include "postgres_connection.h"
#include "get_check_data.h"
#include "points_input.h"


static
bool
column_found(int colNumber) {
    return !(colNumber == SPI_ERROR_NOATTRIBUTE);
}

static
bool
fetch_column_info(
        Column_info_t *info) {

    PGR_DBG("Fetching column info of %s", info->name);
    info->colNumber =  SPI_fnumber(SPI_tuptable->tupdesc, info->name);
    if (info->strict && !column_found(info->colNumber)) {
        elog(ERROR, "Column '%s' not Found", info->name);
    }
    if (column_found(info->colNumber)) {
        (info->type) = SPI_gettypeid(SPI_tuptable->tupdesc, (info->colNumber));
        if (SPI_result == SPI_ERROR_NOATTRIBUTE) {
            elog(ERROR, "Type of column '%s' not Found", info->name);
        }
        PGR_DBG("Column found");
        return true;
    }
    PGR_DBG("Column not found");
    return false;
}



static
void fetch_points_column_info(
        Column_info_t info[],
        int info_size) {
    PGR_DBG("Entering Fetch_points_column_info\n");

    int i;
    for ( i = 0; i < info_size; ++i) {
        if (fetch_column_info(&info[i])) {
            switch (info[i].eType) {
                case ANY_INTEGER:
                    pgr_check_any_integer_type(info[i].name, info[i].type);
                    break;
                case ANY_NUMERICAL:
                    pgr_check_any_numerical_type(info[i].name, info[i].type);
                    break;
                case TEXT:
                    pgr_check_text_type(info[i].name, info[i].type);
                    break;
                case CHAR:
                    pgr_check_char_type(info[i].name, info[i].type);
                    break;
                default:
                    elog(ERROR, "Unknown type");
            }
        }
    }
}

static
void fetch_point(
        HeapTuple *tuple,
        TupleDesc *tupdesc, 
        Column_info_t info[4],
        int64_t *default_pid,
        char default_side,
        Point_on_edge_t *point) {
    if (column_found(info[0].colNumber)) {
        point->pid = pgr_SPI_getBigInt(tuple, tupdesc, info[0].colNumber, info[0].type);
    } else {
        point->pid = *default_pid;
        ++(*default_pid);
    }

    point->edge_id = pgr_SPI_getBigInt(tuple, tupdesc, info[1].colNumber, info[1].type);
    point->fraction = pgr_SPI_getFloat8(tuple, tupdesc, info[2].colNumber, info[2].type);

    if (column_found(info[3].colNumber)) {
        point->side = (char)pgr_SPI_getChar(tuple, tupdesc, info[3].colNumber, info[3].type, false, default_side);
    } else {
        point->side = default_side;
    }

}


// pid, edge_id, fraction, [side] 
void
pgr_get_points(
        char *points_sql,
        Point_on_edge_t **points,
        int64_t *total_points) {

    const int tuple_limit = 1000000;

    PGR_DBG("Entering pgr_get_get_points");

    int64_t ntuples;
    int64_t total_tuples;

    Column_info_t info[4];

    int i;
    for (i = 0; i < 4; ++i) {
        info[i].colNumber = -1;
        info[i].type = -1;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = strdup("pid");
    info[1].name = strdup("edge_id");
    info[2].name = strdup("fraction");
    info[3].name = strdup("side");

    info[0].strict = false;
    info[3].strict = false;
    info[2].eType = ANY_NUMERICAL;
    info[3].eType = CHAR;



    void *SPIplan;
    SPIplan = pgr_SPI_prepare(points_sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = TRUE;
    (*total_points) = total_tuples = 0;

    int64_t default_pid = 0;
    char default_side = 'b';

    PGR_DBG("Starting Cycle");
    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0) {
            /* on the first tuple get the column information */
            fetch_points_column_info(info, 4);
        }

        ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            PGR_DBG("Getting Memory");
            if ((*points) == NULL)
                (*points) = (Point_on_edge_t *)palloc0(total_tuples * sizeof(Point_on_edge_t));
            else
                (*points) = (Point_on_edge_t *)repalloc((*points), total_tuples * sizeof(Point_on_edge_t));
            PGR_DBG("Got Memory");

            if ((*points) == NULL) {
                elog(ERROR, "Out of memory"); 
            }

            int64_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing total points:%ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                PGR_DBG("   processing point #%ld", t);
                HeapTuple tuple = tuptable->vals[t];
                fetch_point(&tuple, &tupdesc, info,
                        &default_pid, default_side,
                        &(*points)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    if (total_tuples == 0) {
        (*total_points) = 0;
        PGR_DBG("NO points");
        PGR_DBG("closed");
        return;
    }


    (*total_points) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_points));
    PGR_DBG("closed");
}


