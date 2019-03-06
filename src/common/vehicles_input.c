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

#include "c_common/vehicles_input.h"

#include <limits.h>
#include <float.h>


#include "c_types/column_info_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"


static
void fetch_vehicles(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[16],
        Vehicle_t *vehicle,
        bool with_id) {
    vehicle->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    vehicle->capacity = pgr_SPI_getFloat8(tuple, tupdesc, info[1]);

    vehicle->start_x = with_id ?
        0 :
        pgr_SPI_getFloat8(tuple, tupdesc, info[2]);
    vehicle->start_y = with_id ?
        0 :
        pgr_SPI_getFloat8(tuple, tupdesc, info[3]);

    vehicle->speed = column_found(info[13].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle->cant_v =  column_found(info[4].colNumber) ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[4]) :
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


    if (!(column_found(info[8].colNumber))
            && column_found(info[9].colNumber)) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[8].name),
                 errhint("%s was found, also column is expected %s ",
                     info[9].name, info[8].name)));
    }
    if (column_found(info[8].colNumber)
            && !(column_found(info[9].colNumber))) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[9].name),
                 errhint("%s was found, also column is expected %s ",
                     info[8].name, info[9].name)));
    }

    vehicle->end_x = column_found(info[8].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[8]) :
        vehicle->start_x;
    vehicle->end_y = column_found(info[9].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[9]) :
        vehicle->start_y;

    if (!(column_found(info[10].colNumber))
            && column_found(info[11].colNumber)) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[10].name),
                 errhint("%s was found, also column is expected %s ",
                     info[10].name, info[11].name)));
    }

    if (column_found(info[10].colNumber)
            && !(column_found(info[11].colNumber))) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[11].name),
                 errhint("%s was found, also column is expected %s ",
                     info[11].name, info[10].name)));
    }
    vehicle->end_open_t = column_found(info[10].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[10]) :
        vehicle->start_open_t;
    vehicle->end_close_t = column_found(info[11].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[11]) :
        vehicle->start_close_t;
    vehicle->end_service_t = column_found(info[12].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[12]) :
        vehicle->start_service_t;

    vehicle->speed = column_found(info[13].colNumber) ?
        pgr_SPI_getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle->start_node_id = with_id ?
        pgr_SPI_getBigInt(tuple, tupdesc, info[14]) :
        0;
    vehicle->end_node_id = with_id ?
        (column_found(info[12].colNumber) ?
            pgr_SPI_getBigInt(tuple, tupdesc, info[15]) :
            vehicle->start_node_id) :
        0;
}



static
void pgr_get_vehicles_general(
        char *vehicles_sql,
        Vehicle_t **vehicles,
        size_t *total_vehicles,
        bool with_id) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    PGR_DBG("pgr_get_vehicles");
    PGR_DBG("%s", vehicles_sql);

    Column_info_t info[16];

    int i;
    for (i = 0; i < 16; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }

    info[0].name = "id";
    info[1].name = "capacity";
    info[2].name = "start_x";
    info[3].name = "start_y";
    info[4].name = "number";
    info[5].name = "start_open";
    info[6].name = "start_close";
    info[7].name = "start_service";
    info[8].name = "end_x";
    info[9].name = "end_y";
    info[10].name = "end_open";
    info[11].name = "end_close";
    info[12].name = "end_service";
    info[13].name = "speed";
    info[14].name = "start_node_id";
    info[15].name = "end_node_id";

    info[0].eType = ANY_INTEGER;
    info[4].eType = ANY_INTEGER;
    info[14].eType = ANY_INTEGER;
    info[15].eType = ANY_INTEGER;

    for (i = 4; i < 16; ++i) {
        info[i].strict = false;
    }

    if (with_id) {
        /*
         *  with id, then start_x and start_y are optional
         *  start_node_id is compulsory
         */
        info[2].strict = false;
        info[3].strict = false;
        info[14].strict = true;
    }

    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(vehicles_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = true;
    (*total_vehicles) = total_tuples = 0;

    /* on the first tuple get the column numbers */

    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 16);
        }
        size_t ntuples = SPI_processed;
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
                        &(*vehicles)[total_tuples - ntuples + t], with_id);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0) {
        (*total_vehicles) = 0;
        PGR_DBG("NO orders");
        return;
    }

    (*total_vehicles) = total_tuples;
    if (with_id) {
        PGR_DBG("Finish reading %ld vehicles for matrix", (*total_vehicles));
    } else {
        PGR_DBG("Finish reading %ld vehicles for euclidean", (*total_vehicles));
    }
    time_msg("reading edges", start_t, clock());
}

void
pgr_get_vehicles(
        char *vehicles_sql,
        Vehicle_t **vehicles,
        size_t *total_vehicles) {
    pgr_get_vehicles_general(vehicles_sql, vehicles, total_vehicles, false);
}

void
pgr_get_vehicles_with_id(
        char *vehicles_sql,
        Vehicle_t **vehicles,
        size_t *total_vehicles) {
    pgr_get_vehicles_general(vehicles_sql, vehicles, total_vehicles, true);
}

