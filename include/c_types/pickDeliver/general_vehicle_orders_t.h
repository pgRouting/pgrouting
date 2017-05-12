/*PGR-GNU*****************************************************************
File: general_vehicle_orders_t.h

Copyright (c) 2017 Celia Virginia Vergara Castillo
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

#ifndef INCLUDE_C_TYPES_PICKDELIVER_GENERAL_VEHICLE_ORDERS_T_H_
#define INCLUDE_C_TYPES_PICKDELIVER_GENERAL_VEHICLE_ORDERS_T_H_
#pragma once


#ifdef __cplusplus

#include <cstddef>

#else  // __cplusplus

// for bool
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-pedantic"
#endif

#include <postgres.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

// For NULL & size_t
#include <stdlib.h>


#endif  // __cplusplus

// For int64_t etc
#include <stdint.h>


/*
   OUT seq INTEGER,        done in the .c code
   OUT vehicle_seq INTEGER,
   OUT vehicle_id INTEGER,
   OUT order_id BIGINT,
   OUT travelTime FLOAT,
   OUT arrivalTime FLOAT,
   OUT waitTime FLOAT,
   OUT serviceTime FLOAT,
   OUT departureTime FLOAT,
   */

typedef struct  {
    int vehicle_number;
    int64_t vehicle_id;
    int vehicle_seq;
    int64_t order_id;
    int stop_type;
    double cargo;
    double travelTime;
    double arrivalTime;
    double waitTime;
    double serviceTime;
    double departureTime;
} General_vehicle_orders_t;

/*************************************************************************/

#endif  // INCLUDE_C_TYPES_PICKDELIVER_GENERAL_VEHICLE_ORDERS_T_H_
