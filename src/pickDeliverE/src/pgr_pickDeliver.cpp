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


#if 1
    // TODO(vicky) delete this function
bool
Pgr_pickDeliver::nodesOK() const {
    ENTERING();
    if (m_nodes.empty() && m_base_nodes.empty()) return true;

    msg.log << "     m_nodes: " << m_nodes.size();
    for (const auto n : m_nodes) {
        msg.log << n.id() << ",";
    }

    msg.log << "\n";

    msg.log << "m_base_nodes: " << m_base_nodes.size();
    for (auto const &n : m_base_nodes) {
        msg.log << n->id() << ",";
    }

    pgassertwm(m_nodes.size() == m_base_nodes.size(), msg.get_log().c_str());
    msg.log << "\n";

    for (size_t i = 0; i < m_nodes.size() ; ++i) {
        pgassertwm(m_nodes[i].id() ==  m_base_nodes[i]->id(),
                msg.get_log().c_str());
        pgassertwm(m_nodes[i].idx() ==  m_base_nodes[i]->idx(),
                msg.get_log().c_str());
    }
    return true;
}
#endif

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

    solutions.push_back(Optimize(initial_sols.back()));
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
    /*
     * the problem has cost_matrix.size() nodes
     */
    m_node_id(0),
    m_nodes(),
    m_cost_matrix(cost_matrix),
    m_trucks(vehicles, factor) {
        pgassert(msg.get_error().empty());

        pgassert(!pd_orders.empty());
        pgassert(!vehicles.empty());
        pgassert(!cost_matrix.empty());
        pgassert(m_initial_id > 0 && m_initial_id < 7);

        pgassert(msg.get_error().empty());
        std::ostringstream tmplog;

        msg.log << "\n *** Constructor for the matrix version ***\n";

        if (!msg.get_error().empty()) {
            return;
        }

        pgassert(msg.get_error().empty());

        if (!m_trucks.is_fleet_ok()) {
            // TODO(vicky) revise the function
            pgassert(false);
            msg.error << m_trucks.msg.get_error();
            return;
        }
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
    /*
     * the problem has unknown number of nodes
     */
    m_node_id(0),
    m_nodes(),
    m_base_nodes(),
    m_orders(pd_orders),
    m_trucks(vehicles, factor) {
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
                << o.pickup().order()
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
    msg.log << "id = " << id
        << "m_nodes[id].idx()" << m_nodes[id].idx() << "\n";
    pgassertwm(id == m_nodes[id].idx(), msg.get_log().c_str());
    return m_nodes[id];
}


}  //  namespace vrp
}  //  namespace pgrouting
