/*PGR-GNU*****************************************************************
File: trsp_pgfetch.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev
Copyright (c) 2020 Mahmoud SAKR and Esteban ZIMANYI
mail: m_attia_sakrcw at yahoo.com, estebanzimanyicw at gmail.com
Copyright (c) 2016 Rohith Reddy
Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli at gmail.com
Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev


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
/** @file

This is a copy of pgdata_fetchers.cpp

The old version of TRSP
- Is the only function that has its own edge reader within its code,
  hence the name of the file.
- The old version of TRSP will exist during 3.x series


Notes
- The only function that should remain after all functions read the data on the C++ code is `fetch_edge`
- The code to be removed is marked with "if 0 ... endif" directives
- The old version of TRSP will be removed on V4

@todo: Remove marked code
@todo: V4 Delete this file
 */

#include "cpp_common/trsp_pgfetch.hpp"

#include <vector>
#include <cfloat>
#include <cmath>

#include "cpp_common/get_check_data.hpp"

#include "c_types/info_t.hpp"
#if 0
#include "c_types/ii_t_rt.h"
#include "c_types/coordinate_t.h"
#include "c_types/delauny_t.h"
#include "c_types/edge_bool_t_rt.h"
#include "c_types/costFlow_t.h"
#include "c_types/edge_xy_t.h"
#endif
#include "c_types/edge_t.h"

#if 0
#include "c_types/iid_t_rt.h"
#include "c_types/pickDeliver/orders_t.h"
#include "c_types/restriction_t.h"
#include "c_types/point_on_edge_t.h"
#include "c_types/pickDeliver/vehicle_t.h"
#endif

namespace pgrouting {
#if 0
void fetch_combination(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        II_t_rt *combination,
        size_t* valid_combinations,
        bool) {
    combination->d1.source = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    combination->d2.target = pgrouting::getBigInt(tuple, tupdesc, info[1]);

    *valid_combinations = *valid_combinations + 1;
}

void fetch_coordinate(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        Coordinate_t* coordinate,
        size_t*,
        bool) {
    if (pgrouting::column_found(info[0].colNumber)) {
        coordinate->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        coordinate->id = *default_id;
        ++(*default_id);
    }
    coordinate->x = pgrouting::getFloat8(tuple, tupdesc, info[1]);
    coordinate->y = pgrouting::getFloat8(tuple, tupdesc, info[2]);
}

void fetch_delauny(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        Delauny_t* delauny,
        size_t*,
        bool) {
    delauny->tid = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    delauny->pid = pgrouting::getBigInt(tuple, tupdesc, info[1]);
    delauny->x = pgrouting::getFloat8(tuple, tupdesc, info[2]);
    delauny->y = pgrouting::getFloat8(tuple, tupdesc, info[3]);
}

/* edges have many fetchers */

void fetch_basic_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        Edge_bool_t *edge,
        size_t *valid_edges,
        bool) {
    if (pgrouting::column_found(info[0].colNumber)) {
        edge->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }
    bool new_columns = pgrouting::column_found(info[5].colNumber);

    edge->source = pgrouting::getBigInt(tuple, tupdesc, info[1]);
    edge->target = pgrouting::getBigInt(tuple, tupdesc, info[2]);

    if (new_columns) {
        edge->going = pgrouting::getFloat8(tuple, tupdesc, info[5]) > 0
            || (pgrouting::column_found(info[6].colNumber)
                    && pgrouting::getFloat8(tuple, tupdesc, info[6]) > 0);
    } else {
        edge->going = pgrouting::getFloat8(tuple, tupdesc, info[3]) > 0
            || (pgrouting::column_found(info[4].colNumber)
                    && pgrouting::getFloat8(tuple, tupdesc, info[4]) > 0);
    }

    (*valid_edges)++;
}
#endif

void fetch_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        Edge_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (pgrouting::column_found(info[0].colNumber)) {
        edge->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    }

    edge->cost = pgrouting::getFloat8(tuple, tupdesc, info[3]);

    if (pgrouting::column_found(info[4].colNumber)) {
        edge->reverse_cost = pgrouting::getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_cost = -1;
    }

    edge->cost = std::isinf(edge->cost)?
        DBL_MAX : edge->cost;

    edge->reverse_cost = std::isinf(edge->reverse_cost)?
        DBL_MAX : edge->reverse_cost;

    *valid_edges = edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}

#if 0
void fetch_costFlow_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        CostFlow_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (pgrouting::column_found(info[0].colNumber)) {
        edge->edge_id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->edge_id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    }

    edge->capacity = pgrouting::getBigInt(tuple, tupdesc, info[3]);
    if (pgrouting::column_found(info[4].colNumber)) {
        edge->reverse_capacity = pgrouting::getBigInt(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_capacity = -1;
    }

    edge->cost = pgrouting::getFloat8(tuple, tupdesc, info[5]);
    if (pgrouting::column_found(info[6].colNumber)) {
        edge->reverse_cost = pgrouting::getFloat8(tuple, tupdesc, info[6]);
    } else {
        edge->reverse_cost = 0;
    }

    *valid_edges = edge->capacity < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_capacity < 0? *valid_edges: *valid_edges + 1;
}

void fetch_edge_with_xy(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        Edge_xy_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (pgrouting::column_found(info[0].colNumber)) {
        edge->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    }
    edge->cost = pgrouting::getFloat8(tuple, tupdesc, info[3]);

    if (pgrouting::column_found(info[4].colNumber)) {
        edge->reverse_cost = pgrouting::getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_cost = -1;
    }

    edge->x1 = pgrouting::getFloat8(tuple, tupdesc, info[5]);
    edge->y1 = pgrouting::getFloat8(tuple, tupdesc, info[6]);
    edge->x2 = pgrouting::getFloat8(tuple, tupdesc, info[7]);
    edge->y2 = pgrouting::getFloat8(tuple, tupdesc, info[8]);

    *valid_edges = edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}

void pgr_fetch_row(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        IID_t_rt *distance,
        size_t*,
        bool) {
    distance->from_vid = pgrouting::getBigInt(tuple, tupdesc,  info[0]);
    distance->to_vid = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
    distance->cost = pgrouting::getFloat8(tuple, tupdesc, info[2]);
}


void fetch_orders(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        Orders_t* pd_order,
        size_t*,
        bool with_id) {
    pd_order->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    pd_order->demand = pgrouting::getFloat8(tuple, tupdesc, info[1]);

    /*
     * the pickups
     */
    pd_order->pick_x = with_id ?
        0 : pgrouting::getFloat8(tuple, tupdesc, info[2]);
    pd_order->pick_y =  with_id ?
        0 : pgrouting::getFloat8(tuple, tupdesc, info[3]);
    pd_order->pick_open_t = pgrouting::getFloat8(tuple, tupdesc, info[4]);
    pd_order->pick_close_t = pgrouting::getFloat8(tuple, tupdesc, info[5]);
    pd_order->pick_service_t = pgrouting::column_found(info[6].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[6]) : 0;

    /*
     * the deliveries
     */
    pd_order->deliver_x =  with_id ?
        0 : pgrouting::getFloat8(tuple, tupdesc, info[7]);
    pd_order->deliver_y =  with_id ?
        0 : pgrouting::getFloat8(tuple, tupdesc, info[8]);
    pd_order->deliver_open_t = pgrouting::getFloat8(tuple, tupdesc, info[9]);
    pd_order->deliver_close_t = pgrouting::getFloat8(tuple, tupdesc, info[10]);
    pd_order->deliver_service_t = pgrouting::column_found(info[11].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[11]) : 0;

    pd_order->pick_node_id = with_id ?
        pgrouting::getBigInt(tuple, tupdesc, info[12]) : 0;
    pd_order->deliver_node_id = with_id ?
        pgrouting::getBigInt(tuple, tupdesc, info[13]) : 0;
}


void fetch_restriction(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        Restriction_t *restriction,
        size_t*,
        bool) {
    restriction->cost = pgrouting::getFloat8(tuple, tupdesc, info[0]);

    restriction->via = NULL;
    restriction->via_size = 0;
    restriction->via = pgrouting::getBigIntArr(tuple, tupdesc, info[1], &restriction->via_size);
}


void fetch_point(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_pid,
        Point_on_edge_t* point,
        size_t*,
        bool) {
    if (pgrouting::column_found(info[0].colNumber)) {
        point->pid = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        ++(*default_pid);
        point->pid = *default_pid;
    }

    point->edge_id = pgrouting::getBigInt(tuple, tupdesc, info[1]);
    point->fraction = pgrouting::getFloat8(tuple, tupdesc, info[2]);

    if (pgrouting::column_found(info[3].colNumber)) {
        point->side = pgrouting::getChar(tuple, tupdesc, info[3], false, 'b');
    } else {
        point->side = 'b';
    }
}

void fetch_vehicle(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        Vehicle_t *vehicle,
        size_t*,
        bool with_id) {
    vehicle->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    vehicle->capacity = pgrouting::getFloat8(tuple, tupdesc, info[1]);

    vehicle->start_x = with_id ?
        0 :
        pgrouting::getFloat8(tuple, tupdesc, info[2]);
    vehicle->start_y = with_id ?
        0 :
        pgrouting::getFloat8(tuple, tupdesc, info[3]);

    vehicle->speed = pgrouting::column_found(info[13].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle->cant_v =  pgrouting::column_found(info[4].colNumber) ?
        pgrouting::getBigInt(tuple, tupdesc, info[4]) :
        1;
    vehicle->start_open_t = pgrouting::column_found(info[5].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[5]) :
        0;
    vehicle->start_close_t = pgrouting::column_found(info[6].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[6]) :
        DBL_MAX;
    vehicle->start_service_t = pgrouting::column_found(info[7].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[7]) :
        0;


    if (!(pgrouting::column_found(info[8].colNumber))
            && pgrouting::column_found(info[9].colNumber)) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[8].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[9].name.c_str(), info[8].name.c_str())));
    }
    if (pgrouting::column_found(info[8].colNumber)
            && !(pgrouting::column_found(info[9].colNumber))) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[9].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[8].name.c_str(), info[9].name.c_str())));
    }

    vehicle->end_x = pgrouting::column_found(info[8].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[8]) :
        vehicle->start_x;
    vehicle->end_y = pgrouting::column_found(info[9].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[9]) :
        vehicle->start_y;

    if (!(pgrouting::column_found(info[10].colNumber))
            && pgrouting::column_found(info[11].colNumber)) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[10].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[10].name.c_str(), info[11].name.c_str())));
    }

    if (pgrouting::column_found(info[10].colNumber)
            && !(pgrouting::column_found(info[11].colNumber))) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[11].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[11].name.c_str(), info[10].name.c_str())));
    }
    vehicle->end_open_t = pgrouting::column_found(info[10].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[10]) :
        vehicle->start_open_t;
    vehicle->end_close_t = pgrouting::column_found(info[11].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[11]) :
        vehicle->start_close_t;
    vehicle->end_service_t = pgrouting::column_found(info[12].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[12]) :
        vehicle->start_service_t;

    vehicle->speed = pgrouting::column_found(info[13].colNumber) ?
        pgrouting::getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle->start_node_id = with_id ?
        pgrouting::getBigInt(tuple, tupdesc, info[14]) :
        0;
    vehicle->end_node_id = with_id ?
        (pgrouting::column_found(info[12].colNumber) ?
            pgrouting::getBigInt(tuple, tupdesc, info[15]) :
            vehicle->start_node_id) :
        0;
}
#endif

}  // namespace pgrouting
