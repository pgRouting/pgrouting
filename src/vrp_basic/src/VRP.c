/*PGR-GNU*****************************************************************

Copyright (c) 2013 Khondoker Md. Razequl Islam
ziboncsedu@gmail.com

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

#include "./VRP.h"
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
// #include <math.h>

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
#include "fmgr.h"




#include "./../../common/src/pgr_types.h"

#undef qsort

// -------------------------------------------------------------------------

/*
 * Define this to have profiling enabled
 */
// #define PROFILE

#ifdef PROFILE
#include <sys/time.h>

struct timeval prof_astar, prof_store, prof_extract, prof_total;
int64_t proftime[5];
int64_t profipts1, profipts2, profopts;

#define profstart(x) do { gettimeofday(&x, NULL); } while (0);
#define profstop(n, x) do { struct timeval _profstop;   \
    int64_t _proftime;                         \
    gettimeofday(&_profstop, NULL);                         \
    _proftime = (_profstop.tv_sec*1000000+_profstop.tv_usec) -     \
    (x.tv_sec*1000000+x.tv_usec); \
    elog(NOTICE, \
            "PRF(%s) %lu (%f ms)", \
            (n), \
            _proftime, _proftime / 1000.0);    \
} while (0);

#else

#define profstart(x) do { } while (0);
#define profstop(n, x) do { } while (0);

#endif  //   PROFILE


//  ------------------------------------------------------------------------

PGDLLEXPORT Datum vrp(PG_FUNCTION_ARGS);

#undef DEBUG
// #define DEBUG 1
#include "../../common/src/debug_macro.h"


//  The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef struct vehicle_columns {
    int vehicle_id;
    int capacity;
} vehicle_columns_t;

typedef struct order_columns {
    int id;
    int order_unit;
    int open_time;
    int close_time;
    int service_time;

    int x;
    int y;
} order_columns_t;

typedef struct distance_columns {
    int src_id;
    int dest_id;
    int cost;
    int distance;
    int traveltime;
} distance_columns_t;


// float DISTANCE[MAX_TOWNS][MAX_TOWNS];
// float x[MAX_TOWNS], y[MAX_TOWNS];

static char *
text2char(text *in) {
    char *out = (char*)palloc(VARSIZE(in));

    memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
    out[VARSIZE(in) - VARHDRSZ] = '\0';
    return out;
}

static int
finish(int *code) {
    *code = SPI_finish();
    if (*code != SPI_OK_FINISH) {
        elog(ERROR, "couldn't disconnect from SPI");
        return -1;
    }
    return 0;
}



static int
fetch_distance_columns(SPITupleTable *tuptable, distance_columns_t *distance_columns) {
    PGR_DBG("Fetching distance");

    distance_columns->src_id = SPI_fnumber(SPI_tuptable->tupdesc, "src_id");
    distance_columns->dest_id = SPI_fnumber(SPI_tuptable->tupdesc, "dest_id");
    distance_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");
    distance_columns->distance = SPI_fnumber(SPI_tuptable->tupdesc, "distance");
    distance_columns->traveltime = SPI_fnumber(SPI_tuptable->tupdesc, "traveltime");
    if (distance_columns->src_id == SPI_ERROR_NOATTRIBUTE ||
            distance_columns->dest_id == SPI_ERROR_NOATTRIBUTE ||
            distance_columns->cost == SPI_ERROR_NOATTRIBUTE ||
            distance_columns->distance == SPI_ERROR_NOATTRIBUTE ||
            distance_columns->traveltime == SPI_ERROR_NOATTRIBUTE) {
        elog(ERROR, "Error, query must return columns "
                "'src_id', 'dest_id', 'cost', 'distance' and 'traveltime'");
        return -1;
    }

    return 0;
}


static void
fetch_distance(HeapTuple *tuple, TupleDesc *tupdesc,
        distance_columns_t *distance_columns, vrp_cost_element_t *dist, size_t t) {
    Datum binval;
    bool isnull;

    PGR_DBG("fetch_distance: src_id col:%i", distance_columns->src_id);

    binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->src_id, &isnull);

    PGR_DBG("back from SPI_getbinval for src_id");
    //  PGR_DBG("binval = %i", binval);

    if (isnull)
        elog(ERROR, "src_id contains a null value");

    dist->src_id = DatumGetInt32(binval);

    PGR_DBG("back from DatumGetInt32");
    PGR_DBG("src_id = %i", dist->src_id);

    binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->dest_id, &isnull);
    if (isnull)
        elog(ERROR, "dest_id contains a null value");

    dist->dest_id = DatumGetInt32(binval);

    PGR_DBG("dest_id = %i", dist->dest_id);

    binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->cost, &isnull);

    if (isnull)
        elog(ERROR, "cost contains a null value");

    dist->cost = DatumGetFloat8(binval);

    PGR_DBG("cost = %lf", dist->cost);

    binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->distance, &isnull);
    if (isnull)
        elog(ERROR, "distance contains a null value");

    dist->distance = DatumGetFloat8(binval);

    PGR_DBG("distance = %lf", dist->distance);

    binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->traveltime, &isnull);

    if (isnull)
        elog(ERROR, "traveltime contains a null value");

    dist->traveltime = DatumGetFloat8(binval);

    PGR_DBG("traveltime = %lf", dist->traveltime);

    // PGR_DBG("dist[%i][%i] = %f\n", from_point, to_point, value);


    // PGR_DBG("dist[%i(%i:%i)][%i(%i:%i)] = %f\n", from, from_order, from_point, to, to_order, to_point, *(dist + (num_rows * from) + to));
}

static int
fetch_order_columns(SPITupleTable *tuptable, order_columns_t *order_columns) {
    PGR_DBG("Fetching order");

    // order_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
    order_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
    order_columns->order_unit = SPI_fnumber(SPI_tuptable->tupdesc, "order_unit");
    order_columns->open_time = SPI_fnumber(SPI_tuptable->tupdesc, "open_time");
    order_columns->close_time = SPI_fnumber(SPI_tuptable->tupdesc, "close_time");
    order_columns->service_time = SPI_fnumber(SPI_tuptable->tupdesc, "service_time");
    order_columns->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
    order_columns->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");
    if (  // order_columns->id == SPI_ERROR_NOATTRIBUTE ||
            order_columns->id == SPI_ERROR_NOATTRIBUTE ||
            order_columns->open_time == SPI_ERROR_NOATTRIBUTE ||
            order_columns->order_unit == SPI_ERROR_NOATTRIBUTE ||
            order_columns->close_time == SPI_ERROR_NOATTRIBUTE ||
            order_columns->service_time == SPI_ERROR_NOATTRIBUTE ||
            order_columns->x == SPI_ERROR_NOATTRIBUTE ||
            order_columns->y == SPI_ERROR_NOATTRIBUTE
       ) {
        //       elog(ERROR, "Error, query must return columns "
        //            "'id', 'order_id', 'pu_time', 'do_time', 'pu_time_window', 'do_time_window', 'from_x', 'to_x', 'from_y', 'to_y' and 'size'");
        elog(ERROR, "Error, query must return columns "
                // "'id',
                "'id', 'order_unit', 'open_time', 'close_time', 'service_time', 'x', 'y'");
        return -1;
    }

    return 0;
}

static void
fetch_order(HeapTuple *tuple, TupleDesc *tupdesc,
        order_columns_t *order_columns, vrp_orders_t *order, size_t t) {
    Datum binval;
    bool isnull;

    PGR_DBG("inside fetch_order\n");

    // binval = SPI_getbinval(*tuple, *tupdesc, order_columns->id, &isnull);
    //
    //  PGR_DBG("got binval\n");
    //
    // if (isnull)
    //  elog(ERROR, "id contains a null value");
    //
    // order->id = DatumGetInt32(binval);
    order->id = (int)t + 1;

    PGR_DBG("id = %i\n", order->id);

    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->id, &isnull);
    if (isnull)
        elog(ERROR, "order_id contains a null value");

    order->id = DatumGetInt32(binval);

    PGR_DBG("order_id = %i\n", order->id);


    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->order_unit, &isnull);
    if (isnull)
        elog(ERROR, "order_unit contains a null value");

    order->order_unit = DatumGetInt32(binval);

    PGR_DBG("order_unit = %i\n", order->order_unit);

    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->open_time, &isnull);
    if (isnull)
        elog(ERROR, "open_time contains a null value");

    order->open_time = DatumGetInt32(binval);

    PGR_DBG("open_time = %i\n", order->open_time);

    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->close_time, &isnull);
    if (isnull)
        elog(ERROR, "close_time contains a null value");

    order->close_time = DatumGetInt32(binval);

    PGR_DBG("close_time = %d\n", order->close_time);

    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->service_time, &isnull);
    if (isnull)
        elog(ERROR, "service_time contains a null value");

    order->service_time = DatumGetInt32(binval);

    PGR_DBG("service_time = %d\n", order->service_time);

    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->x, &isnull);
    if (isnull)
        elog(ERROR, "x contains a null value");

    order->x = DatumGetFloat8(binval);

    PGR_DBG("x = %f\n", order->x);

    binval = SPI_getbinval(*tuple, *tupdesc, order_columns->y, &isnull);
    if (isnull)
        elog(ERROR, "y contains a null value");

    order->y = DatumGetFloat8(binval);

    PGR_DBG("doUT = %f\n", order->y);
}

static int
fetch_vehicle_columns(SPITupleTable *tuptable, vehicle_columns_t *vehicle_columns) {
    PGR_DBG("Fetching order");

    vehicle_columns->vehicle_id = SPI_fnumber(SPI_tuptable->tupdesc, "vehicle_id");
    vehicle_columns->capacity = SPI_fnumber(SPI_tuptable->tupdesc, "capacity");

    if (vehicle_columns->vehicle_id == SPI_ERROR_NOATTRIBUTE ||
            vehicle_columns->capacity == SPI_ERROR_NOATTRIBUTE) {
        elog(ERROR, "Error, query must return columns "
                "'id' and 'capacity'");
        return -1;
    }

    return 0;
}

static void
fetch_vehicle(HeapTuple *tuple, TupleDesc *tupdesc,
        vehicle_columns_t *vehicle_columns, vrp_vehicles_t *vehicle, size_t t) {
    Datum binval;
    bool isnull;

    PGR_DBG("inside fetch_vehicle\n");

    // binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->id, &isnull);
    // PGR_DBG("Got id\n");
    //
    // if (isnull)
    //  elog(ERROR, "id contains a null value");
    //
    // vehicle->id = DatumGetInt32(binval);


    binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->vehicle_id, &isnull);
    PGR_DBG("Got vehicle_id\n");

    if (isnull)
        elog(ERROR, "vehicle_id contains a null value");

    vehicle->id = DatumGetInt32(binval);

    PGR_DBG("vehicle_id = %i\n", vehicle->id);

    binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->capacity, &isnull);
    if (isnull)
        elog(ERROR, "capacity contains a null value");

    vehicle->capacity = DatumGetInt32(binval);

    PGR_DBG("capacity = %d\n", vehicle->capacity);
}

static int conn(int *SPIcode) {
    int res = 0;

    *SPIcode = SPI_connect();

    if (*SPIcode != SPI_OK_CONNECT) {
        elog(ERROR, "vrp: couldn't open a connection to SPI");
        res = -1;
    }

    return res;
}

static int prepare_query(Portal *SPIportal, char* sql) {
    int res = 0;

    void* SPIplan = SPI_prepare(sql, 0, NULL);

    if (SPIplan == NULL) {
        elog(ERROR, "vrp: couldn't create query plan via SPI");
        res = -1;
    }

    if ((*SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
        elog(ERROR, "vrp: SPI_cursor_open('%s') returns NULL", sql);
        res = -1;
    }

    return res;
}

static int solve_vrp(char* orders_sql, char* vehicles_sql,
        char* dist_sql,
        int depot,
        vrp_result_element_t** path, size_t *path_count) {
    int SPIcode;

    Portal SPIportal_o;
    Portal SPIportal_v;
    Portal SPIportal_d;
    //  Portal SPIportal_p;

    bool moredata = TRUE;
    size_t ntuples;

    size_t order_num;
    size_t vehicle_num;
    size_t dist_num;

    vrp_vehicles_t *vehicles = NULL;
    vehicle_columns_t vehicle_columns = {.vehicle_id = -1, .capacity = -1};

    vrp_orders_t *orders = NULL;
    order_columns_t order_columns = {.id = -1, .order_unit = -1, .open_time = -1, .close_time = -1, .service_time = -1, .x = -1, .y = -1};

    vrp_cost_element_t *costs = NULL;
    distance_columns_t distance_columns = {.src_id = -1, .dest_id = -1, .cost = -1, .distance = -1, .traveltime = -1};

    char *err_msg = NULL;
    int ret = -1;

    //  int   z = 0;

    //  int    tt, cc;
    //  double dx, dy;
    //  float fit = 0.0;

    int prep = -1, con = -1;

    // int total_tuples = 0;
    order_num = 0;
    vehicle_num = 0;

    PGR_DBG("start solve_vrp\n");

    // vrp_orders_t depot_ord = {id:0, order_id:depot, from:depot_point, to:depot_point};
    // orders = palloc(1 * sizeof(vrp_orders_t));
    // orders[0] = depot_ord;

    con = conn(&SPIcode);

    if (con < 0)
        return ret;


    //  Fetching orders

    PGR_DBG("calling prepare_query for orders_sql");

    prep = prepare_query(&SPIportal_o, orders_sql);

    if (prep < 0)
        return ret;

    PGR_DBG("Query: %s\n", orders_sql);
    PGR_DBG("Query executed\n");

    PGR_DBG("Orders before: %lu\n", order_num);

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal_o, TRUE, TUPLIMIT);

        PGR_DBG("cursor fetched\n");

        if (order_columns.id == -1) {
            if (fetch_order_columns(SPI_tuptable, &order_columns) == -1)
                return finish(&SPIcode);
        }

        ntuples = SPI_processed;

        order_num += ntuples;

        PGR_DBG("Tuples: %lu\n", order_num);

        if (!orders)
            orders = palloc(order_num * sizeof(vrp_orders_t));
        else
            orders = repalloc(orders, (order_num + 1) * sizeof(vrp_orders_t));

        if (orders == NULL) {
            elog(ERROR, "Out of memory");
            return finish(&SPIcode);
        }

        if (ntuples > 0) {
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            PGR_DBG("Got tuple desc\n");
            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                // PGR_DBG("Before order fetched [%i]\n", order_num - ntuples + t);
                fetch_order(&tuple, &tupdesc, &order_columns,
                        &orders[order_num - ntuples + t], t);

                // &orders[t+1], t);
                PGR_DBG("Order fetched\n");
            }

            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }  //  end of fetching orders
    // finish(&SPIcode_o);
    /*
       int o;
       for (o = 0; o<order_num+1;++o)
       {
       elog(NOTICE, "ORDERS[%i] = {id = %i, open = %i, close = %i, service = %i}", o, orders[o].id, orders[o].open_time, orders[o].close_time, orders[o].service_time);
       }
       */
    PGR_DBG("order_num = %lu", order_num);

    // qsort (orders, order_num+1, sizeof (vrp_orders_t), order_cmp);


    //  Fetching vehicles

    moredata = TRUE;
    prep = prepare_query(&SPIportal_v, vehicles_sql);

    if (prep < 0)
        return ret;

    PGR_DBG("Query: %s\n", vehicles_sql);
    PGR_DBG("Query executed\n");


    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal_v, TRUE, TUPLIMIT);

        if (vehicle_columns.vehicle_id == -1) {
            if (fetch_vehicle_columns(SPI_tuptable, &vehicle_columns) == -1)
                return finish(&SPIcode);
        }


        ntuples = SPI_processed;

        vehicle_num += ntuples;

        PGR_DBG("Tuples: %lu\n", vehicle_num);

        if (!vehicles)
            vehicles = palloc(vehicle_num * sizeof(vrp_vehicles_t));
        else
            vehicles = repalloc(vehicles, vehicle_num * sizeof(vrp_vehicles_t));

        if (vehicles == NULL) {
            elog(ERROR, "Out of memory");
            return finish(&SPIcode);
        }

        if (ntuples > 0) {
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            PGR_DBG("Got tuple desc\n");

            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                PGR_DBG("Before vehicle fetched\n");
                fetch_vehicle(&tuple, &tupdesc, &vehicle_columns,
                        &vehicles[vehicle_num - ntuples + t], t);
                PGR_DBG("Vehicle fetched\n");
            }

            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }  //  end of fetching vehicles
    // finish(&SPIcode_v);

    // double dist[order_num*2+1][order_num*2+1];

    //  Fetching distances

    dist_num = 0;
    moredata = TRUE;
    prep = prepare_query(&SPIportal_d, dist_sql);

    if (prep < 0)
        return ret;

    PGR_DBG("Query: %s\n", dist_sql);
    PGR_DBG("Query executed\n");

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal_d, TRUE, TUPLIMIT);

        if (distance_columns.src_id == -1) {
            if (fetch_distance_columns(SPI_tuptable, &distance_columns) == -1)
                return finish(&SPIcode);
        }

        ntuples = SPI_processed;
        dist_num += ntuples;

        PGR_DBG("Tuples: %lu\n", vehicle_num);

        if (!costs)
            costs = palloc(dist_num * sizeof(vrp_cost_element_t));
        else
            costs = repalloc(costs, dist_num * sizeof(vrp_cost_element_t));

        if (costs == NULL) {
            elog(ERROR, "Out of memory");
            return finish(&SPIcode);
        }

        if (ntuples > 0) {
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            PGR_DBG("Got tuple desc\n");
            size_t t;
            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                PGR_DBG("Before distance fetched\n");
                fetch_distance(&tuple, &tupdesc, &distance_columns,
                        &costs[dist_num - ntuples + t], t);
                PGR_DBG("Distance fetched\n");
            }

            SPI_freetuptable(tuptable);
        } else {
            moredata = FALSE;
        }
    }  //  end of fetching distances


    PGR_DBG("Calling vrp\n");

    profstop("extract", prof_extract);
    profstart(prof_vrp);

    PGR_DBG("Total orders: %lu\n", order_num);
    PGR_DBG("Total vehicles: %lu\n", vehicle_num);


    // qsort (orders, order_num+1, sizeof (vrp_orders_t), order_cmp_asc);


#ifdef DEBUG
    int o;
    for (o = 0; o < order_num + 1; ++o) {
        PGR_DBG("ORDERS[%i] = {id = %i, open = %i, close = %i, service = %i}", o, orders[o].id, orders[o].open_time, orders[o].close_time, orders[o].service_time);
    }
#endif



    // itinerary = (vrp_result_element_t *)palloc(sizeof(vrp_result_element_t)*(order_num*2-1)*vehicle_num);

    PGR_DBG("Calling vrp solver\n");
    // elog(NOTICE, "Calling find_vrp_solution: vehicles: %i, orders: %i, dists: %i, depot: %i", vehicle_num, order_num, dist_num, depot);

    ret = find_vrp_solution(vehicles, vehicle_num,
            orders, order_num,
            costs, dist_num,
            depot,
            path, path_count, &err_msg);

    // ret = -1;
    // elog(NOTICE, "vrp solved! ret: %d, path_count: %d", ret, *path_count);
    //  int pp;
    /*
       for (pp = 0; pp < *path_count; pp++)
       {
       elog(NOTICE, "Row: %d: %d %d %d %d %d", pp, (*path)[pp].order_id, (*path)[pp].order_pos, (*path)[pp].vehicle_id, (*path)[pp].arrival_time, (*path)[pp].depart_time);
       }
       */
    PGR_DBG("vrp solved! ret: %d, path_count: %lu", ret, *path_count);
    // PGR_DBG("Score: %f\n", fit);

    profstop("vrp", prof_vrp);
    profstart(prof_store);

    PGR_DBG("Profile changed and ret is %i", ret);

    if (ret < 0) {
        // elog(ERROR, "Error computing path: %s", err_msg);
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), errmsg("Error computing path: %s", err_msg)));
    }

    // pfree(vehicles);
    // pfree(orders);
    return finish(&SPIcode);
}

PG_FUNCTION_INFO_V1(vrp);
PGDLLEXPORT Datum
vrp(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t                  call_cntr;
    uint32_t                  max_calls;
    TupleDesc            tuple_desc;
    vrp_result_element_t         *path;

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        // int path_count;
        //  int ret = -1;
        size_t path_count = 0;

        //  XXX profiling messages are not thread safe
        profstart(prof_total);
        profstart(prof_extract);

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        // path = (vrp_result_element_t *)palloc(sizeof(vrp_result_element_t)*(MAX_ORDERS-1)*2*MAX_VEHICLES);


        PGR_DBG("Calling solve_vrp ...");

        //  ret =
        solve_vrp(  // text2char(PG_GETARG_TEXT_P(0)),  //  points sql
                text2char(PG_GETARG_TEXT_P(0)),  //   orders sql
                text2char(PG_GETARG_TEXT_P(1)),  //   vehicles sql
                text2char(PG_GETARG_TEXT_P(2)),  //   distances query
                PG_GETARG_INT32(3),  //   depot id
                &path, &path_count);

        PGR_DBG("Back from solve_vrp, path_count:%lu", path_count);
        // elog(NOTICE, "Back from solve_vrp, path_count:%d", path_count);

        /* total number of tuples to be returned */
        // PGR_DBG("Counting tuples number\n");

        funcctx->max_calls = (uint32_t)path_count;

        funcctx->user_fctx = path;

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = BlessTupleDesc(tuple_desc);

        /*
         * generate attribute metadata needed later to produce tuples from raw
         * C strings
         */
        // attinmeta = TupleDescGetAttInMetadata(tuple_desc);
        // funcctx->attinmeta = attinmeta;

        MemoryContextSwitchTo(oldcontext);
        // elog(NOTICE, "table formed");
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;

    path = (vrp_result_element_t *)funcctx->user_fctx;

    // elog(NOTICE, "Point 1");
    // PGR_DBG("Trying to allocate some memory\n");
    // PGR_DBG("call_cntr = %i, max_calls = %i\n", call_cntr, max_calls);

    if (call_cntr < max_calls) {
        /* do when there is more left to send */
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        char* nulls;

        values = palloc(5 * sizeof(Datum));
        nulls = palloc(5 * sizeof(char));

        values[0] = Int32GetDatum(path[call_cntr].order_id);   //  order id
        nulls[0] = ' ';
        values[1] = Int32GetDatum(path[call_cntr].order_pos);  //   order pos
        nulls[1] = ' ';
        values[2] = Int32GetDatum(path[call_cntr].vehicle_id);  //   vehicle id
        nulls[2] = ' ';
        values[3] = Int32GetDatum(path[call_cntr].arrival_time);  //   arrival time
        nulls[3] = ' ';
        // values[4] = TimeTzADTPGetDatum(&path[call_cntr].time);
        values[4] = Int32GetDatum(path[call_cntr].depart_time);  //   departure time
        nulls[4] = ' ';

        //  PGR_DBG("Heap making\n");
        // elog(NOTICE, "Result %d %d %d", call_cntr, path[call_cntr].order_id, max_calls);
        tuple = heap_form_tuple(tuple_desc, values, nulls);

        // PGR_DBG("Datum making\n");

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        // PGR_DBG("Trying to free some memory\n");

        /* clean up */
        pfree(values);
        pfree(nulls);


        SRF_RETURN_NEXT(funcctx, result);
    } else {
        /* do when there is no more left */

        PGR_DBG("Ending function\n");
        profstop("store", prof_store);
        profstop("total", prof_total);
        PGR_DBG("Profiles stopped\n");

        free(path);

        PGR_DBG("Itinerary cleared\n");

        SRF_RETURN_DONE(funcctx);
    }
}
