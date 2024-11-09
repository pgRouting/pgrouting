/*PGR-GNU*****************************************************************
File: pickDeliver.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

#include "vrp/pickDeliver.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "c_types/schedule_rt.h"

#include "cpp_common/assert.hpp"

#include "vrp/initials_code.hpp"
#include "vrp/vehicle_node.hpp"
#include "vrp/vehicle_pickDeliver.hpp"
#include "vrp/order.hpp"
#include "vrp/pd_orders.hpp"
#include "vrp/fleet.hpp"
#include "vrp/solution.hpp"
#include "vrp/initial_solution.hpp"
#include "vrp/optimize.hpp"

namespace pgrouting {
namespace vrp {



void
Pgr_pickDeliver::solve() {
    auto initial_sols = solutions;

    if (m_initial_id == 0) {
        msg.log << "trying all \n";
        for (int i = 1; i < 7; ++i) {
            initial_sols.push_back(Initial_solution((Initials_code)i, m_orders.size()));
            msg.log << "solution " << i << "\n" << initial_sols.back().tau();
            // TODO(vicky) calculate the time it takes
            msg.log << "Initial solution " << i
                << " duration: " << initial_sols.back().duration();
        }
    } else {
        msg.log << "only trying " << m_initial_id << "\n";
        initial_sols.push_back(Initial_solution((Initials_code)m_initial_id, m_orders.size()));
        // TODO(vicky) calculate the time it takes
        msg.log << "Initial solution " << m_initial_id
            << " duration: " << initial_sols[0].duration();
    }


    /*
     * Sorting solutions: the best is at the back
     */
    pgassert(!initial_sols.empty());
    std::sort(initial_sols.begin(), initial_sols.end(), []
            (const Solution &lhs, const Solution &rhs) -> bool {
            return rhs < lhs;
            });

    solutions.push_back(Optimize(initial_sols.back(), m_max_cycles));
    pgassert(!solutions.empty());

    msg.log << "best solution duration = " << solutions.back().duration();
}



std::vector< Schedule_rt >
Pgr_pickDeliver::get_postgres_result() const {
    auto result = solutions.back().get_postgres_result();

    Schedule_rt aggregates = {
            /*
             * Vehicle id = -2 indicates its an aggregate row
             *
             * (twv, cv, fleet, wait, duration)
             */
            -2,  // summary row on vehicle_number
            solutions.back().twvTot(),  // on vehicle_id
            solutions.back().cvTot(),   // on vehicle_seq
            -1,  // on order_id
            -1,  // on stop_id
            -2,  // on stop_type (gets increased later by one so it gets -1)
            -1,  // not accounting total loads
            solutions.back().total_travel_time(),
            -1,  // not accounting arrival_travel_time
            solutions.back().wait_time(),
            solutions.back().total_service_time(),
            solutions.back().duration(),
            };
    result.push_back(aggregates);


#ifndef NDEBUG
    for (const auto &sol : solutions) {
        msg.log << sol.tau();
    }
#endif
    return result;
}

void
Pgr_pickDeliver::add_node(const Vehicle_node &node) {
    m_nodes.push_back(node);
}

/** Constructor  for the matrix version
 *
 */
Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<Orders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        const pgrouting::tsp::Dmatrix &cost_matrix,
        double factor,
        size_t p_max_cycles,
        int initial) :
    PD_problem(this),
    m_initial_id(initial),
    m_max_cycles(p_max_cycles),
    m_nodes(),
    m_cost_matrix(cost_matrix),
    m_orders(pd_orders),
    m_trucks(vehicles, factor) {
        ENTERING(msg);
        pgassert(!pd_orders.empty());
        pgassert(!vehicles.empty());
        pgassert(!m_cost_matrix.empty());
        if (!(m_initial_id > 0 && m_initial_id < OneDepot)) {
            msg.log << "\n m_initial_id " << m_initial_id;
        }
        pgassertwm(m_initial_id > 0 && m_initial_id <= OneDepot, msg.get_log().c_str());
        pgassert(!m_nodes.empty());

        if (!msg.get_error().empty()) {
            return;
        }


        msg.log << "\n Checking fleet ...";
        if (!m_trucks.is_fleet_ok()) {
            pgassert(msg.get_error().empty());
            return;
        }
        msg.log << "fleet OK \n";


        /*
         * check the (S, P, D, E) order on all vehicles
         * stop when a feasible truck is found
         */
        msg.log << "\n Checking orders";
        for (const auto &o : m_orders) {
            if (!m_trucks.is_order_ok(o)) {
                msg.error << "Order not feasible on any truck was found";
                msg.log << "The order "
                    << o.id()
                    << " is not feasible on any truck";
                msg.log << "\n" << o;
                return;
            }
        }
        msg.log << "orders OK \n";

        m_trucks.set_compatibles(m_orders);
        EXITING(msg);
    }  //  constructor



}  //  namespace vrp
}  //  namespace pgrouting
