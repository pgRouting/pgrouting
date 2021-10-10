/*PGR-GNU*****************************************************************
File: withPoints_driver.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

#ifndef INCLUDE_DRIVERS_DRIVING_DISTANCE_WITHPOINTS_DD_DRIVER_H_
#define INCLUDE_DRIVERS_DRIVING_DISTANCE_WITHPOINTS_DD_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
using Point_on_edge_t = struct Point_on_edge_t;
using Edge_t = struct Edge_t;
using Path_rt = struct Path_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Edge_t Edge_t;
typedef struct Path_rt Path_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif


    void do_pgr_many_withPointsDD(
            Edge_t      *edges,             size_t total_edges,
            Point_on_edge_t *points_p,          size_t total_points,
            Edge_t      *edges_of_points,   size_t total_edges_of_points,

            int64_t  *start_pids_arr,    size_t s_len,
            double distance,

            bool directed,
            char driving_side,
            bool details,
            bool equiCost,

            Path_rt **return_tuples, size_t *return_count,
            char** log_msg,
            char** notice_msg,
            char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_DRIVING_DISTANCE_WITHPOINTS_DD_DRIVER_H_
