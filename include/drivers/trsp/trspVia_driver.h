/*PGR-GNU*****************************************************************
File: trspVia_driver.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo

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

#ifndef INCLUDE_DRIVERS_TRSP_TRSPVIA_DRIVER_H_
#define INCLUDE_DRIVERS_TRSP_TRSPVIA_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstdint>
#   include <cstddef>
using Edge_t = struct Edge_t;
using Routes_t = struct Routes_t;
using Restriction_t = struct Restriction_t;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Edge_t Edge_t;
typedef struct Routes_t Routes_t;
typedef struct Restriction_t Restriction_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif

void do_trspVia(
        Edge_t *, size_t,           // edges
        Restriction_t *, size_t,    // restrictions
        int64_t *, size_t,          // vias

        bool,  // directed
        bool,  // strict
        bool,  // U_turn_on_edge

        Routes_t **, size_t *,  // tuples

        char**,   // log
        char**,   // notice
        char**);  // err

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_TRSP_TRSPVIA_DRIVER_H_
