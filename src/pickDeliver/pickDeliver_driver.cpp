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

#include <sstream>
#include <string>
#include <deque>
#include <vector>
#include <utility>

#include "vrp/pgr_pickDeliver.h"
#include "vrp/initials_code.h"
#include "cpp_common/pggetdata.hpp"
#include "cpp_common/Dmatrix.h"

#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"

#include "c_types/iid_t_rt.h"
#include "c_types/pickDeliver/orders_t.h"
#include "c_types/pickDeliver/vehicle_t.h"
#include "c_types/pickDeliver/schedule_rt.h"

void
pgr_do_pickDeliver(
        char* customers_sql,
        char *vehicles_sql,
        char *matrix_sql,

        double factor,
        int max_cycles,
        int initial_solution_id,

        Schedule_rt **return_tuples,
        size_t *return_count,

        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    char *hint;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(*return_count == 0);
        pgassert(!(*return_tuples));

        hint = matrix_sql;
        auto orders = pgrouting::pgget::get_orders(std::string(customers_sql), true);
        if (orders.size() == 0) {
            *notice_msg = pgr_msg("Insufficient data found on inner query");
            *log_msg = hint? pgr_msg(hint) : nullptr;
            return;
        }

        hint = matrix_sql;
        auto vehicles = pgrouting::pgget::get_vehicles(std::string(vehicles_sql), true);
        if (vehicles.size() == 0) {
            *notice_msg = pgr_msg("Insufficient data found on inner query");
            *log_msg = hint? pgr_msg(hint) : nullptr;
            return;
        }

        hint = matrix_sql;
        auto data_costs = pgrouting::pgget::get_matrixRows(std::string(matrix_sql));

        if (data_costs.size() == 0) {
            *notice_msg = pgr_msg("Insufficient data found on inner query");
            *log_msg = hint? pgr_msg(hint) : nullptr;
            return;
        }
        hint = nullptr;

        pgrouting::tsp::Dmatrix cost_matrix(data_costs);

        auto depot_node = vehicles[0].start_node_id;

        /*
         * This applies to the one depot problem
         */
        if ((pgrouting::vrp::Initials_code)(initial_solution_id) == pgrouting::vrp::OneDepot) {
            /*
             * All Vehicles must depart from same location
             */
            for (const auto &v : vehicles) {
                if (v.start_node_id != depot_node && v.end_node_id != depot_node) {
                    err << "All vehicles must depart & arrive to same node";
                    *err_msg = pgr_msg(err.str().c_str());
                    return;
                }
            }

            /*
             * All Orders must depart from depot
             */
            for (const auto &o : orders) {
                if (o.pick_node_id != depot_node) {
                    err << "All orders must be picked at depot";
                    *err_msg = pgr_msg(err.str().c_str());
                    return;
                }
            }
        }

        if (!cost_matrix.has_no_infinity()) {
            err << "An Infinity value was found on the Matrix";
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }

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
            log << pd_problem.msg.get_log();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
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
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch (const std::pair<std::string, std::string>& ex) {
        (*return_count) = 0;
        err << ex.first;
        log.str("");
        log.clear();
        log << ex.second;
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (const std::pair<std::string, int64_t>& ex) {
        (*return_count) = 0;
        err << ex.first;
        log.str("");
        log.clear();
        log << "Node missing on matrix: id =  " << ex.second;
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
