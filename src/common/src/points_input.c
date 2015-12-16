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
#include "points_input.h"


struct {
    int column;
    int type;
    strict bool;
    char *name;
} Column_info_t;

static
bool
column_found(int colNumber) {
    return !(colNumber == SPI_ERROR_NOATTRIBUTE);
}

static
bool
fetch_column_info(
        int *colNumber,
        int *coltype,
        char *colName,
        bool strict) {
    PGR_DBG("Fetching column info of %s", colName);
    (*colNumber) =  SPI_fnumber(SPI_tuptable->tupdesc, colName);
    if (strict && !column_found(*colNumber)) {
        elog(ERROR, "Column '%s' not Found", colName);
    }
    if (column_found(*colNumber)) {
        (*coltype) = SPI_gettypeid(SPI_tuptable->tupdesc, (*colNumber));
        if (SPI_result == SPI_ERROR_NOATTRIBUTE) {
            elog(ERROR, "Type of column '%s' not Found", colName);
        }
        PGR_DBG("Column found");
        return true;
    }
    PGR_DBG("Column not found");
    return false;
}



static
void fetch_points_column_info(
        int columns[4], 
        int types[4]) { 

    PGR_DBG("Entering Fetch_points_column_info\n");

    if (fetch_column_info(&columns[0], &types[0], "pid", false)) {
        pgr_check_any_integer_type("pid", types[0]);
    }

    fetch_column_info(&columns[1], &types[1], "edge_id", true);
    pgr_check_any_integer_type("edge_id", types[1]);

    fetch_column_info(&columns[2], &types[2], "fraction", true);
    pgr_check_any_numerical_type("fraction", types[2]);

    if (fetch_column_info(&columns[3], &types[3], "side", false)) {
        pgr_check_char_type("side", types[3]);
    }
}

static
void fetch_point(
        HeapTuple *tuple,
        TupleDesc *tupdesc, 
        int columns[4],
        int types[4],
        int64_t default_pid,
        char default_side,
        Point_on_edge_t *point) {
    if (column_found(columns[0])) {
        point->pid = pgr_SPI_getBigInt(tuple, tupdesc, columns[0], types[0]);
    } else {
        point->pid = default_pid;
    }
    point->edge_id = pgr_SPI_getBigInt(tuple, tupdesc, columns[1], types[1]);
    point->fraction = pgr_SPI_getFloat8(tuple, tupdesc, columns[2], types[2]);
    if (column_found(columns[3])) {
        point->side = (char)pgr_SPI_getChar(tuple, tupdesc, columns[3], types[3], false, default_side);
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

    int columns[4];
    int types[4];
    int i;
    for (i = 0; i < 4; ++i) {
        info[i].column = -1;
        info[i].type = -1;
        info[i].strict = true;
    }
    info[0].name = strdup("pid");
    info[1].name = strdup("edge_id");
    info[2].name = strdup("fraction");
    info[3].name = strdup("side");
    info[0].strict = false;
    info[4].strict = false;

    for (i = 0; i < 4; ++i) columns[i] = -1;
    for (i = 0; i < 4; ++i) types[i] = -1;


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
            fetch_points_column_info(columns, types);
#ifdef DEBUG
            int i;
            PGR_DBG("i\tcolumns\types");
            for (i = 0; i < 4; i++) {
                PGR_DBG("i=%d\t%d\t%d", i, columns[i], types[i]);
            }   
#endif

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
                fetch_point(&tuple, &tupdesc,
                        columns, types,
                        default_pid, default_side,
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


