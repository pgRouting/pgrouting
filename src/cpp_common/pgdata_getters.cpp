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

#include "c_common/pgdata_getters.h"
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

#include "cpp_common/get_data.hpp"
#include "cpp_common/get_check_data.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgdata_fetchers.hpp"

#include "c_types/info_t.hpp"
#include "c_types/edge_t.h"
#include "c_types/point_on_edge_t.h"


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
        std::vector<Column_info_t> info{5};

        info[0] = {-1, 0, !ignore_id, "id", pgrouting::ANY_INTEGER};
        info[1] = {-1, 0, true, "source", pgrouting::ANY_INTEGER};
        info[2] = {-1, 0, true, "target", pgrouting::ANY_INTEGER};
        info[3] = {-1, 0, true, "cost", pgrouting::ANY_NUMERICAL};
        info[4] = {-1, 0, false, "reverse_cost", pgrouting::ANY_NUMERICAL};

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
