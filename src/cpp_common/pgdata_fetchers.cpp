/*PGR-GNU*****************************************************************
File: pgdata_fetchers.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2024 Celia Virginia Vergara Castillo
- Return (C++) structure
Copyright (c) 2023 Celia Virginia Vergara Castillo
- cat into one file
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

#include "cpp_common/pgdata_fetchers.hpp"

#include <vector>
#include <limits>
#include <cmath>

#include "cpp_common/get_check_data.hpp"

#include "cpp_common/info_t.hpp"
#include "c_types/ii_t_rt.h"
#include "cpp_common/coordinate_t.hpp"
#include "cpp_common/delauny_t.hpp"
#include "c_types/edge_bool_t_rt.h"
#include "cpp_common/costFlow_t.hpp"
#include "cpp_common/edge_xy_t.hpp"
#include "cpp_common/edge_t.hpp"
#include "c_types/iid_t_rt.h"
#include "cpp_common/orders_t.hpp"
#include "cpp_common/restriction_t.hpp"
#include "cpp_common/point_on_edge_t.hpp"
#include "cpp_common/vehicle_t.hpp"


namespace pgrouting {
namespace pgget {

II_t_rt fetch_combination(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        size_t*,
        bool) {
    II_t_rt combination;
    combination.d1.source = getBigInt(tuple, tupdesc, info[0]);
    combination.d2.target = getBigInt(tuple, tupdesc, info[1]);
    return combination;
}

Coordinate_t fetch_coordinate(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        size_t*,
        bool) {
    Coordinate_t coordinate;
    if (column_found(info[0].colNumber)) {
        coordinate.id = getBigInt(tuple, tupdesc, info[0]);
    } else {
        coordinate.id = *default_id;
        ++(*default_id);
    }
    coordinate.x = getFloat8(tuple, tupdesc, info[1]);
    coordinate.y = getFloat8(tuple, tupdesc, info[2]);
    return coordinate;
}

Delauny_t fetch_delauny(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        size_t*,
        bool) {
    Delauny_t delauny;
    delauny.tid = getBigInt(tuple, tupdesc, info[0]);
    delauny.pid = getBigInt(tuple, tupdesc, info[1]);
    delauny.x = getFloat8(tuple, tupdesc, info[2]);
    delauny.y = getFloat8(tuple, tupdesc, info[3]);
    return delauny;
}

/* edges have many fetchers */

Edge_bool_t fetch_basic_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        size_t *valid_edges,
        bool) {
    Edge_bool_t edge;
    if (column_found(info[0].colNumber)) {
        edge.id = getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge.id = *default_id;
        ++(*default_id);
    }
    bool new_columns = column_found(info[5].colNumber);

    edge.source = getBigInt(tuple, tupdesc, info[1]);
    edge.target = getBigInt(tuple, tupdesc, info[2]);

    if (new_columns) {
        edge.going = getFloat8(tuple, tupdesc, info[5]) > 0
            || (column_found(info[6].colNumber)
                    && getFloat8(tuple, tupdesc, info[6]) > 0);
    } else {
        edge.going = getFloat8(tuple, tupdesc, info[3]) > 0
            || (column_found(info[4].colNumber)
                    && getFloat8(tuple, tupdesc, info[4]) > 0);
    }

    (*valid_edges)++;
    return edge;
}

Edge_t fetch_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        size_t *valid_edges,
        bool normal) {
    Edge_t edge;
    if (column_found(info[0].colNumber)) {
        edge.id = getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge.id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge.source = getBigInt(tuple, tupdesc,  info[1]);
        edge.target = getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge.target = getBigInt(tuple, tupdesc,  info[1]);
        edge.source = getBigInt(tuple, tupdesc, info[2]);
    }

    edge.cost = getFloat8(tuple, tupdesc, info[3]);

    if (column_found(info[4].colNumber)) {
        edge.reverse_cost = getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge.reverse_cost = -1;
    }

    edge.cost = std::isinf(edge.cost)?
        std::numeric_limits<double>::max() : edge.cost;

    edge.reverse_cost = std::isinf(edge.reverse_cost)?
        std::numeric_limits<double>::max() : edge.reverse_cost;

    *valid_edges = edge.cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge.reverse_cost < 0? *valid_edges: *valid_edges + 1;

    return edge;
}


CostFlow_t fetch_costFlow_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        size_t *valid_edges,
        bool normal) {
    CostFlow_t edge;
    if (column_found(info[0].colNumber)) {
        edge.edge_id = getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge.edge_id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge.source = getBigInt(tuple, tupdesc,  info[1]);
        edge.target = getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge.target = getBigInt(tuple, tupdesc,  info[1]);
        edge.source = getBigInt(tuple, tupdesc, info[2]);
    }

    edge.capacity = getBigInt(tuple, tupdesc, info[3]);
    if (column_found(info[4].colNumber)) {
        edge.reverse_capacity = getBigInt(tuple, tupdesc, info[4]);
    } else {
        edge.reverse_capacity = -1;
    }

    edge.cost = getFloat8(tuple, tupdesc, info[5]);
    if (column_found(info[6].colNumber)) {
        edge.reverse_cost = getFloat8(tuple, tupdesc, info[6]);
    } else {
        edge.reverse_cost = 0;
    }

    *valid_edges = edge.capacity < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge.reverse_capacity < 0? *valid_edges: *valid_edges + 1;
    return edge;
}

Edge_xy_t fetch_edge_xy(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        size_t *valid_edges,
        bool normal) {
    Edge_xy_t edge;
    if (column_found(info[0].colNumber)) {
        edge.id = getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge.id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge.source = getBigInt(tuple, tupdesc,  info[1]);
        edge.target = getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge.target = getBigInt(tuple, tupdesc,  info[1]);
        edge.source = getBigInt(tuple, tupdesc, info[2]);
    }
    edge.cost = getFloat8(tuple, tupdesc, info[3]);

    if (column_found(info[4].colNumber)) {
        edge.reverse_cost = getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge.reverse_cost = -1;
    }

    edge.x1 = getFloat8(tuple, tupdesc, info[5]);
    edge.y1 = getFloat8(tuple, tupdesc, info[6]);
    edge.x2 = getFloat8(tuple, tupdesc, info[7]);
    edge.y2 = getFloat8(tuple, tupdesc, info[8]);

    *valid_edges = edge.cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge.reverse_cost < 0? *valid_edges: *valid_edges + 1;
    return edge;
}

IID_t_rt pgr_fetch_row(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        size_t*,
        bool) {
    IID_t_rt distance;
    distance.from_vid = getBigInt(tuple, tupdesc,  info[0]);
    distance.to_vid = getBigInt(tuple, tupdesc,  info[1]);
    distance.cost = getFloat8(tuple, tupdesc, info[2]);
    return distance;
}


Orders_t fetch_orders(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        size_t*,
        bool with_id) {
    Orders_t pd_order;
    pd_order.id = getBigInt(tuple, tupdesc, info[0]);
    pd_order.demand = getFloat8(tuple, tupdesc, info[1]);

    /*
     * the pickups
     */
    pd_order.pick_x = with_id ?
        0 : getFloat8(tuple, tupdesc, info[2]);
    pd_order.pick_y =  with_id ?
        0 : getFloat8(tuple, tupdesc, info[3]);
    pd_order.pick_open_t = getFloat8(tuple, tupdesc, info[4]);
    pd_order.pick_close_t = getFloat8(tuple, tupdesc, info[5]);
    pd_order.pick_service_t = column_found(info[6].colNumber) ?
        getFloat8(tuple, tupdesc, info[6]) : 0;

    /*
     * the deliveries
     */
    pd_order.deliver_x =  with_id ?
        0 : getFloat8(tuple, tupdesc, info[7]);
    pd_order.deliver_y =  with_id ?
        0 : getFloat8(tuple, tupdesc, info[8]);
    pd_order.deliver_open_t = getFloat8(tuple, tupdesc, info[9]);
    pd_order.deliver_close_t = getFloat8(tuple, tupdesc, info[10]);
    pd_order.deliver_service_t = column_found(info[11].colNumber) ?
        getFloat8(tuple, tupdesc, info[11]) : 0;

    pd_order.pick_node_id = with_id ?
        getBigInt(tuple, tupdesc, info[12]) : 0;
    pd_order.deliver_node_id = with_id ?
        getBigInt(tuple, tupdesc, info[13]) : 0;
    return pd_order;
}


Restriction_t fetch_restriction(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        size_t*,
        bool) {
    Restriction_t restriction;
    restriction.cost = getFloat8(tuple, tupdesc, info[0]);

    restriction.via = NULL;
    restriction.via_size = 0;
    restriction.via = getBigIntArr(tuple, tupdesc, info[1], &restriction.via_size);
    return restriction;
}


Point_on_edge_t fetch_point(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_pid,
        size_t*,
        bool) {
    Point_on_edge_t point = {};
    if (column_found(info[0].colNumber)) {
        point.pid = getBigInt(tuple, tupdesc, info[0]);
    } else {
        ++(*default_pid);
        point.pid = *default_pid;
    }

    point.edge_id = getBigInt(tuple, tupdesc, info[1]);
    point.fraction = getFloat8(tuple, tupdesc, info[2]);

    if (column_found(info[3].colNumber)) {
        point.side = getChar(tuple, tupdesc, info[3], false, 'b');
    } else {
        point.side = 'b';
    }
    return point;
}

Vehicle_t fetch_vehicle(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t*,
        size_t*,
        bool with_id) {
    Vehicle_t vehicle;
    vehicle.id = getBigInt(tuple, tupdesc, info[0]);
    vehicle.capacity = getFloat8(tuple, tupdesc, info[1]);

    vehicle.start_x = with_id ?
        0 :
        getFloat8(tuple, tupdesc, info[2]);
    vehicle.start_y = with_id ?
        0 :
        getFloat8(tuple, tupdesc, info[3]);

    vehicle.speed = column_found(info[13].colNumber) ?
        getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle.cant_v =  column_found(info[4].colNumber) ?
        getBigInt(tuple, tupdesc, info[4]) :
        1;
    vehicle.start_open_t = column_found(info[5].colNumber) ?
        getFloat8(tuple, tupdesc, info[5]) :
        0;
    vehicle.start_close_t = column_found(info[6].colNumber) ?
        getFloat8(tuple, tupdesc, info[6]) :
        std::numeric_limits<double>::max();
    vehicle.start_service_t = column_found(info[7].colNumber) ?
        getFloat8(tuple, tupdesc, info[7]) :
        0;


    if (!(column_found(info[8].colNumber))
            && column_found(info[9].colNumber)) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[8].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[9].name.c_str(), info[8].name.c_str())));
    }
    if (column_found(info[8].colNumber)
            && !(column_found(info[9].colNumber))) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[9].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[8].name.c_str(), info[9].name.c_str())));
    }

    vehicle.end_x = column_found(info[8].colNumber) ?
        getFloat8(tuple, tupdesc, info[8]) :
        vehicle.start_x;
    vehicle.end_y = column_found(info[9].colNumber) ?
        getFloat8(tuple, tupdesc, info[9]) :
        vehicle.start_y;

    if (!(column_found(info[10].colNumber))
            && column_found(info[11].colNumber)) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[10].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[10].name.c_str(), info[11].name.c_str())));
    }

    if (column_found(info[10].colNumber)
            && !(column_found(info[11].colNumber))) {
        ereport(ERROR,
                (errmsg("Column \'%s\' not Found", info[11].name.c_str()),
                 errhint("%s was found, also column is expected %s ",
                     info[11].name.c_str(), info[10].name.c_str())));
    }
    vehicle.end_open_t = column_found(info[10].colNumber) ?
        getFloat8(tuple, tupdesc, info[10]) :
        vehicle.start_open_t;
    vehicle.end_close_t = column_found(info[11].colNumber) ?
        getFloat8(tuple, tupdesc, info[11]) :
        vehicle.start_close_t;
    vehicle.end_service_t = column_found(info[12].colNumber) ?
        getFloat8(tuple, tupdesc, info[12]) :
        vehicle.start_service_t;

    vehicle.speed = column_found(info[13].colNumber) ?
        getFloat8(tuple, tupdesc, info[13]) :
        1;
    vehicle.start_node_id = with_id ?
        getBigInt(tuple, tupdesc, info[14]) :
        0;
    vehicle.end_node_id = with_id ?
        (column_found(info[12].colNumber) ?
         getBigInt(tuple, tupdesc, info[15]) :
         vehicle.start_node_id) :
        0;
    return vehicle;
}

}  // namespace pgget
}  // namespace pgrouting
