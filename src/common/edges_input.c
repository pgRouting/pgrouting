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

#include "c_common/edges_input.h"

#include <math.h>
#include <float.h>
#include <limits.h>
/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
#else
#   include <stddef.h>
#endif

#include "c_types/column_info_t.h"
#include "c_types/edge_bool_t.h"
#include "c_types/costFlow_t.h"
#include "c_types/edge_xy_t.h"

#include "c_common/debug_macro.h"
#include "c_common/get_check_data.h"
#include "c_common/time_msg.h"

#include "c_types/flow_t.h"
#include "c_types/edge_t.h"

/*
 * Only for undirected graph, without weights on the costs
 * going: true when cost or reverse_cost is non negative
*/
static
void fetch_basic_edge(
    HeapTuple *tuple,
    TupleDesc *tupdesc,
    Column_info_t info[7],
    Edge_bool_t *edge,
    size_t *valid_edges) {

    edge->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    edge->source = pgr_SPI_getBigInt(tuple, tupdesc, info[1]);
    edge->target = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);

    edge->going = pgr_SPI_getFloat8(tuple, tupdesc, info[3]) > 0
        || ((column_found(info[4].colNumber) && pgr_SPI_getFloat8(tuple, tupdesc, info[4]) > 0));

    (*valid_edges)++;
}

static
void fetch_edge(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[5],
        int64_t *default_id,
        float8 default_rcost,
        Edge_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (column_found(info[0].colNumber)) {
        edge->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    }

    edge->cost = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);

    if (column_found(info[4].colNumber)) {
        edge->reverse_cost = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_cost = default_rcost;
    }

    edge->cost = isinf(edge->cost)?
        DBL_MAX : edge->cost;

    edge->reverse_cost = isinf(edge->reverse_cost)?
        DBL_MAX : edge->reverse_cost;

    *valid_edges = edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}

static
void fetch_costFlow_edge(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[7],
        int64_t *default_id,
        int64_t default_rcapacity,
        float8 default_rcost,
        CostFlow_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (column_found(info[0].colNumber)) {
        edge->edge_id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->edge_id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    }

    edge->capacity = pgr_SPI_getBigInt(tuple, tupdesc, info[3]);
    if (column_found(info[4].colNumber)) {
        edge->reverse_capacity = pgr_SPI_getBigInt(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_capacity = default_rcapacity;
    }

    edge->cost = pgr_SPI_getFloat8(tuple, tupdesc, info[5]);
    if (column_found(info[6].colNumber)) {
        edge->reverse_cost = pgr_SPI_getFloat8(tuple, tupdesc, info[6]);
    } else {
        edge->reverse_cost = default_rcost;
    }

    *valid_edges = edge->capacity < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_capacity < 0? *valid_edges: *valid_edges + 1;
}

static
void fetch_edge_with_xy(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info[9],
        int64_t *default_id,
        float8 default_rcost,
        Edge_xy_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (column_found(info[0].colNumber)) {
        edge->id = pgr_SPI_getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgr_SPI_getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgr_SPI_getBigInt(tuple, tupdesc, info[2]);
    }
    edge->cost = pgr_SPI_getFloat8(tuple, tupdesc, info[3]);

    if (column_found(info[4].colNumber)) {
        edge->reverse_cost = pgr_SPI_getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_cost = default_rcost;
    }

    edge->x1 = pgr_SPI_getFloat8(tuple, tupdesc, info[5]);
    edge->y1 = pgr_SPI_getFloat8(tuple, tupdesc, info[6]);
    edge->x2 = pgr_SPI_getFloat8(tuple, tupdesc, info[7]);
    edge->y2 = pgr_SPI_getFloat8(tuple, tupdesc, info[8]);
#if 0
    PGR_DBG("%ld", edge->id);
    PGR_DBG("x1=%f y1=%.15lf", edge->x1, edge->y1);
    PGR_DBG("x2=%f y2=%.15lf", edge->x2, edge->y2);
#endif


    *valid_edges = edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}

static
void
get_edges_9_columns(
        char *sql,
        Edge_xy_t **edges,
        size_t *total_edges,
        bool normal) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples;
    size_t valid_edges;

    Column_info_t info[9];

    info[0].name = "id";
    info[1].name = "source";
    info[2].name = "target";
    info[3].name = "cost";
    info[4].name = "reverse_cost";
    info[5].name = "x1";
    info[6].name = "y1";
    info[7].name = "x2";
    info[8].name = "y2";

    int i;
    for (i = 0; i < 3; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    for (i = 3; i < 9; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_NUMERICAL;
    }
    /*
     * reverse_cost is optional
     */
    info[4].strict = false;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*total_edges) = total_tuples = valid_edges = 0;


    int64_t default_id = 0;
    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 9);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*edges) == NULL)
                (*edges) = (Edge_xy_t *)
                    palloc0(total_tuples * sizeof(Edge_xy_t));
            else
                (*edges) = (Edge_xy_t *)
                    repalloc((*edges), total_tuples * sizeof(Edge_xy_t));

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_edge_with_xy(&tuple, &tupdesc, info,
                        &default_id, -1,
                        &(*edges)[total_tuples - ntuples + t],
                        &valid_edges, normal);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0 || valid_edges == 0) {
        PGR_DBG("No edges found");
    }

    (*total_edges) = total_tuples;
    time_msg("reading edges", start_t, clock());
}



static
void
get_edges_5_columns(
        char *sql,
        Edge_t **edges,
        size_t *totalTuples,
        bool ignore_id,
        bool normal) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

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
    info[0].name = "id";
    info[1].name = "source";
    info[2].name = "target";
    info[3].name = "cost";
    info[4].name = "reverse_cost";

    info[0].strict = !ignore_id;
    info[4].strict = false;

    info[3].eType = ANY_NUMERICAL;
    info[4].eType = ANY_NUMERICAL;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*totalTuples) = total_tuples = valid_edges = 0;


    int64_t default_id = 0;
    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 5);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*edges) == NULL)
                (*edges) = (Edge_t *)
                    palloc0(total_tuples * sizeof(Edge_t));
            else
                (*edges) = (Edge_t *)
                    repalloc((*edges), total_tuples * sizeof(Edge_t));

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_edge(&tuple, &tupdesc, info,
                        &default_id, -1,
                        &(*edges)[total_tuples - ntuples + t],
                        &valid_edges,
                        normal);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0 || valid_edges == 0) {
        PGR_DBG("No edges found");
    }

    (*totalTuples) = total_tuples;
    time_msg("reading edges", start_t, clock());
}

static
void
get_edges_flow(
    char *sql,
    Edge_t **edges,
    size_t *totalTuples,
    bool ignore_id) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

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
    info[0].name = "id";
    info[1].name = "source";
    info[2].name = "target";
    info[3].name = "capacity";
    info[4].name = "reverse_capacity";

    info[0].strict = !ignore_id;
    info[4].strict = false;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*totalTuples) = total_tuples = valid_edges = 0;


    int64_t default_id = 0;
    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 5);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*edges) == NULL)
                (*edges) = (Edge_t *)
                    palloc0(total_tuples * sizeof(Flow_t));
            else
                (*edges) = (Edge_t *)
                    repalloc((*edges), total_tuples * sizeof(Flow_t));

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_edge(&tuple, &tupdesc, info,
                           &default_id, -1,
                           &(*edges)[total_tuples - ntuples + t],
                           &valid_edges,
                           true);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0 || valid_edges == 0) {
        PGR_DBG("No edges found");
    }

    (*totalTuples) = total_tuples;
    time_msg("Reading edges", start_t, clock());
}

static
void
get_edges_costFlow(
    char *sql,
    CostFlow_t **edges,
    size_t *totalTuples,
    bool ignore_id) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples;
    size_t valid_edges;

    Column_info_t info[7];

    int i;
    for (i = 0; i < 5; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = "id";
    info[1].name = "source";
    info[2].name = "target";
    info[3].name = "capacity";
    info[4].name = "reverse_capacity";
    info[5].name = "cost";
    info[6].name = "reverse_cost";

    info[0].strict = !ignore_id;
    info[4].strict = false;
    info[6].strict = false;

    info[5].eType = ANY_NUMERICAL;
    info[6].eType = ANY_NUMERICAL;

    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*totalTuples) = total_tuples = valid_edges = 0;


    int64_t default_id = 0;
    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 7);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*edges) == NULL)
                (*edges) = (CostFlow_t *)
                    palloc0(total_tuples * sizeof(CostFlow_t));
            else
                (*edges) = (CostFlow_t *)
                    repalloc((*edges), total_tuples * sizeof(CostFlow_t));

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_costFlow_edge(&tuple, &tupdesc, info,
                                    &default_id, -1, 0,
                                    &(*edges)[total_tuples - ntuples + t],
                                    &valid_edges,
                                    true);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0 || valid_edges == 0) {
        PGR_DBG("No edges found");
    }

    (*totalTuples) = total_tuples;
    time_msg("reading edges", start_t, clock());
}

static
void
get_edges_basic(
    char *sql,
    Edge_bool_t **edges,
    size_t *totalTuples) {
    clock_t start_t = clock();

    const int tuple_limit = 1000000;

    size_t total_tuples;
    size_t valid_edges;

    Column_info_t info[7];

    int i;
    for (i = 0; i < 5; ++i) {
        info[i].colNumber = -1;
        info[i].type = 0;
        info[i].strict = true;
        info[i].eType = ANY_INTEGER;
    }
    info[0].name = "id";
    info[1].name = "source";
    info[2].name = "target";
    info[3].name = "cost";
    info[4].name = "reverse_cost";

    info[4].strict = false;

    info[3].eType = ANY_NUMERICAL;
    info[4].eType = ANY_NUMERICAL;


    void *SPIplan;
    SPIplan = pgr_SPI_prepare(sql);

    Portal SPIportal;
    SPIportal = pgr_SPI_cursor_open(SPIplan);


    bool moredata = true;
    (*totalTuples) = total_tuples = valid_edges = 0;


    while (moredata == true) {
        SPI_cursor_fetch(SPIportal, true, tuple_limit);
        if (total_tuples == 0)
            pgr_fetch_column_info(info, 5);

        size_t ntuples = SPI_processed;
        total_tuples += ntuples;

        if (ntuples > 0) {
            if ((*edges) == NULL)
                (*edges) = (Edge_bool_t *)palloc0(
                        total_tuples * sizeof(Edge_bool_t));
            else
                (*edges) = (Edge_bool_t *)repalloc(
                        (*edges), total_tuples * sizeof(Edge_bool_t));

            if ((*edges) == NULL) {
                elog(ERROR, "Out of memory");
            }

            size_t t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_basic_edge(&tuple, &tupdesc, info,
                           &(*edges)[total_tuples - ntuples + t],
                           &valid_edges);
            }
            SPI_freetuptable(tuptable);
        } else {
            moredata = false;
        }
    }

    SPI_cursor_close(SPIportal);

    if (total_tuples == 0 || valid_edges == 0) {
        PGR_DBG("No edges found");
    }

    (*totalTuples) = total_tuples;
    time_msg("reading edges", start_t, clock());
}

/* select id, source, target, capacity, reverse_capacity */
void
pgr_get_flow_edges(
    char *sql,
    Edge_t **edges,
    size_t *total_edges) {
    bool ignore_id = false;
    get_edges_flow(sql, edges, total_edges, ignore_id);
}

/* select id, source, target, capacity, reverse_capacity, cost, reverse_cost */
void
pgr_get_costFlow_edges(
    char *sql,
    CostFlow_t **edges,
    size_t *total_edges) {
    bool ignore_id = false;
    get_edges_costFlow(sql, edges, total_edges, ignore_id);
}

/* select id, source, target, cost, reverse_cost */
void
pgr_get_edges(
        char *edges_sql,
        Edge_t **edges,
        size_t *total_edges) {
    bool ignore_id = false;
    bool normal = true;
    get_edges_5_columns(edges_sql, edges, total_edges, ignore_id, normal);
}

/* select id, source AS target, target AS source, cost, reverse_cost */
void
pgr_get_edges_reversed(
        char *edges_sql,
        Edge_t **edges,
        size_t *total_edges) {
    bool ignore_id = false;
    bool normal = false;
    get_edges_5_columns(edges_sql, edges, total_edges, ignore_id, normal);
}

/* select source, target, cost, reverse_cost */
void
pgr_get_edges_no_id(
        char *edges_sql,
        Edge_t **edges,
        size_t *total_edges) {
    bool ignore_id = true;
    bool normal = true;
    get_edges_5_columns(edges_sql, edges, total_edges, ignore_id, normal);
}

/* select id, source, target, cost, reverse_cost, x1, y1, x2, y2 */
void
pgr_get_edges_xy(
        char *edges_sql,
        Edge_xy_t **edges,
        size_t *total_edges) {
    get_edges_9_columns(edges_sql, edges, total_edges, true);
}

/* select id,
 * source AS target,
 * target AS source,
 * cost, reverse_cost,
 * x1, y1, x2, y2 */
void
pgr_get_edges_xy_reversed(
        char *edges_sql,
        Edge_xy_t **edges,
        size_t *total_edges) {
    get_edges_9_columns(edges_sql, edges, total_edges, false);
}

/* used in flow algorithms */
void
pgr_get_basic_edges(
        char *sql,
        Edge_bool_t **edges,
        size_t *total_edges) {
    get_edges_basic(sql, edges, total_edges);
}
