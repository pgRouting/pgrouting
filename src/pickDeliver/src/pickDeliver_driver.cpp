/*PGR-GNU*****************************************************************
File: pickDeliver_driver.cpp

Generated with Template by:
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


#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <vector>
#include "./pgr_dijkstra.hpp"
#include "./pickDeliver_driver.h"

// #define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../common/src/memory_func.hpp"

/************************************************************
  orders_sql TEXT,
  max_vehicles INTEGER,
  capacity FLOAT,
  max_cycles INTEGER, 
 ***********************************************************/
void
do_pgr_pickDeliver(
        Order_t  *orders_arr,
        size_t total_orders,
        int max_vehicles,
        double capacity,
        int max_cycles,
        General_vehicle_orders_t **return_tuples,
        size_t *return_count,
        char ** err_msg) {
    std::ostringstream log;
    try {
        log << "Starting do_pgr_pickDeliver\n";
        log << "max_vehicles"  << max_vehicles << "\n";
        log << "capacity"  << capacity << "\n";
        log << "max_cycles"  << max_cycles << "\n";
        log << "total_orders"  << total_orders << "\n";
        for (size_t i = 0; i < total_orders; i++) {
            log << orders_arr[i].id << "\t";
        }

        *err_msg = strdup(log.str().c_str());
    } catch ( ... ) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
    }
}

