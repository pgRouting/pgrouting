/*PGR-GNU*****************************************************************

FILE: pgr_pickDeliver.cpp

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

#include "vrp/pgr_pickDeliver.h"

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "cpp_common/pgr_assert.h"

#include "vrp/vehicle_node.h"
#include "vrp/vehicle_pickDeliver.h"
#include "vrp/order.h"
#include "vrp/pd_orders.h"
#include "vrp/fleet.h"
#include "vrp/solution.h"
#include "vrp/initial_solution.h"
#include "vrp/optimize.h"

namespace pgrouting {
namespace vrp {


// TODO(vicky) delete this function
bool
Pgr_pickDeliver::nodesOK() const {
    ENTERING();
    if (m_nodes.empty() && m_base_nodes.empty()) return true;

    pgassertwm(m_nodes.size() == m_base_nodes.size(), msg.get_log().c_str());
    for (size_t i = 0; i < m_nodes.size() ; ++i) {
        pgassertwm(m_nodes[i].id() ==  m_base_nodes[i]->id(),
                msg.get_log().c_str());
        pgassertwm(m_nodes[i].idx() ==  m_base_nodes[i]->idx(),
                msg.get_log().c_str());
    }
    EXITING();
    return true;
}

Solution
Pgr_pickDeliver::optimize(const Solution solution) {
    pgassert(false);
    /*
     * Optimize a solution
     */
#if 1
    msg.log << "max_cycles: " << m_max_cycles << "\n";
    Optimize opt_solution(solution, m_max_cycles);
#else
    Optimize opt_solution(solution, 1);
#endif
    msg.log << opt_solution.best_solution.tau("optimized");
    return opt_solution.best_solution;
}

void
Pgr_pickDeliver::solve() {
    auto initial_sols = solutions;

    if (m_initial_id == 0) {
        msg.log << "trying all \n";
        for (int i = 1; i < 7; ++i) {
            initial_sols.push_back(Initial_solution(i, m_orders.size()));
            msg.log << "solution " << i << "\n" << initial_sols.back().tau();
            // TODO(vicky) calculate the time it takes
            msg.log << "Initial solution " << i
                << " duration: " << initial_sols.back().duration();
        }
    } else {
        msg.log << "only trying " << m_initial_id << "\n";
        initial_sols.push_back(Initial_solution(m_initial_id, m_orders.size()));
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

#if 1
    solutions.push_back(Optimize(initial_sols.back(), m_max_cycles));
#else
    solutions.push_back(initial_sols.back());
#endif
    pgassert(!solutions.empty());

    msg.log << "best solution duration = " << solutions.back().duration();
}



std::vector< General_vehicle_orders_t >
Pgr_pickDeliver::get_postgres_result() const {
    auto result = solutions.back().get_postgres_result();

    General_vehicle_orders_t aggregates = {
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
    for (const auto sol : solutions) {
        msg.log << sol.tau();
    }
#endif
    return result;
}

/** Constructor  for the matrix version
 *
 */
Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        const pgrouting::tsp::Dmatrix &cost_matrix,
        double factor,
        size_t p_max_cycles,
        int initial) :
    PD_problem(this),
    m_initial_id(initial),
    m_max_cycles(p_max_cycles),
    m_node_id(0),
    m_nodes(),
    m_base_nodes(),
    m_cost_matrix(cost_matrix),
    m_orders(pd_orders),
    m_trucks(vehicles, factor) {
        ENTERING();
        pgassert(!pd_orders.empty());
        pgassert(!vehicles.empty());
        pgassert(!m_cost_matrix.empty());
        pgassert(m_initial_id > 0 && m_initial_id < 7);
        pgassert(nodesOK());

        if (!msg.get_error().empty()) {
            return;
        }

        pgassert(m_trucks.msg.get_error().empty());
        pgassert(msg.get_error().empty());

        msg.log << "\n Checking fleet ...";
        if (!m_trucks.is_fleet_ok()) {
            pgassert(msg.get_error().empty());
            pgassert(!m_trucks.msg.get_error().empty());
            msg.error << m_trucks.msg.get_error();
            return;
        }
        msg.log << "fleet OK \n";

#if 0
        for (const auto t : m_trucks) {
            msg.log << t << "\n";
        }
        for (const auto &o : m_orders) {
            msg.log << o << "\n";
        }
#endif

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
#if 0
                double old_speed(0);
                for (auto t : m_trucks) {
                    if (old_speed == t.speed()) continue;
                    old_speed = t.speed();
                    msg.log << "****** With speed: " << t.speed() << "\n";
                    msg.log << t.start_site() << "\n";
                    msg.log << o.pickup() << "\n";
                    msg.log << "travel time to "
                        << t.start_site().travel_time_to(
                                o.pickup(), t.speed()) << "\n";

                    msg.log << o.delivery() << "\n";
                    msg.log << t.end_site() << "\n";
                    msg.log << "travel time to "
                        << t.start_site().travel_time_to(
                                o.delivery(), t.speed())
                        << "\n";
                    t.push_back(o);
                }
#endif
                return;
            }
        }
        msg.log << "orders OK \n";

        m_trucks.set_compatibles(m_orders);
#if 0
        msg.error << "foo";
        for (const auto t : m_trucks) {
            msg.log << t << "\n";
        }
#endif
        EXITING();
    }  //  constructor



/***** Constructor for the eculedian version *******/

Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        double factor,
        size_t p_max_cycles,
        int initial) :
    PD_problem(this),
    m_initial_id(initial),
    m_max_cycles(p_max_cycles),
    m_node_id(0),
    m_nodes(),
    m_base_nodes(),
    m_cost_matrix(),
    m_orders(pd_orders),
    m_trucks(vehicles, factor) {
        ENTERING();
        pgassert(!pd_orders.empty());
        pgassert(!vehicles.empty());
        pgassert(m_cost_matrix.empty());
        pgassert(factor > 0);
        pgassert(m_initial_id > 0 && m_initial_id < 7);

        if (!msg.get_error().empty()) {
            return;
        }

        pgassert(m_trucks.msg.get_error().empty());
        pgassert(msg.get_error().empty());

        msg.log << "\n Checking fleet";
        if (!m_trucks.is_fleet_ok()) {
            msg.error << m_trucks.msg.get_error();
            pgassert(!m_trucks.msg.get_error().empty());
            return;
        }

        pgassert(m_trucks.msg.get_error().empty());


#ifndef NDEBUG
        for (const auto t : m_trucks) {
            msg.log << t << "\n";
        }
        for (const auto &o : m_orders) {
            msg.log << o << "\n";
        }
#endif

        /*
         * check the (S, P, D, E) order on all vehicles
         * stop when a feasible truck is found
         */
        msg.log << "\n Checking orders";
        for (const auto &o : m_orders) {
            if (!m_trucks.is_order_ok(o)) {
                msg.error << "Order not feasible on any truck was found";
                msg.log << "The order "
                    << o.pickup().order()
                    << " is not feasible on any truck";
                msg.log << "\n" << o;
                return;
            }
        }

        m_trucks.set_compatibles(m_orders);
        EXITING();
    }  //  constructor


}  //  namespace vrp
}  //  namespace pgrouting
