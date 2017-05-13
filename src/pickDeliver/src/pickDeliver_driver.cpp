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


#include "drivers/pickDeliver/pickDeliver_driver.h"

#include <string.h>
#include <sstream>
#include <string>
#include <deque>
#include <vector>

#include "pickDeliver/pgr_pickDeliver.h"
#include "cpp_common/Dmatrix.h"

#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"

void
do_pgr_pickDeliver(
        PickDeliveryOrders_t customers_arr[],
        size_t total_customers,

        Vehicle_t *vehicles_arr,
        size_t total_vehicles,

        Matrix_cell_t *matrix_cells_arr,
        size_t total_cells,

        int max_cycles,
        int initial_solution_id,

        General_vehicle_orders_t **return_tuples,
        size_t *return_count,

        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(total_customers);
        pgassert(total_vehicles);
        pgassert(total_vehicles);
        pgassert(*return_count == 0);
        pgassert(!(*return_tuples));
        log << "do_pgr_pickDeliver\n";


        /*
         * transform to C++ containers
         */
        std::vector<PickDeliveryOrders_t> orders(
                customers_arr, customers_arr + total_customers);

        std::vector<Vehicle_t> vehicles(
                vehicles_arr, vehicles_arr + total_vehicles);

        std::vector <Matrix_cell_t> data_costs(
                matrix_cells_arr,
                matrix_cells_arr + total_cells);

        pgrouting::tsp::Dmatrix cost_matrix(data_costs);

        if (!cost_matrix.has_no_infinity()) {
            err << "An Infinity value was found on the Matrix";
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }

        log << "Read data\n";
        pgrouting::vrp::pickdeliver::Pgr_pickDeliver pd_problem(
                orders,
                vehicles,
                cost_matrix,
                max_cycles,
                initial_solution_id);

        if (!pd_problem.msg.has_error()) {
            log << "ERROR found\n";
            log << pd_problem.msg.get_log();
            err << pd_problem.msg.get_error();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }
        pgassert(false);
        log << pd_problem.msg.get_log();
        log << "Finish Reading data\n";
        pd_problem.msg.clear();

#if 1
        log << pd_problem;
        log << pd_problem.msg.get_log();
        log << "Finish printing read data\n";
        pd_problem.msg.clear();
#endif
        *log_msg = pgr_msg(log.str().c_str());
        return;
        try {
            pd_problem.solve();

            log << pd_problem.msg.get_log();
            log << "Finish solve\n";
            pd_problem.msg.clear();
            *log_msg = pgr_msg(log.str().c_str());
            return;
        } catch (AssertFailedException &except) {
            log << pd_problem.msg.get_log();
            throw except;
        }

        auto solution = pd_problem.get_postgres_result();

        log << pd_problem.msg.get_log();
        log << "solution size: " << solution.size() << "\n";
        pd_problem.msg.clear();

        if (!solution.empty()) {
            (*return_tuples) = pgr_alloc(solution.size(), (*return_tuples));
            int seq = 0;
            for (const auto &row : solution) {
                (*return_tuples)[seq] = row;
                ++seq;
            }
        }
        (*return_count) = solution.size();

        pgassert(*err_msg == NULL);
        *log_msg = log.str().empty()?
            nullptr :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            nullptr :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (std::exception& except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
