/*PGR-GNU*****************************************************************
File: pgdata_getters.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2024 Celia Virginia Vergara Castillo
- Return C++ container
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

extern "C" {

#include <postgres.h>
#include <executor/spi.h>
#include <funcapi.h>
#include <utils/builtins.h>
#include <access/htup_details.h>
#include <fmgr.h>
#include <utils/array.h>
#include <catalog/pg_type.h>

}

#include "cpp_common/pgdata_getters.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>
#include <sstream>
#include <set>

#include "cpp_common/get_data.hpp"
#include "cpp_common/get_check_data.hpp"
#include "cpp_common/pgdata_fetchers.hpp"

#include "cpp_common/info_t.hpp"
#include "cpp_common/coordinate_t.hpp"
#include "cpp_common/costFlow_t.hpp"
#include "c_types/ii_t_rt.h"
#include "c_types/iid_t_rt.h"
#include "cpp_common/delauny_t.hpp"
#include "cpp_common/edge_t.hpp"
#include "c_types/edge_bool_t_rt.h"
#include "cpp_common/edge_xy_t.hpp"
#include "cpp_common/orders_t.hpp"
#include "cpp_common/vehicle_t.hpp"
#include "cpp_common/point_on_edge_t.hpp"
#include "cpp_common/restriction_t.hpp"

namespace pgrouting {
namespace pgget {

/**
 * @param[in] input the postgres array
 * @param[in] allow_empty when true, empty arrays are accepted.
 * @returns Returns a vector of int64_t
 */
std::vector<int64_t> get_intArray(ArrayType *input, bool allow_empty) {
    return pgrouting::get_pgarray(input, allow_empty);
}

/**
 * @param[in] arr 1 dimension postgres array
 * @returns Returns a set of int64_t
 *
 * Can be empty set.
 */
std::set<int64_t> get_intSet(ArrayType *arr) {
    return pgrouting::get_pgset(arr);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT source, target FROM combinations;
  ~~~~

  @param[in] sql The combinations query
  @returns vector of `II_t_rt` combinations
  */
std::vector<II_t_rt> get_combinations(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "source", ANY_INTEGER},
    {-1, 0, true, "target", ANY_INTEGER}};

    return get_data<II_t_rt>(sql, true, info, &fetch_combination);
}

/** @details

  For queries of the type:
  ~~~~{.c}
  SELECT id, x, y FROM coordinates;
  ~~~~

  @param[in] sql The coordinates query
  @returns vector of `Coordinate_t`
  */
std::vector<Coordinate_t> get_coordinates(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "x", ANY_NUMERICAL},
    {-1, 0, true, "y", ANY_NUMERICAL}};

    return get_data<Coordinate_t>(sql, true, info, &fetch_coordinate);
}


/** @details

  For queries of the type:
  ~~~~{.c}
  SELECT tid, pid, x, y FROM delauny;
  SELECT pid, pid, x, y FROM delauny;
  ~~~~

  @param[in] sql The delauny triangles query
  @returns vector of `Delauny_t`
  */
std::vector<Delauny_t> get_delauny(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "tid", ANY_INTEGER},
    {-1, 0, true, "pid", ANY_INTEGER},
    {-1, 0, true, "x", ANY_NUMERICAL},
    {-1, 0, true, "y", ANY_NUMERICAL}};

    return get_data<Delauny_t>(sql, true, info, &fetch_delauny);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, capacity, [reverse_capacity] FROM edge_table;
  ~~~~

  @param[in] sql The flow edges query
  @returns vector of `Edge_t`
  */
std::vector<Edge_t> get_flow_edges(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "source", ANY_INTEGER},
    {-1, 0, true, "target", ANY_INTEGER},
    {-1, 0, true, "capacity", ANY_INTEGER},
    {-1, 0, false, "reverse_capacity", ANY_INTEGER}};

    return get_data<Edge_t>(sql, true, info, &fetch_edge);
}

/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, capacity, [reverse_capacity], cost, [reverse_cost]
  FROM edge_table;
  ~~~~

  @param[in] sql The query
  @returns vector of `CostFlow_t` edges
  */
std::vector<CostFlow_t> get_costFlow_edges(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "source", ANY_INTEGER},
    {-1, 0, true, "target", ANY_INTEGER},
    {-1, 0, true, "capacity", ANY_INTEGER},
    {-1, 0, false, "reverse_capacity", ANY_INTEGER},
    {-1, 0, true, "cost", ANY_NUMERICAL},
    {-1, 0, false, "reverse_cost", ANY_NUMERICAL}};

    return get_data<CostFlow_t>(sql, true, info, &fetch_costFlow_edge);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost] FROM edge_table;
  ~~~~

  For backward compatibility it also accepts:
  ~~~~{.c}
  SELECT id, source, target, going, [coming] FROM edge_table;
  ~~~~
  @todo This option will be removed on V4

  @param[in] sql The query
  @returns vector of `Edge_bool_t`
  */
std::vector<Edge_bool_t> get_basic_edges(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "source", ANY_INTEGER},
    {-1, 0, true, "target", ANY_INTEGER},
    {-1, 0, false, "going", ANY_NUMERICAL},
    {-1, 0, false, "coming", ANY_NUMERICAL},
    {-1, 0, false, "cost", ANY_NUMERICAL},
    {-1, 0, false, "reverse_cost", ANY_NUMERICAL}};

    return get_data<Edge_bool_t>(sql, true, info, &fetch_basic_edge);
}

/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost], x1, y1, x2, y2 FROM edge_table;
  ~~~~

  @param[in] sql The edges query
  @param[in] normal when false then the graph is reversed
  @returns vector of `Edge_xy_t`
  */
std::vector<Edge_xy_t> get_edges_xy(const std::string &sql, bool normal) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "source", ANY_INTEGER},
    {-1, 0, true, "target", ANY_INTEGER},
    {-1, 0, true, "cost", ANY_NUMERICAL},
    {-1, 0, false, "reverse_cost", ANY_NUMERICAL},
    {-1, 0, true, "x1", ANY_NUMERICAL},
    {-1, 0, true, "y1", ANY_NUMERICAL},
    {-1, 0, true, "x2", ANY_NUMERICAL},
    {-1, 0, true, "y2", ANY_NUMERICAL}};

    return get_data<Edge_xy_t>(sql, normal, info, &fetch_edge_xy);
}

/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost] FROM edge_table;
  ~~~~

  @param[in] sql The query
  @param[in] normal when false then the graph is reversed
  @param[in] ignore_id when true id value of edge is ignored
  @returns vector of `Edge_t`
  */
std::vector<Edge_t> get_edges(const std::string &sql, bool normal, bool ignore_id) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, !ignore_id, "id", ANY_INTEGER},
    {-1, 0, true, "source", ANY_INTEGER},
    {-1, 0, true, "target", ANY_INTEGER},
    {-1, 0, true, "cost", ANY_NUMERICAL},
    {-1, 0, false, "reverse_cost", ANY_NUMERICAL}};

    return get_data<Edge_t>(sql, normal, info, &fetch_edge);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT start_vid, end_vid, agg_cost FROM matrix_data;
  ~~~~

  @param[in] sql The matrix
  @returns vector of `IID_t_rt`
  */
std::vector<IID_t_rt> get_matrixRows(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "start_vid", ANY_INTEGER},
    {-1, 0, true, "end_vid", ANY_INTEGER},
    {-1, 0, true, "agg_cost", ANY_NUMERICAL}};
    return get_data<IID_t_rt>(sql, true, info, &pgr_fetch_row);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, demand
  [p_node_id | p_x, p_y], p_open, p_close, p_service,
  [d_node_id | d_x, d_y], d_open, d_close, d_service,
  FROM orders;
  ~~~~

  @param[in] sql The orders query
  @param[in] with_id flag that indicates if id is to be used
  @returns vector of `Orders_t`
  */
std::vector<Orders_t> get_orders(
        const std::string &sql,
        bool with_id
        ) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "demand", ANY_NUMERICAL},
    {-1, 0, true, "p_x", ANY_NUMERICAL},
    {-1, 0, true, "p_y", ANY_NUMERICAL},
    {-1, 0, true, "p_open", ANY_NUMERICAL},
    {-1, 0, true, "p_close", ANY_NUMERICAL},
    {-1, 0, false, "p_service", ANY_NUMERICAL},
    {-1, 0, true, "d_x", ANY_NUMERICAL},
    {-1, 0, true, "d_y", ANY_NUMERICAL},
    {-1, 0, true, "d_open", ANY_NUMERICAL},
    {-1, 0, true, "d_close", ANY_NUMERICAL},
    {-1, 0, false, "d_service", ANY_NUMERICAL},
    /* nodes are going to be ignored*/
    {-1, 0, false, "p_node_id", ANY_INTEGER},
    {-1, 0, false, "d_node_id", ANY_INTEGER}};

    if (with_id) {
        /* (x,y) values are ignored*/
        info[2].strict = false;
        info[3].strict = false;
        info[7].strict = false;
        info[8].strict = false;
        /* nodes are compulsory*/
        info[12].strict = true;
        info[13].strict = true;
    }

    return get_data<Orders_t>(sql, with_id, info, &fetch_orders);
}

/**

  For queries of the type:
  ~~~~{.c}
  SELECT pid, edge_id, fraction side FROM points;
  ~~~~

  @param[in] sql The points query
  @returns vector of `Point_on_edge_t`
  */
std::vector<Point_on_edge_t> get_points(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, false, "pid", ANY_INTEGER},
    {-1, 0, true, "edge_id", ANY_INTEGER},
    {-1, 0, true, "fraction", ANY_NUMERICAL},
    {-1, 0, false, "side", pgrouting::CHAR1}};

    return get_data<Point_on_edge_t>(sql, true, info, &fetch_point);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT cost, path FROM restrictions;
  ~~~~

  @param[in] sql The restrictions query
  @returns vector of `Restriction_t`
  */
std::vector<Restriction_t> get_restrictions(const std::string &sql) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "cost", ANY_NUMERICAL},
    {-1, 0, true, "path", ANY_INTEGER_ARRAY}};

    return get_data<Restriction_t>(sql, true, info, &fetch_restriction);
}


/**

  For queries of the type:
  ~~~~{.c}
  SELECT id, capacity, speed, number
  [start_node_id | start_x, start_y], start_open, start_close, start_service,
  [end_node_id | end_x, end_y], end_open, end_close, end_service,
  FROM orders;
  ~~~~

  @param[in] sql The vehicles query
  @param[in] with_id flag that indicates if id is to be used
  @returns vector of `Vehicle_t`
  */
std::vector<Vehicle_t> get_vehicles(const std::string &sql, bool with_id) {
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{
    {-1, 0, true, "id", ANY_INTEGER},
    {-1, 0, true, "capacity", ANY_NUMERICAL},
    {-1, 0, true, "start_x", ANY_NUMERICAL},
    {-1, 0, true, "start_y", ANY_NUMERICAL},
    {-1, 0, false, "number", ANY_INTEGER},
    {-1, 0, false, "start_open", ANY_NUMERICAL},
    {-1, 0, false, "start_close", ANY_NUMERICAL},
    {-1, 0, false, "start_service", ANY_NUMERICAL},
    {-1, 0, false, "end_x", ANY_NUMERICAL},
    {-1, 0, false, "end_y", ANY_NUMERICAL},
    {-1, 0, false, "end_open", ANY_NUMERICAL},
    {-1, 0, false, "end_close", ANY_NUMERICAL},
    {-1, 0, false, "end_service", ANY_NUMERICAL},
    {-1, 0, false, "speed", ANY_NUMERICAL},
    /* nodes are going to be ignored*/
    {-1, 0, false, "start_node_id", ANY_INTEGER},
    {-1, 0, false, "end_node_id", ANY_INTEGER}};

    if (with_id) {
        /* (x,y) values are ignored*/
        info[2].strict = false;
        info[3].strict = false;
        /* start nodes are compulsory*/
        info[14].strict = false;
    }

    return get_data<Vehicle_t>(sql, with_id, info, &fetch_vehicle);
}

}  // namespace pgget
}  // namespace pgrouting
