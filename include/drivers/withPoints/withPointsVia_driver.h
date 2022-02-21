/*PGR-GNU*****************************************************************
File: withPointsVia_driver.h

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo

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

#ifndef INCLUDE_DRIVERS_WITHPOINTS_WITHPOINTSVIA_DRIVER_H_
#define INCLUDE_DRIVERS_WITHPOINTS_WITHPOINTSVIA_DRIVER_H_
#pragma once

#ifdef __cplusplus
#   include <cstdint>
#   include <cstddef>
using Point_on_edge_t = struct Point_on_edge_t;
using Edge_t = struct Edge_t;
using Routes_t = struct Routes_t;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Point_on_edge_t Point_on_edge_t;
typedef struct Edge_t Edge_t;
typedef struct Routes_t Routes_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif

/** @brief Process pgr_withPointsVia */
void
do_withPointsVia(
        Edge_t*,  size_t,  // edges
        Point_on_edge_t *, size_t,  // Points
        Edge_t*, size_t,  // edges of points
        int64_t *,    size_t,  // via vertices

        bool,  // directed

        char,  // driving_side
        bool,  // details

        bool,    // strict
        bool,  // U_turn_on_edge,

        Routes_t**, size_t*,  // results

        char**,   // log
        char**,   // notice
        char**);   // error

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_WITHPOINTS_WITHPOINTSVIA_DRIVER_H_
