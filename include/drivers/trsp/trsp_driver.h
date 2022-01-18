/*PGR-GNU*****************************************************************
File: trsp_driver.h

Copyright (c) 2011 Stephen Woodbridge
Copyright (c) 2017 Vicky Vergara
* Rewrite for cleanup
Copyright (c) 2022 Vicky Vergara
* Rewrite for new signatures

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

#ifndef INCLUDE_DRIVERS_TRSP_TRSP_DRIVER_H_
#define INCLUDE_DRIVERS_TRSP_TRSP_DRIVER_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
using Restriction_t = struct Restriction_t;
using Path_rt = struct Path_rt;
using Edge_t = struct Edge_t;
using II_t_rt = struct II_t_rt;
#else
#include <stddef.h>
#include <stdint.h>
typedef struct Restriction_t Restriction_t;
typedef struct Path_rt Path_rt;
typedef struct Edge_t Edge_t;
typedef struct II_t_rt II_t_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void do_trsp(
        Edge_t *, size_t,           // edges
        Restriction_t *, size_t,    // restrictions

        II_t_rt *, size_t,  // combinations
        int64_t *, size_t,  // starts
        int64_t *, size_t,  // ends

        bool,  // directed

        Path_rt **, size_t *,  // tuples

        char**,   // log
        char**,   // notice
        char**);  // err

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_TRSP_TRSP_DRIVER_H_
