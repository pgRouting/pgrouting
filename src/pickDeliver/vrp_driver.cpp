/*PGR-GNU*****************************************************************
File: vrp_driver.cpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#include "drivers/vrp_driver.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <cstdint>

#include "c_types/schedule_rt.h"
#include "c_types/iid_t_rt.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/Dmatrix.hpp"

#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "vrp/pickDeliver.hpp"
#include "vrp/initials_code.hpp"
#include "cpp_common/orders_t.hpp"
#include "cpp_common/vehicle_t.hpp"

namespace pgrouting {
namespace drivers {

void do_vrp(
        const std::string &customers_sql,
        const std::string &vehicles_sql,
        const std::string &matrix_sql,

        double factor,
        int max_cycles,
        int initial_solution_id,

        Which which,
        Schedule_rt* &return_tuples,
        size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    using pgrouting::pgr_alloc;

    std::string hint = "";
    return_tuples = nullptr;
    return_count = 0;

    try {
        using pgrouting::pgget::get_orders;
        using pgrouting::pgget::get_vehicles;
        using pgrouting::pgget::get_matrixRows;

        hint = customers_sql;
        auto orders = get_orders(customers_sql, which == PICKDELIVER);
        if (orders.size() == 0) {
            notice << "Insufficient data found on inner query";
            log << customers_sql;
            return;
        }

        hint = vehicles_sql;
        auto vehicles = get_vehicles(vehicles_sql, which == PICKDELIVER);
        if (vehicles.size() == 0) {
            notice << "Insufficient data found on inner query";
            log << vehicles_sql;
            return;
        }

        hint = matrix_sql;
        auto data_costs = which == PICKDELIVER? get_matrixRows(matrix_sql) : std::vector<IID_t_rt>();

        if (which == PICKDELIVER && data_costs.size() == 0) {
            notice << "Insufficient data found on inner query";
            log << matrix_sql;
            return;
        }

        hint = "";



        auto depot_node = which == PICKDELIVER? vehicles[0].start_node_id : 0;

        /*
         * This applies to the one depot problem
         */
        if (which == PICKDELIVER && (pgrouting::vrp::Initials_code)(initial_solution_id) == pgrouting::vrp::OneDepot) {
            /*
             * All Vehicles must depart from same location
             */
            for (const auto &v : vehicles) {
                if (v.start_node_id != depot_node && v.end_node_id != depot_node) {
                    err << "All vehicles must depart & arrive to same node";
                    return;
                }
            }

            /*
             * All Orders must depart from depot
             */
            for (const auto &o : orders) {
                if (o.pick_node_id != depot_node) {
                    err << "All orders must be picked at depot";
                    return;
                }
            }
        }

        pgrouting::tsp::Dmatrix cost_matrix(data_costs);

        if (which == PICKDELIVER && !cost_matrix.has_no_infinity()) {
            err << "An Infinity value was found on the Matrix";
            return;
        }

        std::map<std::pair<double, double>, int64_t> matrix_data;
        pgrouting::Identifiers<int64_t> unique_ids;

        if (which == PICKDELIVEREUCLIDEAN) {
        for (const auto &o : orders) {
            matrix_data[std::pair<double, double>(o.pick_x, o.pick_y)] = o.pick_node_id;
            matrix_data[std::pair<double, double>(o.deliver_x, o.deliver_y)] = o.deliver_node_id;
        }

        for (const auto &v : vehicles) {
            matrix_data[std::pair<double, double>(v.start_x, v.start_y)] = v.start_node_id;
            matrix_data[std::pair<double, double>(v.end_x, v.end_y)] = v.end_node_id;
        }

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
}

        pgrouting::tsp::Dmatrix cost_matrix_euclidean(matrix_data);

        log << "Initialize problem\n";
        pgrouting::vrp::Pgr_pickDeliver pd_problem(
                orders,
                vehicles,
                which == PICKDELIVER? cost_matrix : cost_matrix_euclidean,
                factor,
                static_cast<size_t>(max_cycles),
                initial_solution_id);

        err << pd_problem.msg.get_error();
        if (!err.str().empty()) {
            log << pd_problem.msg.get_log();
            return;
        }

        log << pd_problem.msg.get_log();
        log << "Finish Reading data\n";
        pd_problem.msg.clear();


        try {
            pd_problem.solve();
        } catch (AssertFailedException &except) {
            log << pd_problem.msg.get_log();
            pd_problem.msg.clear();
            throw;
        } catch(...) {
            log << "Caught unknown exception!";
            throw;
        }


        log << pd_problem.msg.get_log();
        log << "Finish solve\n";
        pd_problem.msg.clear();

        auto solution = pd_problem.get_postgres_result();
        log << pd_problem.msg.get_log();
        pd_problem.msg.clear();
        log << "solution size: " << solution.size() << "\n";


        if (!solution.empty()) {
            return_tuples = pgr_alloc(solution.size(), (return_tuples));
            int seq = 0;
            for (const auto &row : solution) {
                return_tuples[seq] = row;
                ++seq;
            }
        }
        return_count = solution.size();
    } catch (AssertFailedException &except) {
        err << except.what();
    } catch (const std::pair<std::string, std::string>& ex) {
        err << ex.first;
        log << ex.second;
    } catch (const std::string &ex) {
        err << ex;
        log << hint;
    } catch (std::exception &except) {
        err << except.what();
    } catch (const std::pair<std::string, int64_t>& ex) {
        err << ex.first;
        log << "Node missing on matrix: id = " << ex.second;
    } catch(...) {
        err << "Caught unknown exception!";
    }
}

}  // namespace drivers
}  // namespace pgrouting
