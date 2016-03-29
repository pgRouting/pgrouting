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
#include <string.h>
#include <deque>
#include <vector>
#include "./../../common/src/pgr_assert.h"
#include "./pgr_pickDeliver.h"
#include "./pickDeliver_driver.h"

// #define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../common/src/memory_func.hpp"

/************************************************************
  customers_sql TEXT,
  max_vehicles INTEGER,
  capacity FLOAT,
  max_cycles INTEGER, 
 ***********************************************************/
void
do_pgr_pickDeliver(
        Customer_t  *customers_arr,
        size_t total_customers,
        int max_vehicles,
        double capacity,
        int max_cycles,
        General_vehicle_orders_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream log;
    try {
        *return_tuples = NULL;
        *return_count = 0;

#if 0
        log << "Starting do_pgr_pickDeliver\n";
        log << "max_vehicles: "  << max_vehicles << "\n";
        log << "capacity: "  << capacity << "\n";
        log << "max_cycles: "  << max_cycles << "\n";
        log << "total_customers: "  << total_customers << "\n";
        for (size_t i = 0; i < total_customers; i++) {
            log << customers_arr[i].id << "\t";
        }
        log << "\n";
#endif

        log << "Read data\n";
        std::string error("");
        Pgr_pickDeliver pd_problem(customers_arr, total_customers, max_vehicles, capacity, max_cycles, error);
        if (error.compare("")) {
            pd_problem.get_log(log);
            *log_msg = strdup(log.str().c_str());
            *err_msg = strdup(error.c_str());
            return;
        }
        pd_problem.get_log(log);
        log << "Finish Reading data\n";

        try {
            pd_problem.solve();
        } catch (AssertFailedException &exept) {
            pd_problem.get_log(log);
            throw exept;
        }

        pd_problem.get_log(log);
        log << "Finish solve\n";





        *log_msg = strdup(log.str().c_str());

    } catch (AssertFailedException &exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
    } catch(...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
    }
}

