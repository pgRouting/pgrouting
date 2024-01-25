/*PGR-GNU*****************************************************************
File: trsp_pgget.cpp

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
/** file

This is a copy of pgdata_getters.cpp

The old version of TRSP
- Is the only function that has its own edge reader within its code,
  hence the name of the file.
- The old version of TRSP will exist during 3.x series


@note
- The only function that should remain after all functions read the data on the
C++ code is `fetch_edge`
- The code to be removed is marked with "if 0 ... endif" directives
- The old version of TRSP will be removed on V4

 @todo: Remove marked code
 @todo: V4 Delete this file
*/

#include "c_common/trsp_pgget.h"
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

#include "cpp_common/get_data.hpp"
#include "cpp_common/get_check_data.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/trsp_pgfetch.hpp"

#include "c_types/info_t.hpp"
#if 0
#include "c_types/coordinate_t.h"
#include "c_types/costFlow_t.h"
#include "c_types/ii_t_rt.h"
#include "c_types/iid_t_rt.h"
#include "c_types/delauny_t.h"
#endif
#include "c_types/edge_t.h"
#if 0
#include "c_types/edge_bool_t_rt.h"
#include "c_types/edge_xy_t.h"
#include "c_types/flow_t.h"
#include "c_types/pickDeliver/orders_t.h"
#include "c_types/pickDeliver/vehicle_t.h"
#include "c_types/point_on_edge_t.h"
#include "c_types/restriction_t.h"
#endif

#if 0
/**
 * @param[out] arrlen Length of the array
 * @param[in] input the postgres array
 * @param[in] allow_empty when true, empty arrays are accepted.
 * @param[out] err_msg when not null, there was an error and contains the message
 * @returns Returns a C array of integers
 */
int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input, bool allow_empty, char** err_msg) {
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
void pgr_get_combinations(
        char *sql,
        II_t_rt **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{2};

        info[0] = {-1, 0, true, "source", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "target", pgrouting::ANY_INTEGER};
        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_combination);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void pgr_get_coordinates(
        char *sql,
        Coordinate_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{3};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "x", pgrouting::ANY_NUMERICAL};
        info[2] = {-1, 0, true, "y", pgrouting::ANY_NUMERICAL};
        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_coordinate);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void pgr_get_delauny(
        char *sql,
        Delauny_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{4};

        info[0] = {-1, 0, true, "tid", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "pid", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "x", pgrouting::ANY_NUMERICAL};
        info[3] = {-1, 0, true, "y", pgrouting::ANY_NUMERICAL};
        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_delauny);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void
pgr_get_flow_edges(
        char *sql,
        Edge_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{5};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "source", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "target", pgrouting::ANY_INTEGER};
        info[3] = {-1, 0, true, "capacity", pgrouting::ANY_INTEGER};
        info[4] = {-1, 0, false, "reverse_capacity", pgrouting::ANY_INTEGER};

        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_edge);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void
pgr_get_costFlow_edges(
        char *sql,
        CostFlow_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{7};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "source", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "target", pgrouting::ANY_INTEGER};
        info[3] = {-1, 0, true, "capacity", pgrouting::ANY_INTEGER};
        info[4] = {-1, 0, false, "reverse_capacity", pgrouting::ANY_INTEGER};
        info[5] = {-1, 0, true, "cost", pgrouting::ANY_NUMERICAL};
        info[6] = {-1, 0, false, "reverse_cost", pgrouting::ANY_NUMERICAL};

        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_costFlow_edge);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void
pgr_get_basic_edges(
        char *sql,
        Edge_bool_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{7};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "source", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "target", pgrouting::ANY_INTEGER};
        info[3] = {-1, 0, false, "going", pgrouting::ANY_NUMERICAL};
        info[4] = {-1, 0, false, "coming", pgrouting::ANY_NUMERICAL};
        info[5] = {-1, 0, false, "cost", pgrouting::ANY_NUMERICAL};
        info[6] = {-1, 0, false, "reverse_cost", pgrouting::ANY_NUMERICAL};

        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_basic_edge);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void
pgr_get_edges_xy(
        char *sql,
        Edge_xy_t **rows,
        size_t *total_rows,
        bool normal,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{9};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "source", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "target", pgrouting::ANY_INTEGER};
        info[3] = {-1, 0, true, "cost", pgrouting::ANY_NUMERICAL};
        info[4] = {-1, 0, false, "reverse_cost", pgrouting::ANY_NUMERICAL};
        info[5] = {-1, 0, true, "x1", pgrouting::ANY_NUMERICAL};
        info[6] = {-1, 0, true, "y1", pgrouting::ANY_NUMERICAL};
        info[7] = {-1, 0, true, "x2", pgrouting::ANY_NUMERICAL};
        info[8] = {-1, 0, true, "y2", pgrouting::ANY_NUMERICAL};

        pgrouting::get_data(sql, rows, total_rows, normal, info, &pgrouting::fetch_edge_with_xy);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
}
#endif

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
void
pgr_get_edges(
        char *sql,
        Edge_t **rows,
        size_t *total_rows,
        bool normal,
        bool ignore_id,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{
        {-1, 0, !ignore_id, "id", pgrouting::ANY_INTEGER},
        {-1, 0, true, "source", pgrouting::ANY_INTEGER},
        {-1, 0, true, "target", pgrouting::ANY_INTEGER},
        {-1, 0, true, "cost", pgrouting::ANY_NUMERICAL},
        {-1, 0, false, "reverse_cost", pgrouting::ANY_NUMERICAL}};

        pgrouting::get_data(sql, rows, total_rows, normal, info, &pgrouting::fetch_edge);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
}

#if 0
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
void pgr_get_matrixRows(
        char *sql,
        IID_t_rt **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{3};

        info[0] = {-1, 0, true, "start_vid", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "end_vid", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "agg_cost", pgrouting::ANY_NUMERICAL};
        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::pgr_fetch_row);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void pgr_get_orders(
        char *sql,
        Orders_t **rows,
        size_t *total_rows,
        bool with_id,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{14};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "demand", pgrouting::ANY_NUMERICAL};
        info[2] = {-1, 0, true, "p_x", pgrouting::ANY_NUMERICAL};
        info[3] = {-1, 0, true, "p_y", pgrouting::ANY_NUMERICAL};
        info[4] = {-1, 0, true, "p_open", pgrouting::ANY_NUMERICAL};
        info[5] = {-1, 0, true, "p_close", pgrouting::ANY_NUMERICAL};
        info[6] = {-1, 0, false, "p_service", pgrouting::ANY_NUMERICAL};
        info[7] = {-1, 0, true, "d_x", pgrouting::ANY_NUMERICAL};
        info[8] = {-1, 0, true, "d_y", pgrouting::ANY_NUMERICAL};
        info[9] = {-1, 0, true, "d_open", pgrouting::ANY_NUMERICAL};
        info[10] = {-1, 0, true, "d_close", pgrouting::ANY_NUMERICAL};
        info[11] = {-1, 0, false, "d_service", pgrouting::ANY_NUMERICAL};
        /* nodes are going to be ignored*/
        info[12] = {-1, 0, false, "p_node_id", pgrouting::ANY_INTEGER};
        info[13] = {-1, 0, false, "d_node_id", pgrouting::ANY_INTEGER};

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

        pgrouting::get_data(sql, rows, total_rows, with_id, info, &pgrouting::fetch_orders);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void pgr_get_points(
        char *sql,
        Point_on_edge_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_free;
    using pgrouting::pgr_msg;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{4};

        info[0] = {-1, 0, false, "pid", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "edge_id", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "fraction", pgrouting::ANY_NUMERICAL};
        info[3] = {-1, 0, false, "side", pgrouting::CHAR1};
        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_point);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void pgr_get_restrictions(
        char *sql,
        Restriction_t **rows,
        size_t *total_rows,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{2};

        info[0] = {-1, 0, true, "cost", pgrouting::ANY_NUMERICAL};
        info[1] = {-1, 0, true, "path", pgrouting::ANY_INTEGER_ARRAY};
        pgrouting::get_data(sql, rows, total_rows, true, info, &pgrouting::fetch_restriction);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
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
void pgr_get_vehicles(
        char *sql,
        Vehicle_t **rows,
        size_t *total_rows,
        bool with_id,
        char **err_msg) {
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::Column_info_t;
    try {
        std::vector<Column_info_t> info{16};

        info[0] = {-1, 0, true, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "capacity", pgrouting::ANY_NUMERICAL};
        info[2] = {-1, 0, true, "start_x", pgrouting::ANY_NUMERICAL};
        info[3] = {-1, 0, true, "start_y", pgrouting::ANY_NUMERICAL};
        info[4] = {-1, 0, false, "number", pgrouting::ANY_INTEGER};
        info[5] = {-1, 0, false, "start_open", pgrouting::ANY_NUMERICAL};
        info[6] = {-1, 0, false, "start_close", pgrouting::ANY_NUMERICAL};
        info[7] = {-1, 0, false, "start_service", pgrouting::ANY_NUMERICAL};
        info[8] = {-1, 0, false, "end_x", pgrouting::ANY_NUMERICAL};
        info[9] = {-1, 0, false, "end_y", pgrouting::ANY_NUMERICAL};
        info[10] = {-1, 0, false, "end_open", pgrouting::ANY_NUMERICAL};
        info[11] = {-1, 0, false, "end_close", pgrouting::ANY_NUMERICAL};
        info[12] = {-1, 0, false, "end_service", pgrouting::ANY_NUMERICAL};
        info[13] = {-1, 0, false, "speed", pgrouting::ANY_NUMERICAL};
        /* nodes are going to be ignored*/
        info[14] = {-1, 0, false, "start_node_id", pgrouting::ANY_INTEGER};
        info[15] = {-1, 0, false, "end_node_id", pgrouting::ANY_INTEGER};

        if (with_id) {
            /* (x,y) values are ignored*/
            info[2].strict = false;
            info[3].strict = false;
            /* start nodes are compulsory*/
            info[14].strict = false;
        }

        pgrouting::get_data(sql, rows, total_rows, with_id, info, &pgrouting::fetch_vehicle);
    } catch (const std::string &ex) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg(ex.c_str());
    } catch(...) {
        (*rows) = pgr_free(*rows);
        (*total_rows) = 0;
        *err_msg = pgr_msg("Caught unknown exception!");
    }
}
#endif
