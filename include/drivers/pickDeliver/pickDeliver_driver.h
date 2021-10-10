/*PGR-GNU*****************************************************************
File: pickDeliver_driver.h

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

/*! @file pickDeliver_driver.h */

#ifndef INCLUDE_DRIVERS_PICKDELIVER_PICKDELIVER_DRIVER_H_
#define INCLUDE_DRIVERS_PICKDELIVER_PICKDELIVER_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
using Orders_t = struct Orders_t;
using Vehicle_t = struct Vehicle_t;
using Schedule_rt = struct Schedule_rt;
using IID_t_rt = struct IID_t_rt;
#else
#   include <stddef.h>
typedef struct Orders_t Orders_t;
typedef struct Vehicle_t Vehicle_t;
typedef struct Schedule_rt Schedule_rt;
typedef struct IID_t_rt IID_t_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      orders_sql TEXT,
      max_vehicles INTEGER,
      capacity FLOAT,
      max_cycles INTEGER,
     ********************************************************/
    void do_pgr_pickDeliver(
            Orders_t *pd_orders_arr, size_t total_pd_orders,
            Vehicle_t *vehicles_arr, size_t total_vehicles,
            IID_t_rt *matrix_cells_arr, size_t total_cells,

            double factor,
            int max_cycles,
            int initial_solution_id,

            Schedule_rt **return_tuples,
            size_t *return_count,

            char **log_msg,
            char **notice_msg,
            char **err_msg);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_PICKDELIVER_PICKDELIVER_DRIVER_H_
