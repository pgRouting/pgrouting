/*PGR-GNU*****************************************************************
File: many_to_many_withPoints_driver.h

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2021 Celia Virginia Vergara Castillo

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

#ifndef INCLUDE_DRIVERS_TRSP_TRSP_WITHPOINTS_DRIVER_H_
#define INCLUDE_DRIVERS_TRSP_TRSP_WITHPOINTS_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
using Restriction_t = struct Restriction_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Edge_t = struct Edge_t;
using Path_rt = struct Path_rt;
using II_t_rt = struct II_t_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Edge_t Edge_t;
typedef struct Restriction_t Restriction_t;
typedef struct Path_rt Path_rt;
typedef struct II_t_rt II_t_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void do_trsp_withPoints(
            Edge_t *edges,
            size_t total_edges,

            Restriction_t *restrictions,
            size_t restrictions_size,

            Point_on_edge_t *points,
            size_t total_points,

            Edge_t *edges_of_points,
            size_t total_edges_of_points,

            II_t_rt  *combinations,
            size_t total_combinations,

            int64_t *start_pidsArr,
            size_t size_start_pidsArr,

            int64_t *end_pidsArr,
            size_t size_end_pidsArr,


            char driving_side,
            bool details,
            bool directed,
            bool only_cost,
            bool normal,

            Path_rt **return_tuples,
            size_t *return_count,
            char** log_msg,
            char** notice_msg,
            char** err_msg);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_TRSP_TRSP_WITHPOINTS_DRIVER_H_
