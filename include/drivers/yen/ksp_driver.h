/*PGR-GNU*****************************************************************
File: ksp_driver.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Aniket Agarwal
Mail: aniketgarg187 at gmail.com

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

#ifndef INCLUDE_DRIVERS_YEN_KSP_DRIVER_H_
#define INCLUDE_DRIVERS_YEN_KSP_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstdint>
#   include <cstddef>
using Edge_t = struct Edge_t;
using Path_rt = struct Path_rt;
using II_t_rt = struct II_t_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Edge_t Edge_t;
typedef struct Path_rt Path_rt;
typedef struct II_t_rt II_t_rt;
#endif


#ifdef __cplusplus
extern "C" {
#endif

    void  pgr_do_ksp(
            Edge_t*, size_t,
            II_t_rt*, size_t,
            int64_t*, size_t,
            int64_t*, size_t,
            size_t,
            bool,
            bool,
            Path_rt**,
            size_t*,
            char**,
            char**,
            char**);

    void  do_pgr_ksp(
            Edge_t  *data_edges,
            size_t total_edges,
            int64_t  start_vid,
            int64_t end_vid,
            size_t K,
            bool directed,
            bool heap_paths,
            Path_rt **return_tuples,
            size_t *return_count,
            char ** log_msg,
            char ** notice_msg,
            char ** err_msg);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_YEN_KSP_DRIVER_H_
