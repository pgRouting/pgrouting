/*PGR-GNU*****************************************************************
File: bdAstar_driver.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#ifndef INCLUDE_DRIVERS_BDASTAR_BDASTAR_DRIVER_H_
#define INCLUDE_DRIVERS_BDASTAR_BDASTAR_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
using Edge_xy_t = struct Edge_xy_t;
using Path_rt = struct Path_rt;
using II_t_rt = struct II_t_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Edge_xy_t Edge_xy_t;
typedef struct Path_rt Path_rt;
typedef struct II_t_rt II_t_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void pgr_do_bdAstar(
            Edge_xy_t  *data_edges,
            size_t total_edges,

            II_t_rt  *combinations,
            size_t total_combinations,

            int64_t  *start_vidsArr,
            size_t size_start_vidsArr,
            int64_t  *end_vidsArr,
            size_t size_end_vidsArr,

            bool directed,
            int heuristic,
            double factor,
            double epsilon,
            bool only_cost,

            Path_rt **return_tuples,
            size_t *return_count,

            char** log_msg,
            char** notice_msg,
            char** err_msg);


#ifdef __cplusplus
}
#endif

#endif  //  INCLUDE_DRIVERS_BDASTAR_BDASTAR_DRIVER_H_
