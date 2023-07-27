/*PGR-GNU*****************************************************************
File: pgdata_getters.cpp

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

#include "cpp_common/pggetdata.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>
#include <sstream>

#include "cpp_common/get_data.hpp"
#include "cpp_common/get_check_data.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgfetch.hpp"

#include "c_types/info_t.hpp"
#include "c_types/coordinate_t.h"
#include "c_types/costFlow_t.h"
#include "c_types/ii_t_rt.h"
#include "c_types/iid_t_rt.h"
#include "c_types/delauny_t.h"
#include "c_types/edge_t.h"
#include "c_types/edge_bool_t_rt.h"
#include "c_types/edge_xy_t.h"
#include "c_types/flow_t.h"
#include "c_types/pickDeliver/orders_t.h"
#include "c_types/pickDeliver/vehicle_t.h"
#include "c_types/point_on_edge_t.h"
#include "c_types/restriction_t.h"

namespace pgrouting {
namespace pgget {

#if 0
/**
 * @param[out] arrlen Length of the array
 * @param[in] input the postgres array
 * @param[in] allow_empty when true, empty arrays are accepted.
 * @param[out] err_msg when not null, there was an error and contains the message
 * @returns Returns a C array of integers
 */
int64_t* get_bigIntArray(size_t *arrlen, ArrayType *input, bool allow_empty, char** err_msg) {
    using pgrouting::pgr_msg;
    try {
        return pgrouting::get_array(input, arrlen, allow_empty);
    } catch (const std::string &ex) {
        (*arrlen) = 0;
        *err_msg = pgr_msg(ex.c_str());
        return nullptr;
    } catch(...) {
        (*arrlen) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
        return nullptr;
    }
}
#endif


/**
  For queries of the type:
  ~~~~{.c}
  SELECT source, target FROM combinations;
  ~~~~

  @param[in] sql The query
  @param [out] rows array of combinations
  @param [out] total_rows size of combinations
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<II_t_rt>
get_combinations(const std::string &sql) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{2};

    info[0] = {-1, 0, true, "source", ANY_INTEGER};
    info[1] = {-1, 0, true, "target", ANY_INTEGER};
    return get_data1<II_t_rt>(sql, true, info, &fetch_combination);
}

/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, x, y FROM coordinates;
  ~~~~

  @param[in] sql The query
  @param[out] rows the matrix coordinates
  @param[out] total_rows size of coordinates
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Coordinate_t> get_coordinates(
        const std::string &sql
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{3};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "x", ANY_NUMERICAL};
    info[2] = {-1, 0, true, "y", ANY_NUMERICAL};
    return get_data1<Coordinate_t>(sql, true, info, &fetch_coordinate);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT tid, pid, x, y FROM delauny;
  SELECT pid, pid, x, y FROM delauny;
  ~~~~

  @param[in] sql The query
  @param[out] rows the delauny array
  @param[out] total_rows size of delauny
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Delauny_t> get_delauny(const std::string &sql) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{4};

    info[0] = {-1, 0, true, "tid", ANY_INTEGER};
    info[1] = {-1, 0, true, "pid", ANY_INTEGER};
    info[2] = {-1, 0, true, "x", ANY_NUMERICAL};
    info[3] = {-1, 0, true, "y", ANY_NUMERICAL};
    return get_data1<Delauny_t>(sql, true, info, &fetch_delauny);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, capacity, [reverse_capacity] FROM edge_table;
  ~~~~

  @param[in] sql The query
  @param[out] rows the edges array
  @param[out] total_rows size of edges
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Edge_t>
get_flow_edges(
        const std::string &sql
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{5};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "source", ANY_INTEGER};
    info[2] = {-1, 0, true, "target", ANY_INTEGER};
    info[3] = {-1, 0, true, "capacity", ANY_INTEGER};
    info[4] = {-1, 0, false, "reverse_capacity", ANY_INTEGER};

    return get_data1<Edge_t>(sql, true, info, &fetch_edge);
}

/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, capacity, [reverse_capacity], cost, [reverse_cost]
  FROM edge_table;
  ~~~~

  @param[in] sql The query
  @param[out] rows the edges array
  @param[out] total_rows size of edges
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<CostFlow_t>
get_costFlow_edges(
        const std::string &sql
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{7};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "source", ANY_INTEGER};
    info[2] = {-1, 0, true, "target", ANY_INTEGER};
    info[3] = {-1, 0, true, "capacity", ANY_INTEGER};
    info[4] = {-1, 0, false, "reverse_capacity", ANY_INTEGER};
    info[5] = {-1, 0, true, "cost", ANY_NUMERICAL};
    info[6] = {-1, 0, false, "reverse_cost", ANY_NUMERICAL};

    return get_data1<CostFlow_t>(sql, true, info, &fetch_costFlow_edge);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost] FROM edge_table;
  ~~~~

  For backward compatability it also accepts:
  ~~~~{.c}
  SELECT id, source, target, going, [coming] FROM edge_table;
  ~~~~
  @todo This option will be removed on V4

  @param[in] sql The query
  @param[out] rows the edges array
  @param[out] total_rows size of edges
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Edge_bool_t>
get_basic_edges(
        const std::string &sql
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{7};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "source", ANY_INTEGER};
    info[2] = {-1, 0, true, "target", ANY_INTEGER};
    info[3] = {-1, 0, false, "going", ANY_NUMERICAL};
    info[4] = {-1, 0, false, "coming", ANY_NUMERICAL};
    info[5] = {-1, 0, false, "cost", ANY_NUMERICAL};
    info[6] = {-1, 0, false, "reverse_cost", ANY_NUMERICAL};

    return get_data1<Edge_bool_t>(sql, true, info, &fetch_basic_edge);
}

/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost], x1, y1, x2, y2 FROM edge_table;
  ~~~~

  @param[in] sql The query
  @param[out] rows the edges array
  @param[out] total_rows size of edges
  @param[in] normal when false then the graph is reversed
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Edge_xy_t>
get_edges_xy(
        const std::string &sql,
        bool normal) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{9};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "source", ANY_INTEGER};
    info[2] = {-1, 0, true, "target", ANY_INTEGER};
    info[3] = {-1, 0, true, "cost", ANY_NUMERICAL};
    info[4] = {-1, 0, false, "reverse_cost", ANY_NUMERICAL};
    info[5] = {-1, 0, true, "x1", ANY_NUMERICAL};
    info[6] = {-1, 0, true, "y1", ANY_NUMERICAL};
    info[7] = {-1, 0, true, "x2", ANY_NUMERICAL};
    info[8] = {-1, 0, true, "y2", ANY_NUMERICAL};

    return get_data1<Edge_xy_t>(sql, normal, info, &fetch_edge_xy);
}

/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, source, target, cost, [reverse_cost] FROM edge_table;
  ~~~~

  @param[in] sql The query
  @param[out] rows the edges array
  @param[out] total_rows size of edges
  @param[in] normal when false then the graph is reversed
  @param[in] ignore_id when true id value of edge is ignored
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Edge_t>
get_edges(
        const std::string &sql,
        bool normal,
        bool ignore_id) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{5};

    info[0] = {-1, 0, !ignore_id, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "source", ANY_INTEGER};
    info[2] = {-1, 0, true, "target", ANY_INTEGER};
    info[3] = {-1, 0, true, "cost", ANY_NUMERICAL};
    info[4] = {-1, 0, false, "reverse_cost", ANY_NUMERICAL};

    return get_data1<Edge_t>(sql, normal, info, &fetch_edge);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT start_vid, end_vid, agg_cost FROM matrix_data;
  ~~~~

  @param[in] sql The query
  @param[out] rows of the matrix array
  @param[out] total_rows size of matrix rows
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<IID_t_rt> get_matrixRows(
        const std::string &sql
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{3};

    info[0] = {-1, 0, true, "start_vid", ANY_INTEGER};
    info[1] = {-1, 0, true, "end_vid", ANY_INTEGER};
    info[2] = {-1, 0, true, "agg_cost", ANY_NUMERICAL};
    return get_data1<IID_t_rt>(sql, true, info, &pgr_fetch_row);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, demand
  [p_node_id | p_x, p_y], p_open, p_close, p_service,
  [d_node_id | d_x, d_y], d_open, d_close, d_service,
  FROM orders;
  ~~~~

  @param[in] sql The query
  @param[out] rows of the orders
  @param[out] total_rows size of orders rows
  @param[out] with_id flag to choose if its euclidean or matrix
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Orders_t> get_orders(
        const std::string &sql,
        bool with_id
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{14};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "demand", ANY_NUMERICAL};
    info[2] = {-1, 0, true, "p_x", ANY_NUMERICAL};
    info[3] = {-1, 0, true, "p_y", ANY_NUMERICAL};
    info[4] = {-1, 0, true, "p_open", ANY_NUMERICAL};
    info[5] = {-1, 0, true, "p_close", ANY_NUMERICAL};
    info[6] = {-1, 0, false, "p_service", ANY_NUMERICAL};
    info[7] = {-1, 0, true, "d_x", ANY_NUMERICAL};
    info[8] = {-1, 0, true, "d_y", ANY_NUMERICAL};
    info[9] = {-1, 0, true, "d_open", ANY_NUMERICAL};
    info[10] = {-1, 0, true, "d_close", ANY_NUMERICAL};
    info[11] = {-1, 0, false, "d_service", ANY_NUMERICAL};
    /* nodes are going to be ignored*/
    info[12] = {-1, 0, false, "p_node_id", ANY_INTEGER};
    info[13] = {-1, 0, false, "d_node_id", ANY_INTEGER};

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

    return get_data1<Orders_t>(sql, with_id, info, &fetch_orders);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT pid, edge_id, fraction side FROM points;
  ~~~~

  @param[in] sql The query
  @param[out] rows the points array
  @param[out] total_rows size of points
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Point_on_edge_t> get_points(
        const std::string &sql
        ) {
    using pgrouting::pgr_free;
    using pgrouting::pgr_msg;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{4};

    info[0] = {-1, 0, false, "pid", ANY_INTEGER};
    info[1] = {-1, 0, true, "edge_id", ANY_INTEGER};
    info[2] = {-1, 0, true, "fraction", ANY_NUMERICAL};
    info[3] = {-1, 0, false, "side", pgrouting::CHAR1};
    return get_data1<Point_on_edge_t>(sql, true, info, &fetch_point);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT cost, path FROM restrictions;
  ~~~~

  @param[in] sql The query
  @param[out] rows the restrictions array
  @param[out] total_rows size of restrictions
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Restriction_t> get_restrictions(
        const std::string &sql
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{2};

    info[0] = {-1, 0, true, "cost", ANY_NUMERICAL};
    info[1] = {-1, 0, true, "path", ANY_INTEGER_ARRAY};
    return get_data1<Restriction_t>(sql, true, info, &fetch_restriction);
}


/**
  For queries of the type:
  ~~~~{.c}
  SELECT id, capacity, speed, number
  [start_node_id | start_x, start_y], start_open, start_close, start_service,
  [end_node_id | end_x, end_y], end_open, end_close, end_service,
  FROM orders;
  ~~~~

  @param[in] sql The query
  @param[out] rows of vehicles
  @param[out] total_rows size of vehicles rows
  @param[out] with_id flag to choose if its euclidean or matrix
  @param[out] err_msg when not null, there was an error and contains the message
  */
std::vector<Vehicle_t> get_vehicles(
        const std::string &sql,
        bool with_id
        ) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    std::vector<Column_info_t> info{16};

    info[0] = {-1, 0, true, "id", ANY_INTEGER};
    info[1] = {-1, 0, true, "capacity", ANY_NUMERICAL};
    info[2] = {-1, 0, true, "start_x", ANY_NUMERICAL};
    info[3] = {-1, 0, true, "start_y", ANY_NUMERICAL};
    info[4] = {-1, 0, false, "number", ANY_INTEGER};
    info[5] = {-1, 0, false, "start_open", ANY_NUMERICAL};
    info[6] = {-1, 0, false, "start_close", ANY_NUMERICAL};
    info[7] = {-1, 0, false, "start_service", ANY_NUMERICAL};
    info[8] = {-1, 0, false, "end_x", ANY_NUMERICAL};
    info[9] = {-1, 0, false, "end_y", ANY_NUMERICAL};
    info[10] = {-1, 0, false, "end_open", ANY_NUMERICAL};
    info[11] = {-1, 0, false, "end_close", ANY_NUMERICAL};
    info[12] = {-1, 0, false, "end_service", ANY_NUMERICAL};
    info[13] = {-1, 0, false, "speed", ANY_NUMERICAL};
    /* nodes are going to be ignored*/
    info[14] = {-1, 0, false, "start_node_id", ANY_INTEGER};
    info[15] = {-1, 0, false, "end_node_id", ANY_INTEGER};

    if (with_id) {
        /* (x,y) values are ignored*/
        info[2].strict = false;
        info[3].strict = false;
        /* start nodes are compulsory*/
        info[14].strict = false;
    }

    return get_data1<Vehicle_t>(sql, with_id, info, &fetch_vehicle);
}

}  // namespace pgget
}  // namespace pgrouting
