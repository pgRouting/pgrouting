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
Pgr_pickDeliver::solve(const Solution init_solution) {
    Optimize solution(0, init_solution);
    solution.decrease_truck();
    solution.move_duration_based();
    solution.move_wait_time_based();
    solution.inter_swap();
    return solution.best_solution;
}

void
Pgr_pickDeliver::solve() {
#if 0
    solutions.push_back(Initial_solution(0, this));
#endif
    solutions.push_back(Initial_solution(1));

    solutions.push_back(solve(solutions.back()));

#if 1
    solutions.push_back(Initial_solution(2));
    solutions.push_back(solve(solutions.back()));
    solutions.push_back(Initial_solution(3));
    solutions.push_back(solve(solutions.back()));
    solutions.push_back(Initial_solution(4));
    solutions.push_back(solve(solutions.back()));
    solutions.push_back(Initial_solution(5));
    solutions.push_back(solve(solutions.back()));
    solutions.push_back(Initial_solution(6));
    solutions.push_back(solve(solutions.back()));
#endif

#if 1
    /*
     * Sorting solutions: the best is at the back
     */
    std::sort(solutions.begin(), solutions.end(), []
            (const Solution &lhs, const Solution &rhs) -> bool {
            return rhs < lhs;
            });
#endif
}



std::vector< General_vehicle_orders_t >
Pgr_pickDeliver::get_postgres_result() const {
    auto result = solutions.back().get_postgres_result();

    General_vehicle_orders_t aggregates = {
            /*
             * Vehicle id = -1 indicates its an aggregate row
             *
             * (twv, cv, fleet, wait, duration)
             */
            -1,
            solutions.back().twvTot(),
            solutions.back().cvTot(),
            -1,  // summary
            0,  // not accounting total loads
            solutions.back().total_travel_time(),
            0,  // not accounting arrival_travel_time
            solutions.back().wait_time(),
            solutions.back().total_service_time(),
            solutions.back().duration(),
            };
    result.push_back(aggregates);


#ifndef NDEBUG
    for (const auto sol : solutions) {
        log << sol.tau();
    }
#endif
    return result;
}



/***** Constructor *******/

Pgr_pickDeliver::Pgr_pickDeliver(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        const std::vector<Vehicle_t> &vehicles,
        size_t p_max_cycles,
        std::string &err) 
{
    PD_problem(this);
    pgassert(!pd_orders.empty());
    pgassert(!vehicles.empty());


    m_max_cycles = p_max_cycles;
    pgassert(m_max_cycles > 0);
    std::ostringstream tmplog;
    err = "";

#if 0
    m_speed = m_trucks.m_trucks[0].speed();
#endif
    log << "\n *** Constructor of problem ***\n";

    size_t node_id(0);
    if (!m_trucks.build_fleet(vehicles, node_id)
            || !m_trucks.is_fleet_ok()) {
        error << m_trucks.get_error();
        err = error.str();
        return;
    };

#if 1
    m_speed = m_trucks.m_trucks[0].speed();
    pgassert(m_trucks.m_trucks[0].end_site().is_end());
    pgassert(m_trucks.m_trucks[0].end_site().is_end());

    m_starting_site = m_trucks.m_trucks[0].start_site();
    m_ending_site = m_trucks.m_trucks[0].end_site();
    max_vehicles = m_trucks.m_trucks.size();
    max_capacity = m_trucks.m_trucks[0].capacity();
    pgassert(m_starting_site.is_start());
    pgassert(m_ending_site.is_end());
#endif

    m_orders.build_orders(pd_orders, node_id);
#if 0
    log << "validating orders";
    if (!m_orders.is_valid()) {
        error << m_orders.get_error();
        err = error.str();
        return;
    };
#endif

    /*
     * check the (S, P, D, E) order on all vehicles
     * stop when a feasable truck is found
     */
    for (const auto &o : m_orders) {
        if (!m_trucks.is_order_ok(o)) {
            error << "The Order "
                << o.pickup().id()
                << " is not feasible on any truck";
            err = error.str();
            return;
        }
    }

#if 1
    for (auto &o : m_orders) {
        o.setCompatibles(m_speed);
    }
#endif


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
