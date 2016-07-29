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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include "./pickDeliver_driver.h"
#include <string.h>
#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_alloc.hpp"
#include "./pgr_pickDeliver.h"

/************************************************************
  customers_sql TEXT,
  max_vehicles INTEGER,
  capacity FLOAT,
  max_cycles INTEGER,
 ***********************************************************/
void
do_pgr_pickDeliver(
        Customer_t *customers_arr,
        size_t total_customers,
        int max_vehicles,
        double capacity,
        double speed,
        int max_cycles,
        General_vehicle_orders_t **result_tuples,
        size_t *total_count,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream log;
    try {
        std::ostringstream tmp_log;
        *result_tuples = NULL;
        *total_count = 0;

        log << "Read data\n";
        std::string error("");
        pgrouting::vrp::Pgr_pickDeliver pd_problem(
                customers_arr,
                total_customers,
                max_vehicles,
                capacity,
                speed,
                max_cycles,
                error);
        if (error.compare("")) {
            pd_problem.get_log(log);
            *log_msg = strdup(log.str().c_str());
            *err_msg = strdup(error.c_str());
            return;
        }
        pd_problem.get_log(tmp_log);
        log << "Finish Reading data\n";

        try {
            pd_problem.solve();
        } catch (AssertFailedException &exept) {
            pd_problem.get_log(log);
            throw exept;
        }

        pd_problem.get_log(log);
        log << "Finish solve\n";

        std::vector<General_vehicle_orders_t> solution;
        pd_problem.get_postgres_result(solution);
        pd_problem.get_log(tmp_log);
        log << "solution size: " << solution.size() << "\n";


        (*result_tuples) = pgr_alloc(solution.size(), (*result_tuples));
        int seq = 0;
        for (const auto &row : solution) {
            (*result_tuples)[seq] = row;
            ++seq;
        }
        (*total_count) = solution.size();

        pd_problem.get_log(log);
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

