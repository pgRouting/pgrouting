/*PGR-GNU*****************************************************************
File: trsp_pgget.h

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

This is a copy of pgdata_getters.h

The old version of TRSP
- Is the only function that has its own edge reader within its code,
  hence the name of the file.
- The old version of TRSP will exist during 3.x series


@note
- The only function that should remain after all functions read the data on the
  C++ code is `fetch_edge`
- The code to be removed is marked with "if 1 ... endif" directives
- The old version of TRSP will be removed on V4

 @todo: Remove marked code
 @todo: V4 Delete this file
*/
#ifndef INCLUDE_C_COMMON_TRSP_PGGET_H_
#define INCLUDE_C_COMMON_TRSP_PGGET_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <postgres.h>
#include <executor/spi.h>
#include <funcapi.h>
#include <utils/builtins.h>
#include <access/htup_details.h>
#include <fmgr.h>
#include <utils/array.h>
#include <catalog/pg_type.h>
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#endif

#include "cpp_common/undefPostgresDefine.hpp"

#ifdef __cplusplus
#if 1
using IID_t_rt = struct IID_t_rt;
using II_t_rt = struct II_t_rt;
using Coordinate_t = struct Coordinate_t;
using Delauny_t = struct Delauny_t;
using Edge_xy_t = struct Edge_xy_t;
using Flow_t = struct Flow_t;
using Edge_bool_t = struct Edge_bool_t;
using CostFlow_t = struct CostFlow_t;
#endif
using Edge_t = struct Edge_t;
#if 1
using Orders_t = struct Orders_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Vehicle_t = struct Vehicle_t;
using Restriction_t = struct Restriction_t;
#endif
#else
#if 1
typedef struct IID_t_rt IID_t_rt;
typedef struct II_t_rt II_t_rt;
typedef struct Coordinate_t Coordinate_t;
typedef struct Delauny_t Delauny_t;
typedef struct Edge_xy_t Edge_xy_t;
typedef struct Flow_t Flow_t;
typedef struct Edge_bool_t Edge_bool_t;
typedef struct CostFlow_t CostFlow_t;
#endif
typedef struct Edge_t Edge_t;
#if 1
typedef struct Orders_t Orders_t;
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Restriction_t Restriction_t;
typedef struct Vehicle_t Vehicle_t;
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif
#if 1
/** @brief Read rows of combinations, called from C file*/
void pgr_get_combinations(char *, II_t_rt **, size_t *, char**);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_coordinates(char*, Coordinate_t**, size_t*, char**);

/** @brief Read rows of delauny triangles, called from C file*/
void pgr_get_delauny(char*, Delauny_t**, size_t*, char**);
#endif

/** @brief Read edges called from C file*/
void pgr_get_edges(char*, Edge_t**, size_t*, bool, bool, char**);

#if 1
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
#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_C_COMMON_TRSP_PGGET_H_
