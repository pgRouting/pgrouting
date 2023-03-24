/*PGR-GNU*****************************************************************
File: pgdata_getters.h

Copyright (c) 2023 Celia Virginia Vergara Castillo
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

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
using IID_t_rt = struct IID_t_rt;
using II_t_rt = struct II_t_rt;
using Coordinate_t = struct Coordinate_t;
using Delauny_t = struct Delauny_t;
using Edge_xy_t = struct Edge_xy_t;
using Flow_t = struct Flow_t;
using Edge_bool_t = struct Edge_bool_t;
using CostFlow_t = struct CostFlow_t;
using Edge_t = struct Edge_t;
using Orders_t = struct Orders_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Vehicle_t = struct Vehicle_t;
using Restriction_t = struct Restriction_t;
#else
typedef struct IID_t_rt IID_t_rt;
typedef struct II_t_rt II_t_rt;
typedef struct Coordinate_t Coordinate_t;
typedef struct Delauny_t Delauny_t;
typedef struct Edge_xy_t Edge_xy_t;
typedef struct Flow_t Flow_t;
typedef struct Edge_bool_t Edge_bool_t;
typedef struct CostFlow_t CostFlow_t;
typedef struct Edge_t Edge_t;
typedef struct Orders_t Orders_t;
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Restriction_t Restriction_t;
typedef struct Vehicle_t Vehicle_t;
#endif


/** @brief Read rows of combinations, called from C file*/
void pgr_get_combinations(char *, II_t_rt **, size_t *);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_coordinates(char*, Coordinate_t**, size_t*);

/** @brief Read rows of delauny triangles, called from C file*/
void pgr_get_delauny(char*, Delauny_t**, size_t*);

/** @brief Read edges called from C file*/
void pgr_get_edges(char*, Edge_t**, size_t*, bool, bool);

/** @brief Read edges with x, y endpointscalled from C file */
void pgr_get_edges_xy(char*, Edge_xy_t**, size_t*, bool);

/** @brief Read edges for flow called from C file */
void pgr_get_flow_edges(char*, Edge_t**, size_t*);

/** @brief Read edges for cost called from C fileflow */
void pgr_get_costFlow_edges(char*, CostFlow_t**, size_t*);

/** @brief Read "basic edges" called from C file */
void pgr_get_basic_edges(char*, Edge_bool_t**, size_t*);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_matrixRows(char*, IID_t_rt**, size_t *);

/** @brief Reads the pick-Deliver orders */
void pgr_get_orders(char *, Orders_t **, size_t *, bool);

/** @brief Read rows of points, called from C file*/
void pgr_get_points(char*, Point_on_edge_t**, size_t*);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_restrictions(char*, Restriction_t**, size_t*);

/** @brief Reads the vehicles */
void pgr_get_vehicles(char*, Vehicle_t**, size_t*, bool);

#endif  // INCLUDE_C_COMMON_PGDATA_GETTERS_H_
