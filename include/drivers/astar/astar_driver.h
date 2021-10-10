/*PGR-GNU*****************************************************************
File: astarOneToMany_driver.h

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

#ifndef INCLUDE_DRIVERS_ASTAR_ASTAR_DRIVER_H_
#define INCLUDE_DRIVERS_ASTAR_ASTAR_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
using Edge_xy_t = struct Edge_xy_t;
using II_t_rt = struct II_t_rt;
using Path_rt = struct Path_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Edge_xy_t Edge_xy_t;
typedef struct II_t_rt II_t_rt;
typedef struct Path_rt Path_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      edges_sql TEXT,
      vertex_table TEXT,
      start_vid BIGINT,
      end_vid BIGINT  directed BOOLEAN DEFAULT true,
     ********************************************************/
    void do_pgr_astarManyToMany(
            Edge_xy_t  *data_edges,
            size_t total_edges,

            II_t_rt  *combinations,
            size_t total_combinations,

            int64_t  *start_vidsArr, size_t size_start_vidsArr,
            int64_t  *end_vidsArr, size_t size_end_vidsArr,
            bool directed,
            int heuristic,
            double factor,
            double epsilon,
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

#endif  // INCLUDE_DRIVERS_ASTAR_ASTAR_DRIVER_H_
