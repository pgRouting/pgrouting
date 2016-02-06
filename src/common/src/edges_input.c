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

// #define DEBUG
#include "./debug_macro.h"
#include "./pgr_types.h"
#include "./postgres_connection.h"
#include "./get_check_data.h"
#include "./edges_input.h"



static
void pgr_fetch_edge(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[5],
        int64_t *default_id,
        float8 default_rcost,
        pgr_edge_t *edge,
        size_t *valid_edges) {
    if (column_found(info[0].colNumber)) {
        edge->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }

    edge->source = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
    edge->target = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    edge->cost = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);

    if (column_found(info[4].colNumber)) {
        edge->reverse_cost = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_cost = default_rcost;
    }

    *valid_edges = edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}

static
void
get_data_5_columns(
        char *sql,
        pgr_edge_t **edges,
        size_t *totalTuples,
        bool ignore_id) {
    const int tuple_limit = 1000000;

    size_t ntuples;
    size_t total_tuples;
    size_t valid_edges;

    Column_info_t info[5];

    int i;
    for (i = 0; i < 5; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = strdup("id");
    info[1].name = strdup("source");
    info[2].name = strdup("target");
    info[3].name = strdup("cost");
    info[4].name = strdup("reverse_cost");

    info[0].strict = !ignore_id;
    info[4].strict = false;

    info[3].eType = ANY_NUMERICAL;
    info[4].eType = ANY_NUMERICAL;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = TRUE;
    (*totalTuples) = total_tuples = valid_edges = 0;


    int64_t default_id = 0;
    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 5);

        ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*edges) == NULL)
                (*edges) = (pgr_edge_t *)palloc0(total_tuples * sizeof(pgr_edge_t));
            else
                (*edges) = (pgr_edge_t *)repalloc((*edges), total_tuples * sizeof(pgr_edge_t));

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            PGR_DBG("processing %d edge tupĺes", ntuples);

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                pgr_fetch_edge(&tuple, &tupdesc, info,
                        &default_id, -1,
                        &(*edges)[total_tuples - ntuples + t],
                        &valid_edges);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }


    if (total_tuples == 0 || valid_edges == 0) {
        (*totalTuples) = 0;
        PGR_DBG("NO edges");
        return;
    }


    (*totalTuples) = total_tuples;
    PGR_DBG("Finish reading %ld edges, %ld", total_tuples, (*totalTuples));
}

void
pgr_get_data_5_columns(
        char *sql,
        pgr_edge_t **edges,
        size_t *totalTuples) {
    bool ignore_id = false;
    get_data_5_columns(sql, edges, totalTuples, ignore_id);
}

void
pgr_get_data_4_columns(
        char *sql,
        pgr_edge_t **edges,
        size_t *totalTuples) {
    bool ignore_id = true;
    get_data_5_columns(sql, edges, totalTuples, ignore_id);
}
