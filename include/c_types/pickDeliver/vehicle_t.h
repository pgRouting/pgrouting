/*PGR-GNU*****************************************************************
File: vehicle_t.h

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
/*! @file */

#ifndef INCLUDE_C_TYPES_PICKDELIVER_VEHICLE_T_H_
#define INCLUDE_C_TYPES_PICKDELIVER_VEHICLE_T_H_
#pragma once

/* for int64_t */
#ifdef __cplusplus
#   include <cstdint>
#else
#   include <stdint.h>
#endif

/*
 * its with either (foo_x, foo_y) pairs for euclidean or with foo_node_id (for matrix)
 */
typedef struct {
    int64_t id;
    double capacity;
    double speed;

    double start_x;
    double start_y;
    int64_t start_node_id;

    int64_t cant_v;

    double start_open_t;
    double start_close_t;
    double start_service_t;

    double end_x;
    double end_y;
    int64_t end_node_id;

    double end_open_t;
    double end_close_t;
    double end_service_t;
} Vehicle_t;


#endif  // INCLUDE_C_TYPES_PICKDELIVER_VEHICLE_T_H_
