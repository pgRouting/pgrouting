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


#include "drivers/pickDeliver/pickDeliverEuclidean_driver.h"

#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <map>
#include <utility>

#include "vrp/pgr_pickDeliver.h"

#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"

/************************************************************
  customers_sql TEXT,
  max_vehicles INTEGER,
  factor FLOAT,
  capacity FLOAT,
  max_cycles INTEGER,
 ***********************************************************/
void
do_pgr_pickDeliverEuclidean(
        PickDeliveryOrders_t *customers_arr,
        size_t total_customers,

        Vehicle_t *vehicles_arr,
        size_t total_vehicles,

        double factor,
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
        *return_tuples = nullptr;
        *return_count = 0;

        /*
         * transform to C++ containers
         */
        std::vector<PickDeliveryOrders_t> orders(
                customers_arr, customers_arr + total_customers);
        std::vector<Vehicle_t> vehicles(
                vehicles_arr, vehicles_arr + total_vehicles);

        std::map<std::pair<double, double>, int64_t> matrix_data;

        for (const auto &o : orders) {
            matrix_data[std::pair<double, double>(o.pick_x, o.pick_y)] = o.pick_node_id;
            matrix_data[std::pair<double, double>(o.deliver_x, o.deliver_y)] = o.deliver_node_id;
        }

        for (const auto &v : vehicles) {
            matrix_data[std::pair<double, double>(v.start_x, v.start_y)] = v.start_node_id;
            matrix_data[std::pair<double, double>(v.end_x, v.end_y)] = v.end_node_id;
        }

        Identifiers<int64_t> unique_ids;
        for (const auto &e : matrix_data) {
            unique_ids += e.second;
        }
        if (unique_ids.size() != matrix_data.size()) {
            // ignoring ids given by the user
            int64_t id(0);
            for (auto &e : matrix_data) {
                e.second = id++;
            }
        }

        for (auto &o : orders) {
            o.pick_node_id = matrix_data[std::pair<double, double>(o.pick_x, o.pick_y)];
            o.deliver_node_id = matrix_data[std::pair<double, double>(o.deliver_x, o.deliver_y)];
        }
        for (auto &v : vehicles) {
            v.start_node_id = matrix_data[std::pair<double, double>(v.start_x, v.start_y)];
            v.end_node_id = matrix_data[std::pair<double, double>(v.end_x, v.end_y)];
        }

        pgrouting::tsp::Dmatrix cost_matrix(matrix_data);

        log << "Initialize problem\n";
        pgrouting::vrp::Pgr_pickDeliver pd_problem(
                orders,
                vehicles,
                cost_matrix,
                factor,
                static_cast<size_t>(max_cycles),
                initial_solution_id);

        err << pd_problem.msg.get_error();
        if (!err.str().empty()) {
            log.str("");
            log.clear();
            log << pd_problem.msg.get_error();
            log << pd_problem.msg.get_log();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }
        log << pd_problem.msg.get_log();
        log << "Finish Reading data\n";

        try {
            pd_problem.solve();
        } catch (AssertFailedException &except) {
            log << pd_problem.msg.get_log();
            throw;
        } catch(...) {
            log << "Caught unknown exception!";
            throw;
        }

        log << pd_problem.msg.get_log();
        log << "Finish solve\n";

        auto solution = pd_problem.get_postgres_result();
        log << pd_problem.msg.get_log();
        log << "solution size: " << solution.size() << "\n";


        if (!solution.empty()) {
            (*return_tuples) = pgr_alloc(solution.size(), (*return_tuples));
            int seq = 0;
            for (const auto &row : solution) {
                (*return_tuples)[seq] = row;
                ++seq;
            }
        }
        (*return_count) = solution.size();

        log << pd_problem.msg.get_log();

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
    } catch (const std::pair<std::string, std::string>& ex) {
        (*return_count) = 0;
        err << ex.first;
        log.str("");
        log.clear();
        log << ex.second;
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
