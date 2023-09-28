/*PGR-GNU*****************************************************************
File: withPoints_driver.h

Copyright (c) 2015 pgRouting developers
Mail: project at pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Yige Huang
Mail: square1ge at gmail.com

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
using MST_rt = struct MST_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Edge_t Edge_t;
typedef struct MST_rt MST_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void pgr_do_withPointsDD(
            Edge_t*,          size_t,
            Point_on_edge_t*, size_t,
            Edge_t*,          size_t,
            int64_t*,         size_t,

            double, char, bool, bool, bool,

            MST_rt**, size_t*,
            char**, char**, char **);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_DRIVING_DISTANCE_WITHPOINTS_DD_DRIVER_H_
