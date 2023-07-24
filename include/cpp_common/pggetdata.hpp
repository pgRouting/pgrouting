/*PGR-GNU*****************************************************************
File: pgdata_getters.h

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

#ifndef INCLUDE_C_COMMON_PGDATA_GETTERS_H_
#define INCLUDE_C_COMMON_PGDATA_GETTERS_H_
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

using IID_t_rt = struct IID_t_rt;
using II_t_rt = struct II_t_rt;
using Coordinate_t = struct Coordinate_t;
using Delauny_t = struct Delauny_t;
using Edge_xy_t = struct Edge_xy_t;
using Flow_t = struct Flow_t;
using Edge_bool_t = struct Edge_bool_t;
using CostFlow_t = struct CostFlow_t;
#include "c_types/edge_t.h"
using Orders_t = struct Orders_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Vehicle_t = struct Vehicle_t;
using Restriction_t = struct Restriction_t;

namespace pgrouting {
namespace pgget {

#if 0
/** @brief Read rows of combinations, called from C file*/
void pgr_get_combinations(char *, II_t_rt **, size_t *, char**);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_coordinates(char*, Coordinate_t**, size_t*, char**);

/** @brief Read rows of delauny triangles, called from C file*/
void pgr_get_delauny(char*, Delauny_t**, size_t*, char**);
#endif

/** @brief Read edges called from C++ file*/
std::vector<Edge_t> get_edges(const std::string&, bool, bool, std::ostringstream&);

#if 0
/** @brief Read edges with x, y endpointscalled from C file */
void pgr_get_edges_xy(char*, Edge_xy_t**, size_t*, bool, char**);

/** @brief Read edges for flow called from C file */
void pgr_get_flow_edges(char*, Edge_t**, size_t*, char**);

/** @brief Read edges for cost called from C fileflow */
void pgr_get_costFlow_edges(char*, CostFlow_t**, size_t*, char**);

/** @brief Read "basic edges" called from C file */
void pgr_get_basic_edges(char*, Edge_bool_t**, size_t*, char**);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_matrixRows(char*, IID_t_rt**, size_t *, char **);

/** @brief Reads the pick-Deliver orders */
void pgr_get_orders(char *, Orders_t **, size_t *, bool, char**);

/** @brief Read rows of points, called from C file*/
void pgr_get_points(char*, Point_on_edge_t**, size_t*, char**);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_restrictions(char*, Restriction_t**, size_t*, char**);

/** @brief Reads the vehicles */
void pgr_get_vehicles(char*, Vehicle_t**, size_t*, bool, char**);

/** @brief Enforces the input array to be @b NOT empty */
int64_t* pgr_get_bigIntArray(size_t*, ArrayType*, bool, char**);
#endif

}  // namespace getters
}  // namespace pgrouting

#endif  // INCLUDE_C_COMMON_PGDATA_GETTERS_H_
