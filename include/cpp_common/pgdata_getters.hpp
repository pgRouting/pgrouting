/*PGR-GNU*****************************************************************
File: pgdata_getters.hpp

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

#ifndef INCLUDE_CPP_COMMON_PGDATA_GETTERS_HPP_
#define INCLUDE_CPP_COMMON_PGDATA_GETTERS_HPP_
#pragma once

extern "C" {
#include <postgres.h>
#include <utils/array.h>
}

#include <cstddef>
#include <cstdint>
#include <string>
#include <set>
#include <vector>

#include "cpp_common/undefPostgresDefine.hpp"

#include "c_types/info_t.hpp"
#include "c_types/ii_t_rt.h"
#include "c_types/coordinate_t.h"
#include "c_types/delauny_t.h"
#include "c_types/edge_bool_t_rt.h"
#include "c_types/costFlow_t.h"
#include "c_types/edge_xy_t.h"
#include "c_types/edge_t.h"
#include "c_types/iid_t_rt.h"
#include "c_types/pickDeliver/orders_t.h"
#include "c_types/restriction_t.h"
#include "c_types/point_on_edge_t.h"
#include "c_types/pickDeliver/vehicle_t.h"

namespace pgrouting {
namespace pgget {

/** @brief Enforces the input array to be @b NOT empty */
std::vector<int64_t> get_intArray(ArrayType*, bool);

/** @brief Reads a postgres array saving it as set */
std::set<int64_t> get_intSet(ArrayType*);

/** @brief Read rows of combinations */
std::vector<II_t_rt> get_combinations(const std::string&);

/** @brief Read rows of matrix */
std::vector<Coordinate_t> get_coordinates(const std::string&);

/** @brief Read rows of delauny triangles */
std::vector<Delauny_t> get_delauny(const std::string&);

/** @brief Read edges */
std::vector<Edge_t> get_edges(const std::string&, bool, bool);

/** @brief Read edges with x, y endpoints */
std::vector<Edge_xy_t> get_edges_xy(const std::string&, bool);

/** @brief Read edges for flow */
std::vector<Edge_t> get_flow_edges(const std::string&);

/** @brief Read edges for cost */
std::vector<CostFlow_t> get_costFlow_edges(const std::string&);

/** @brief Read basic edges */
std::vector<Edge_bool_t> get_basic_edges(const std::string&);

/** @brief Read rows of matrix */
std::vector<IID_t_rt> get_matrixRows(const std::string&);

/** @brief Reads the pick-Deliver orders */
std::vector<Orders_t> get_orders(const std::string&, bool);

/** @brief Read rows of points */
std::vector<Point_on_edge_t> get_points(const std::string&);

/** @brief Read rows of matrix */
std::vector<Restriction_t> get_restrictions(const std::string&);

/** @brief Reads the vehicles */
std::vector<Vehicle_t> get_vehicles(const std::string&, bool);

}  // namespace pgget
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_PGGET_HPP_
