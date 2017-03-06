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

#include "./pgr_pickDeliver.h"

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_assert.h"

#include "./vehicle_node.h"
#include "./vehicle_pickDeliver.h"
#include "./order.h"
#include "./orders.h"
#include "./fleet.h"
#include "./solution.h"
#include "./initial_solution.h"
#include "./optimize.h"

namespace pgrouting {
namespace vrp {



Solution
Pgr_pickDeliver::optimize(const Solution init_solution) {
    /*
     * OPtimize the initial solution
     */
    Optimize solution(init_solution);
#if 0
    solution.decrease_truck();
    solution.move_duration_based();
    solution.move_wait_time_based();
    solution.inter_swap();
#endif
    log << solution.best_solution.tau("optimized");
    return solution.best_solution;
}

void
Pgr_pickDeliver::solve() {
    auto initial_sols = solutions;

    if (m_initial_id == 0) {
        log << "trying all \n";
        for (int i = 1; i < 7; ++i) {
            initial_sols.push_back(Initial_solution(i, m_orders.size()));
            log << "solution " << i << "\n" << initial_sols.back().tau();
        }
    } else {
        log << "only trying " << m_initial_id << "\n";
        initial_sols.push_back(Initial_solution(m_initial_id, m_orders.size()));
    }

    log << "one order per truck duration = " << initial_sols[0].duration();

    /*
     * Sorting solutions: the best is at the back
     */
    pgassert(!initial_sols.empty());
    std::sort(initial_sols.begin(), initial_sols.end(), []
            (const Solution &lhs, const Solution &rhs) -> bool {
            return rhs < lhs;
            });

    solutions.push_back(Optimize(initial_sols.back()));

    pgassert(solutions.size() == 1);
    log << "best solution duration = " << solutions.back().duration();
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
            -2,
            solutions.back().twvTot(),
            solutions.back().cvTot(),
            -1,
            -1,  // summary
            -1,  // not accounting total loads
            solutions.back().total_travel_time(),
            -1,  // not accounting arrival_travel_time
            solutions.back().wait_time(),
            solutions.back().total_service_time(),
            solutions.back().duration(),
            };
    result.push_back(aggregates);


#if 0
#ifndef NDEBUG
    for (const auto sol : solutions) {
        log << sol.tau();
    }
#endif
#endif
    return result;
}



/***** Constructor *******/

Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        size_t p_max_cycles,
        int initial,
        std::string &err) :
    m_initial_id(initial),
    m_max_cycles(p_max_cycles),
    m_node_id(0)
{
    PD_problem(this);
    pgassert(!pd_orders.empty());
    pgassert(!vehicles.empty());
    pgassert(m_max_cycles > 0);
    pgassert(m_initial_id > 0 && m_initial_id < 7);

    std::ostringstream tmplog;
    err = "";

    log << "\n *** Constructor of problem ***\n";

    if (!m_trucks.build_fleet(vehicles)
            || !m_trucks.is_fleet_ok()) {
        error << m_trucks.get_error();
        err = error.str();
        return;
    }


    m_orders.build_orders(pd_orders);

    /*
     * check the (S, P, D, E) order on all vehicles
     * stop when a feasable truck is found
     */
    for (const auto &o : m_orders) {
        if (!m_trucks.is_order_ok(o)) {
            error << "The order "
                << o.pickup().original_id()
                << " is not feasible on any truck";
            err = error.str();
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
            if (o.pickup().id() == node.id()) {
                return o;
            }
        }
    }
    pgassert(node.is_delivery());

    for (const auto o : m_orders) {
        if (o.delivery().id() == node.id()) {
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
    pgassert(id == m_nodes[id].id());
    return m_nodes[id];
}


}  //  namespace vrp
}  //  namespace pgrouting
