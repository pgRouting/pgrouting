/*PGR-GNU*****************************************************************

FILE: solution.cpp

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

#include "pickDeliver/fleet.h"

#include <string>
#include <vector>
#include <limits>

#include "pickDeliver/pd_orders.h"
#include "pickDeliver/tw_node.h"
#include "pickDeliver/vehicle_pickDeliver.h"
#include "pickDeliver/pgr_pickDeliver.h"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/Dmatrix.h"


namespace pgrouting {
namespace vrp {
namespace pickdeliver {


Fleet::Fleet(const Fleet &fleet) :
    PD_problem(),
    m_trucks(fleet.m_trucks),
    used(fleet.used),
    un_used(fleet.un_used)
    {}

Fleet::Fleet(
        const std::vector<Vehicle_t> &vehicles) :
    PD_problem(),
    used(),
    un_used() {
        ENTERING();
        build_fleet(vehicles);
        Identifiers<size_t> unused(m_trucks.size());
        un_used = unused;
        EXITING();
    }


Vehicle_pickDeliver
Fleet::get_truck() {
    auto id = un_used.front();
    msg.log << "id" << id
        << "size" << m_trucks.size();
    pgassertwm(id < m_trucks.size(), msg.log.str());
    used += id;
    if (un_used.size() > 1) un_used -= id;
    return m_trucks[id];
}

void
Fleet::release_truck(size_t id) {
    used -= id;
    un_used += id;
}

Vehicle_pickDeliver
Fleet::get_truck(size_t order) {
    auto id = m_trucks.front().idx();
    for (auto truck : m_trucks) {
        if (truck.feasable_orders().has(order)) {
            id = truck.idx();
            msg.log << "id" << id
                << "size" << m_trucks.size();
            pgassertwm(id < m_trucks.size(), msg.get_log());
            used += id;
            if (un_used.size() > 1) un_used -= id;
            break;
        }
    }
    return m_trucks[id];
}


Vehicle_pickDeliver
Fleet::get_truck(const Order order) {
    ENTERING();
    auto id = m_trucks.front().idx();
    for (auto truck : m_trucks) {
        if (truck.feasable_orders().has(order.idx())) {
            id = truck.idx();
            msg.log << "id" << id
                << "size" << m_trucks.size();
            pgassertwm(id < m_trucks.size(), msg.get_log());
            used += id;
            if (un_used.size() > 1) un_used -= id;
            break;
        }
    }
    return m_trucks[id];
}


/*!
  builds a fleet from a vector of Vehicle_t 

  - creates a phoney truck with unlimited capacity and unlimited closing times
  - checks that the number of vehicles is a legal value
  - creates the requested vehicles

  ERROR when cant_v <= 0
*/
bool
Fleet::build_fleet(
        std::vector<Vehicle_t> vehicles) {
    ENTERING();
    /*
     *  creating a phoney truck with max capacity and max window
     *  with the start & end points of the first vehicle given
     */
    vehicles.push_back({
            /*
             * id, capacity
             */
            -1,
            std::numeric_limits<double>::infinity(),

            vehicles[0].speed,
            vehicles[0].start_x,
            vehicles[0].start_y,
            vehicles[0].start_node_id,

            /*
             * cant_v, start_open_t, start_close_t, start_service_t
             */
            1,
            0,
            std::numeric_limits<double>::infinity(),
            0,

            vehicles[0].end_x,
            vehicles[0].end_y,
            vehicles[0].end_node_id,
            /*
             * end_open_t, end_close_t, end_service_t
             */
            0,
            std::numeric_limits<double>::infinity(),
            0});


    for (auto vehicle : vehicles) {
        if (vehicle.cant_v <= 0) {
            msg.error << "Illegal number of vehicles found vehicle";
            msg.log << vehicle.cant_v << "<= 0 on vehicle " << vehicle.id;
            EXITING();
            return false;
        }

        auto starting_site = Vehicle_node(
                {problem->node_id()++, vehicle, Tw_node::NodeType::kStart});
        auto ending_site = Vehicle_node(
                {problem->node_id()++, vehicle, Tw_node::NodeType::kEnd});

        problem->add_node(starting_site);
        problem->add_node(ending_site);

        pgassert(starting_site.is_start() && ending_site.is_end());

        for (int i = 0; i < vehicle.cant_v; ++i) {
            m_trucks.push_back(Vehicle_pickDeliver(
                        m_trucks.size(),
                        vehicle.id,
                        starting_site,
                        ending_site,
                        vehicle.capacity,
                        vehicle.speed));
#if 0
            msg.log << "inserted Vehicle" << m_trucks.back().idx();
            msg.log << "\n";
#endif
            pgassert((m_trucks.back().idx() + 1)  == m_trucks.size());
        }
    }
    Identifiers<size_t> unused(m_trucks.size());
    un_used = unused;
    EXITING();
    return true;
}

/*!
 *
 * Not ok when:
 * - there was an error building the fleet
 * - a truck is not feasable
 *
 */
bool
Fleet::is_fleet_ok() const {
    ENTERING();
    if (!msg.get_error().empty()) return false;
    for (auto truck : m_trucks) {
        if (!truck.is_feasable()) {
            msg.error << "Vehicle is not feasible";
            msg.log << "Check vehicle #:" << truck.idx();
            EXITING();
            return false;
        }
    }
    EXITING();
    return true;
}

/**
 * Given an order,
 * Cycle trhugh all the trucks to verify if the order can be served by
 * at least one truck
 */
bool
Fleet::is_order_ok(const Order &order) const {
    ENTERING();
    for (const auto truck : m_trucks) {
#if 0
        if (!order.is_valid(truck.speed())) continue;
        if (truck.is_order_feasable(order)) {
            return true;
        }
        msg.log << "checking order " << order.id()
            << "on truck " << truck.id() << "\n";
#endif
        /*
         * The order must be valid given the speed
         */
        if (!order.is_valid(truck.speed())) continue; 

        /*
         * if its feasible, then the one truck is found
         */
        if (truck.is_order_feasable(order)) {
            EXITING();
            return true;
        }
    }
    EXITING();
    return false;
}

Vehicle_pickDeliver&
Fleet::operator[](size_t i) {
    pgassert(i < m_trucks.size());
    return m_trucks[i];
}

void
Fleet::set_compatibles(const PD_Orders &orders) {
    ENTERING();
    for (auto &truck : m_trucks) {
        truck.set_compatibles(orders);
    }
}

/*
 * FRIENDS
 */

std::ostream&
operator << (std::ostream &log, const Fleet &f) {
    log << "fleet\n";
    for (const auto v : f.m_trucks) {
        log << v;
    }
    log << "end fleet\n";

    return log;
}


}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting
