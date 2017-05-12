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

#include "pickDeliver/pgr_pickDeliver.h"

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "cpp_common/pgr_assert.h"

#include "pickDeliver/vehicle_node.h"
#include "pickDeliver/vehicle_pickDeliver.h"
#include "pickDeliver/order.h"
#include "pickDeliver/pd_orders.h"
#include "pickDeliver/fleet.h"
#include "pickDeliver/solution.h"
#include "pickDeliver/initial_solution.h"
#include "pickDeliver/optimize.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {



Solution
Pgr_pickDeliver::optimize(const Solution solution) {
    /*
     * Optimize a solution
     */
    Optimize opt_solution(solution, m_max_cycles);
    msg.log << opt_solution.best_solution.tau("optimized");
    return opt_solution.best_solution;
}

void
Pgr_pickDeliver::solve() {
    ENTERING();
    auto initial_sols = solutions;

    if (m_initial_id == 0) {
        msg.log << "trying all \n";
    return;
        for (int i = 1; i < 7; ++i) {
            initial_sols.push_back(Initial_solution(i, m_orders.size()));
            msg.log << "solution " << i << "\n" << initial_sols.back().tau();
            // TODO calculate the time it takes
            msg.log << "Initial solution " << i << " duration: " << initial_sols.back().duration();
        }
    } else {
        msg.log << "only trying " << m_initial_id << "\n";
        initial_sols.push_back(Initial_solution(m_initial_id, m_orders.size()));
        // TODO calculate the time it takes
        msg.log << "Initial solution " << m_initial_id << " duration: " << initial_sols[0].duration();
        EXITING();
        return;
    }


    /*
     * Sorting solutions: the best is at the back
     */
    pgassert(!initial_sols.empty());
    std::sort(initial_sols.begin(), initial_sols.end(), []
            (const Solution &lhs, const Solution &rhs) -> bool {
            EXITING();
            return rhs < lhs;
            });

#if 0
    solutions.push_back(Optimize(initial_sols.back()));
    pgassert(!solutions.empty());
#endif
    msg.log << "best solution duration = " << solutions.back().duration();
    EXITING();
}



std::vector< General_vehicle_orders_t >
Pgr_pickDeliver::get_postgres_result() const {
    if (solutions.empty()) return std::vector< General_vehicle_orders_t >();
    auto result = solutions.back().get_postgres_result();

    General_vehicle_orders_t aggregates = {
            /*
             * Vehicle id = -2 indicates its an aggregate row
             *
             * (twv, cv, fleet, wait, duration)
             */
            -2,  // summary row on vehicle_number
            solutions.back().twvTot(), // on vehicle_id
            solutions.back().cvTot(),  // on vehicle_seq
            -1,  // on order_id
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
 * - Builds the:
 *    matrix
 *    fleet
 *    orders
 *    nodes
 *
 * - Checks the:
 *   fleet
 */

Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        const pgrouting::tsp::Dmatrix &cost_matrix,
        size_t p_max_cycles,
        int initial) :
    PD_problem(this),
    m_cost_matrix(cost_matrix), 
    m_initial_id(initial),
    m_max_cycles(p_max_cycles),
    /*
     * the problem has cost_matrix.size() nodes
     */
    m_node_id(0),
    m_nodes(),
    m_trucks(vehicles),
    m_orders(pd_orders)
{
    ENTERING();
    pgassert(!pd_orders.empty());
    pgassert(!vehicles.empty());
    pgassert(!cost_matrix.empty());
    pgassert(m_initial_id > 0 && m_initial_id < 7);

    msg.log << "\n *** Constructor for the matrix version ***\n";
    msg.log << "Check problem has no errors\n";

    msg.error << m_trucks.msg.get_error();
    msg.error << m_orders.msg.get_error();
    if (msg.get_error().empty()) {
        /*
         * data has errors
         */
        msg.log << m_trucks.msg.get_log();
        msg.log << m_orders.msg.get_log();
        return;
    }
    pgassert(msg.get_error().empty());
    pgassert(m_trucks.msg.get_error().empty());
    pgassert(m_orders.msg.get_error().empty());


    /* 
     * checking the fleet
     */
    if (!m_trucks.is_fleet_ok()) {
        msg.error << m_trucks.msg.get_error();
        msg.log << m_trucks.msg.get_log();
        return;
    }

    /*
     * check the (S, P, D, E) order on all vehicles
     * stop when a feasible Vehicle is found
     */
    for (const auto &o : m_orders) {
        if (!m_trucks.is_order_ok(o)) {
            msg.error << "An order is not feasible in all vehicles";
            // TODO should be id()
            msg.log << "Check order #" << o.idx() << "\n"
                << o << "\n";
            return;
        }
    }

    m_trucks.set_compatibles(m_orders);
}  //  constructor



/***** Constructor for the eculedian version *******/

Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        size_t p_max_cycles,
        int initial) :
    PD_problem(this),
    m_initial_id(initial),
    m_max_cycles(p_max_cycles),
    /*
     * the problem has unknown number of nodes
     */
    m_node_id(0),
    m_nodes(),
    m_trucks(vehicles)
{
    ENTERING();
    pgassert(!pd_orders.empty());
    pgassert(!vehicles.empty());
    pgassert(m_initial_id > 0 && m_initial_id < 7);

    std::ostringstream tmplog;

    msg.log << "\n *** Constructor of problem ***\n";

    if (!msg.get_error().empty()) {
        return;
    }

    pgassert(msg.get_error().empty());
    msg.log << "\n Building fleet";
    if (!m_trucks.is_fleet_ok()) {
        pgassert(msg.get_error().empty());
        pgassert(!m_trucks.msg.get_error().empty());
        msg.error << m_trucks.msg.get_error();
        return;
    }



#ifndef NDEBUG
    for (const auto t : m_trucks) {
        msg.log << t << "\n";
    }
#endif

    msg.log << "\n Building orders";
    m_orders.build_orders(pd_orders);
    msg.log << " ---> OK\n";


#ifndef NDEBUG
    for (const auto &o : m_orders) {
        msg.log << o << "\n";
    }
#endif

    /*
     * check the (S, P, D, E) order on all vehicles
     * stop when a feasible truck is found
     */
    for (const auto &o : m_orders) {
        if (!m_trucks.is_order_ok(o)) {
            msg.error << "The order "
                << o.pickup().id()
                << " is not feasible on any truck";
            msg.log << "\n" << o;
            return;
        }
    }

    m_trucks.set_compatibles(m_orders);
}  //  constructor


const Order
Pgr_pickDeliver::order_of(const Vehicle_node &node) const {
    pgassert(node.is_pickup() ||  node.is_delivery());
    if (node.is_pickup()) {
        for (const auto o : m_orders) {
            if (o.pickup().idx() == node.idx()) {
                return o;
            }
        }
    }
    pgassert(node.is_delivery());

    for (const auto o : m_orders) {
        if (o.delivery().idx() == node.idx()) {
            return o;
        }
    }
#ifndef NDEBUG
    std::ostringstream err_log;
    err_log << "Order of" << node << " not found";
#endif
    pgassertwm(false, err_log.str());
    return m_orders[0];
}


const Vehicle_node&
Pgr_pickDeliver::node(ID id) const {
    pgassert(id < m_nodes.size());
    msg.log << "id = " << id << "m_nodes[id].idx()" << m_nodes[id].idx() << "\n";
    pgassertwm(id == m_nodes[id].idx(), msg.get_log().c_str());
    return m_nodes[id];
}

std::ostream& operator<<(
        std::ostream &log,
        const Pgr_pickDeliver &pd_prob) {
#if 0
    log << "The matrix\n" << pd_prob.m_cost_matrix;
    for (const auto n : pd_prob.m_nodes) {
        log << n << "\n";
    }
#endif
    log << "The vehicles\n" << pd_prob.m_trucks;

    log << "The nodes\n";
    log << "The orders\n" << pd_prob.m_orders;
    return log;

}

}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting
