/*PGR-GNU*****************************************************************
File: edges_input.c

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
#include "./pgr_types.h"
#include "./postgres_connection.h"
#include "./get_check_data.h"
#include "./edges_input.h"


static
bool has_reverse_cost() {
    PGR_DBG("Checking for has_rcost");
    int colNumber;
    colNumber =  SPI_fnumber(SPI_tuptable->tupdesc, "reverse_cost");
    if (colNumber == SPI_ERROR_NOATTRIBUTE) {
        PGR_DBG("  NOT has_rcost");
        return false;
    } else {
        PGR_DBG("  has_rcost");
        return true;
    }
}



/********************
  Functions for pgr_foo with sql:
  id, source, target, cost, reverse_cost(optional) 
 ************/
static
void pgr_fetch_edge_5_columns_info(
        int (*edge_columns)[5], 
        int (*column_types)[5], 
        bool *has_rcost,
        bool ignore_id) {
    PGR_DBG("Fetching 5 column info\n");
    if (!ignore_id) {
        pgr_fetch_column_info(&(*edge_columns)[0], &(*column_types)[0], "id");
        pgr_check_any_integer_type("id", (*column_types)[0]);
    }
    pgr_fetch_column_info(&(*edge_columns)[1], &(*column_types)[1], "source");
    pgr_check_any_integer_type("source", (*column_types)[1]);
    pgr_fetch_column_info(&(*edge_columns)[2], &(*column_types)[2], "target");
    pgr_check_any_integer_type("target", (*column_types)[2]);
    pgr_fetch_column_info(&(*edge_columns)[3], &(*column_types)[3], "cost");
    pgr_check_any_numerical_type("cost", (*column_types)[3]);
    (*has_rcost) = has_reverse_cost();
    if (*has_rcost) {
        pgr_fetch_column_info(&(*edge_columns)[4], &(*column_types)[4], "reverse_cost");
        pgr_check_any_numerical_type("reverse_cost", (*column_types)[4]);
    }
    PGR_DBG("Found has_rcost %i\n", *has_rcost);
}

static
void pgr_fetch_edge(
        HeapTuple *tuple,
        TupleDesc *tupdesc, 
        int (*edge_columns)[5],
        int (*edge_types)[5],
        pgr_edge_t *target_edge,
        bool has_rcost,
        bool ignore_id,
        int64_t *valid_edges) {

    if (ignore_id) {
        target_edge->id = -1;
    } else {
        target_edge->id = pgr_SPI_getBigInt(tuple, tupdesc, (*edge_columns)[0], (*edge_types)[0]);
    }
    target_edge->source = pgr_SPI_getBigInt(tuple, tupdesc, (*edge_columns)[1], (*edge_types)[1]);
    target_edge->target = pgr_SPI_getBigInt(tuple, tupdesc, (*edge_columns)[2], (*edge_types)[2]);
    target_edge->cost = pgr_SPI_getFloat8(tuple, tupdesc, (*edge_columns)[3], (*edge_types)[3]);

    PGR_DBG("has_rcost %i\n", has_rcost);
    if (has_rcost) {
        target_edge->reverse_cost = pgr_SPI_getFloat8(tuple, tupdesc, (*edge_columns)[4], (*edge_types)[4]);
    } else {
        target_edge->reverse_cost = -1.0;
    }
    PGR_DBG("id: %li\t source: %li\ttarget: %li\tcost: %f\t,reverse: %f\n",
            target_edge->id,  target_edge->source,  target_edge->target,  target_edge->cost,  target_edge->reverse_cost);
    *valid_edges = target_edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = target_edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}





static
void
get_data_5_columns(
        char *sql,
        pgr_edge_t **edges,
        int64_t *totalTuples,
        bool ignore_id) {

    const int tuple_limit = 1000000;
    bool has_rcost = false;

    PGR_DBG("Entering pgr_get_data");

    int ntuples;
    int64_t total_tuples;
    int64_t valid_edges;

    int edge_columns[5];
    int edge_types[5];
    int i;
    for (i = 0; i < 5; ++i) edge_columns[i] = -1;
    for (i = 0; i < 5; ++i) edge_types[i] = -1;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = TRUE;
    (*totalTuples) = total_tuples = valid_edges = 0;

    /*  on the first tuple get the column numbers */

    PGR_DBG("Starting Cycle");
    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_edge_5_columns_info(&edge_columns, &edge_types, &has_rcost, ignore_id);

        ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            PGR_DBG("Getting Memory");
            if ((*edges) == NULL)
                (*edges) = (pgr_edge_t *)palloc0(total_tuples * sizeof(pgr_edge_t));
            else
                (*edges) = (pgr_edge_t *)repalloc((*edges), total_tuples * sizeof(pgr_edge_t));
            PGR_DBG("Got Memory");

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory"); 
            }

            int t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %d", ntuples);
            for (t = 0; t < ntuples; t++) {
                PGR_DBG("   processing %d", t);
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_edge(&tuple, &tupdesc, &edge_columns, &edge_types,
                        &(*edges)[total_tuples - ntuples + t], has_rcost,
                        ignore_id, &valid_edges);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }

    if (total_tuples == 0 || valid_edges == 0) {
        (*totalTuples) = 0;
        PGR_DBG("NO edges");
        PGR_DBG("closed");
        return;
    }


    (*totalTuples) = total_tuples;
    PGR_DBG("Finish reading %ld data, %ld", total_tuples, (*totalTuples));
    PGR_DBG("closed");
}



void
pgr_get_data_5_columns(
        char *sql,
        pgr_edge_t **edges,
        int64_t *totalTuples) {
    bool ignore_id = false;
    get_data_5_columns( sql, edges, totalTuples, ignore_id);
}

void
pgr_get_data_4_columns(
        char *sql,
        pgr_edge_t **edges,
        int64_t *totalTuples) {
    bool ignore_id = true;
    get_data_5_columns( sql, edges, totalTuples, ignore_id);
}


