/*PGR-GNU*****************************************************************
File: restrictions_input.c

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
#include "executor/spi.h"

#include "./debug_macro.h"
#include "./pgr_types.h"
#include "./time_msg.h"
#include "./postgres_connection.h"
#include "./get_check_data.h"
#include "./restrictions_input.h"


static
void fetch_restriction(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[4],
        Restrict_t *restriction) {
    restriction->target_id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    restriction->to_cost = pgr_SPI_getFloat8(tuple, tupdesc,  info[1]);
    char *str = DatumGetCString(SPI_getvalue(*tuple, *tupdesc, info[2].colNumber));

// TODO(someone) because its  text, there is no garanee that the text read is correct
// move this code to c++ to tokenize the integers.

    int i = 0;
    for (i = 0; i < MAX_RULE_LENGTH; ++i) restriction->via[i] = -1;

    if (str != NULL) {
        char *token = NULL;
        int i = 0;

        token = (char *)strtok(str, " ,");

        while (token != NULL && i < MAX_RULE_LENGTH) {
            restriction->via[i] = atoi(token);
            i++;
            token = (char *)strtok(NULL, " ,");
        }
    }
}


void
pgr_get_restriction_data(
        char *restrictions_sql,
        Restrict_t **restrictions,
        size_t *total_restrictions) {
    const int tuple_limit = 1000000;
    clock_t start_t = clock();

    PGR_DBG("pgr_get_restriction_data");
    PGR_DBG("%s", restrictions_sql);

    Column_info_t info[3];

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = strdup("target_id");
    info[1].name = strdup("to_cost");
    info[2].name = strdup("via_path");

    info[1].eType = ANY_NUMERICAL;
    info[2].eType = TEXT;


    size_t ntuples;
    size_t total_tuples;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(restrictions_sql);
    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);

    bool moredata = TRUE;
    (*total_restrictions) = total_tuples = 0;

    /*  on the first tuple get the column numbers */

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0) {
            pgr_fetch_column_info(info, 3);
        }
        ntuples = SPI_processed;
        total_tuples += ntuples;
        PGR_DBG("SPI_processed %ld", ntuples);
        if (ntuples > 0) {
            if ((*restrictions) == NULL)
                (*restrictions) = (Restrict_t *)palloc0(total_tuples * sizeof(Restrict_t));
            else
                (*restrictions) = (Restrict_t *)repalloc((*restrictions), total_tuples * sizeof(Restrict_t));

            if ((*restrictions) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %ld", ntuples);
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_restriction(&tuple, &tupdesc, info,
                        &(*restrictions)[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    if (total_tuples == 0) {
        (*total_restrictions) = 0;
        PGR_DBG("NO restrictions");
        return;
    }

    (*total_restrictions) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*total_restrictions));
    clock_t end_t = clock();
    time_msg(" reading Restrictions", start_t, end_t);
}
