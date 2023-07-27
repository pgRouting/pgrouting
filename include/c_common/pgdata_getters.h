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

#ifdef __cplusplus
using II_t_rt = struct II_t_rt;
using Edge_xy_t = struct Edge_xy_t;
using Flow_t = struct Flow_t;
using Edge_t = struct Edge_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Restriction_t = struct Restriction_t;
#else
typedef struct II_t_rt II_t_rt;
typedef struct Edge_xy_t Edge_xy_t;
typedef struct Flow_t Flow_t;
typedef struct Edge_t Edge_t;
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Restriction_t Restriction_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** @brief Read rows of combinations, called from C file*/
void pgr_get_combinations(char *, II_t_rt **, size_t *, char**);

/** @brief Read edges called from C file*/
void pgr_get_edges(char*, Edge_t**, size_t*, bool, bool, char**);

/** @brief Read rows of points, called from C file*/
void pgr_get_points(char*, Point_on_edge_t**, size_t*, char**);

/** @brief Read rows of matrix, called from C file*/
void pgr_get_restrictions(char*, Restriction_t**, size_t*, char**);

/** @brief Enforces the input array to be @b NOT empty */
int64_t* pgr_get_bigIntArray(size_t*, ArrayType*, bool, char**);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_C_COMMON_PGDATA_GETTERS_H_
