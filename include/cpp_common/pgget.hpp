/*PGR-GNU*****************************************************************
File: pgget.hpp

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

#ifndef INCLUDE_CPP_COMMON_PGGET_H_
#define INCLUDE_CPP_COMMON_PGGET_H_
#pragma once

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

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#if 0
using IID_t_rt = struct IID_t_rt;
using II_t_rt = struct II_t_rt;
using Coordinate_t = struct Coordinate_t;
using Delauny_t = struct Delauny_t;
using Edge_xy_t = struct Edge_xy_t;
using Flow_t = struct Flow_t;
using Edge_bool_t = struct Edge_bool_t;
using CostFlow_t = struct CostFlow_t;
using Edges_t = struct Edges_t;
using Orders_t = struct Orders_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Vehicle_t = struct Vehicle_t;
using Restriction_t = struct Restriction_t;
#endif

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

#if 0
/** @brief Enforces the input array to be @b NOT empty */
int64_t* get_bigIntArray(size_t*, ArrayType*, bool);
#endif

/** @brief Read rows of combinations, called from C file*/
std::vector<II_t_rt> get_combinations(const std::string&);

/** @brief Read rows of matrix, called from C file*/
std::vector<Coordinate_t> get_coordinates(const std::string&);

/** @brief Read rows of delauny triangles, called from C file*/
std::vector<Delauny_t> get_delauny(const std::string&, Delauny_t**, size_t*);

/** @brief Read edges called from C++ file*/
std::vector<Edge_t> get_edges(const std::string&, bool, bool);

/** @brief Read edges with x, y endpointscalled from C file */
std::vector<Edge_xy_t> get_edges_xy(const std::string&, bool);

/** @brief Read edges for flow called from C file */
std::vector<Edge_t> get_flow_edges(const std::string&);

/** @brief Read edges for cost called from C fileflow */
std::vector<CostFlow_t> get_costFlow_edges(const std::string&);

/** @brief Read "basic edges" called from C file */
std::vector<Edge_bool_t> get_basic_edges(const std::string&);

/** @brief Read rows of matrix, called from C file*/
std::vector<IID_t_rt> get_matrixRows(const std::string&);

/** @brief Reads the pick-Deliver orders */
std::vector<Orders_t> get_orders(const std::string&, bool);

/** @brief Read rows of points, called from C file*/
std::vector<Point_on_edge_t> get_points(const std::string&);

/** @brief Read rows of matrix, called from C file*/
std::vector<Restriction_t> get_restrictions(const std::string&);

/** @brief Reads the vehicles */
std::vector<Vehicle_t> get_vehicles(const std::string&, bool);

}  // namespace pgget
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_PGGET_H_
