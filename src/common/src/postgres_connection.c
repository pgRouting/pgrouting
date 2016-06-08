/*PGR-GNU*****************************************************************
File: postgres_connection.c

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
#include "catalog/pg_type.h"
#include "executor/spi.h"


// #define DEBUG
#include "./debug_macro.h"
#include "./postgres_connection.h"

void
pgr_send_error(int errcode) {
    switch (errcode) {
        case 1:
            elog(ERROR, "Unexpected point(s) with same pid but different edge/fraction/side combination found.");
            break;
        case 2:
            elog(ERROR, "Internal: Unexpected mismatch count and sequence number on results");
            break;
        default:
            elog(ERROR, "Unknown exception");
    }
}


char*
pgr_text2char(text *in) {
    char *out = palloc(VARSIZE(in));

    memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
    out[VARSIZE(in) - VARHDRSZ] = '\0';
    return out;
}


// http://www.postgresql.org/docs/9.4/static/spi-spi-finish.html
void
pgr_SPI_finish(void) {
    PGR_DBG("Disconnecting SPI");
    int code = SPI_OK_FINISH;
    code = SPI_finish();
    if (code != SPI_OK_FINISH) {  // SPI_ERROR_UNCONNECTED
        elog(ERROR, "There was no connection to SPI");
    }
}

void
pgr_SPI_connect(void) {
    PGR_DBG("Connecting to SPI");
    int SPIcode;
    SPIcode = SPI_connect();
    if (SPIcode  != SPI_OK_CONNECT) {
        elog(ERROR, "Couldn't open a connection to SPI");
    }
}

SPIPlanPtr
pgr_SPI_prepare(char* sql) {
    PGR_DBG("Preparing Plan");
    SPIPlanPtr SPIplan;
    SPIplan = SPI_prepare(sql, 0, NULL);
    if (SPIplan  == NULL) {
        elog(ERROR, "Couldn't create query plan via SPI: %s", sql);
    }
    return SPIplan;
}

Portal
pgr_SPI_cursor_open(SPIPlanPtr SPIplan) {
    PGR_DBG("Opening Portal");
    Portal SPIportal;
    SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true);
    if (SPIportal == NULL) {
        elog(ERROR, "SPI_cursor_open returns NULL");
    }
    return SPIportal;
}
