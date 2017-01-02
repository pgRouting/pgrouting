/*PGR-GNU*****************************************************************
File: vehicles_input.c

Copyright (c) 2016 Celia Virginia Vergara Castillo
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

#include "./postgres_connection.h"

#include <limits.h>

#include "./debug_macro.h"
#include "./pgr_types.h"
#include "./get_check_data.h"
#include "./vehicles_input.h"


static
void fetch_vehicles(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[14],
        Vehicle_t *vehicle) {

    vehicle->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    vehicle->capacity = pgr_SPI_getFloat8(tuple, tupdesc, info[1]);
    vehicle->start_x = pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    vehicle->start_y = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);

    vehicle->cant_v =  column_found(info[4].colNumber) ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[4]) :
        1;
    vehicle->speed = column_found(info[13].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle->start_open_t = column_found(info[5].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[5]) :
        0;
    vehicle->start_close_t = column_found(info[6].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[6]) :
        DBL_MAX;
    vehicle->start_service_t = column_found(info[7].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[7]) :
        0;

    vehicle->end_x = column_found(info[8].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[8]) :
        vehicle->start_x;
    vehicle->end_y = column_found(info[9].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[9]) :
        vehicle->start_y;
    vehicle->end_open_t = column_found(info[10].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[10]) :
        vehicle->start_open_t;
    vehicle->end_close_t = column_found(info[11].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[11]) :
        vehicle->start_close_t;
    vehicle->end_service_t = column_found(info[12].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[12]) :
        vehicle->start_service_t;
}


static
void fetch_vehicles_with_id(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[14],
        Vehicle_t *vehicle) {

    vehicle->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    vehicle->capacity = pgr_SPI_getFloat8(tuple, tupdesc, info[1]);
    vehicle->cant_v =  column_found(info[2].colNumber) ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[2]) :
        1;
    vehicle->start_node_id = pgr_SPI_getBigInt(tuple, tupdesc, info[3]);

    vehicle->start_open_t = column_found(info[4].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[4]) :
        0;
    vehicle->start_close_t = column_found(info[5].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[5]) :
        DBL_MAX;
    vehicle->start_service_t = column_found(info[6].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[6]) :
        0;

    vehicle->end_node_id = column_found(info[7].colNumber) ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[7]) :
        vehicle->start_node_id;
    vehicle->end_open_t = column_found(info[8].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[8]) :
        vehicle->start_open_t;
    vehicle->end_close_t = column_found(info[9].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[9]) :
        vehicle->start_close_t;
    vehicle->end_service_t = column_found(info[10].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[10]) :
        vehicle->start_service_t;
    vehicle->speed = column_found(info[11].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[11]) :
        1;
}



void
pgr_get_vehicles(
        char *vehicles_sql,
        Vehicle_t **vehicles,
        size_t *total_vehicles) {
    const int tuple_limit = 1000000;

    PGR_DBG("pgr_get_vehicles");
    PGR_DBG("%s", vehicles_sql);

    Column_info_t info[14];

    int i;
    for (i = 0; i < 14; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }

    info[0].name = strdup("id");
    info[1].name = strdup("capacity");
    info[2].name = strdup("start_x");
    info[3].name = strdup("start_y");
    info[4].name = strdup("number");
    info[5].name = strdup("start_open");
    info[6].name = strdup("start_close");
    info[7].name = strdup("start_service");
    info[8].name = strdup("end_x");
    info[9].name = strdup("end_y");
    info[10].name = strdup("end_open");
    info[11].name = strdup("end_close");
    info[12].name = strdup("end_service");
    info[13].name = strdup("speed");

    info[0].eType = ANY_INTEGER;
    info[4].eType = ANY_INTEGER;

    for (i = 4; i < 14; ++i) {
        info[i].strict = false;
    }


    size_t ntuples;
    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(vehicles_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = TRUE;
    (*total_vehicles) = total_tuples = 0;

    /* on the first tuple get the column numbers */

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 14);
        }
        ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("SPI_processed %ld", ntuples);
        if (ntuples > 0) {
            if ((*vehicles) == NULL)
                (*vehicles) = (Vehicle_t *)palloc0(
                        total_tuples * sizeof(Vehicle_t));
            else
                (*vehicles) = (Vehicle_t *)repalloc(
                        (*vehicles),
                        total_tuples * sizeof(Vehicle_t));

            if ((*vehicles) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_vehicles(&tuple, &tupdesc, info,
                        &(*vehicles)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0) {
        (*total_vehicles) = 0;
        PGR_DBG("NO orders");
        return;
    }

    (*total_vehicles) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_vehicles));
}

void
pgr_get_vehicles_with_id(
        char *vehicles_sql,
        Vehicle_t **vehicles,
        size_t *total_vehicles) {
    const int tuple_limit = 1000000;

    PGR_DBG("pgr_get_vehicles");
    PGR_DBG("%s", vehicles_sql);

    Column_info_t info[12];

    int i;
    for (i = 0; i < 12; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }

    info[0].name = strdup("id");
    info[1].name = strdup("capacity");
    info[2].name = strdup("number");
    info[3].name = strdup("start_node_id");
    info[4].name = strdup("start_open");
    info[5].name = strdup("start_close");
    info[6].name = strdup("start_service");
    info[7].name = strdup("end_node_id");
    info[8].name = strdup("end_open");
    info[9].name = strdup("end_close");
    info[10].name = strdup("end_service");
    info[11].name = strdup("speed");

    info[0].eType = ANY_INTEGER;
    info[2].eType = ANY_INTEGER;
    info[3].eType = ANY_INTEGER;
    info[7].eType = ANY_INTEGER;

    /* optionals are the end_*, speed */
    for (i = 4; i < 12; ++i) {
        info[i].strict = false;
    }
    info[2].strict = false;


    size_t ntuples;
    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(vehicles_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = TRUE;
    (*total_vehicles) = total_tuples = 0;

    /* on the first tuple get the column numbers */

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 12);
        }
        ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("SPI_processed %ld", ntuples);
        if (ntuples > 0) {
            if ((*vehicles) == NULL)
                (*vehicles) = (Vehicle_t *)palloc0(
                        total_tuples * sizeof(Vehicle_t));
            else
                (*vehicles) = (Vehicle_t *)repalloc(
                        (*vehicles),
                        total_tuples * sizeof(Vehicle_t));

            if ((*vehicles) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_vehicles_with_id(&tuple, &tupdesc, info,
                        &(*vehicles)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0) {
        (*total_vehicles) = 0;
        PGR_DBG("NO orders");
        return;
    }

    (*total_vehicles) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_vehicles));
}
