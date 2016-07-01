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
#pragma once
#ifndef SRC_PICKDELIVER_SRC_PICKDELIVER_DRIVER_H_
#define SRC_PICKDELIVER_SRC_PICKDELIVER_DRIVER_H_

#include <stddef.h>
#include "./../../common/src/pgr_types.h"

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
            Customer_t *customers_arr, size_t total_customers,
            int max_vehicles,
            double capacity,
            double speed,
            int max_cycles,
            General_vehicle_orders_t **return_tuples,
            size_t *return_count,
            char ** log_msg,
            char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  //  SRC_PICKDELIVER_SRC_PICKDELIVER_DRIVER_H_
