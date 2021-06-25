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

#include "vrp/fleet.h"

#include <vector>
#include <memory>
#include <utility>
#include <limits>
#include <string>

#include "vrp/dnode.h"
#include "vrp/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {


Pgr_pickDeliver* Fleet::problem;

/**
* @returns reference to the problem's message
*/
Pgr_messages&
Fleet::msg() {
    return problem->msg;
}

Fleet::Fleet(const Fleet &fleet) :
    m_trucks(fleet.m_trucks),
    m_used(fleet.m_used),
    m_un_used(fleet.m_un_used)
    {}

Fleet::Fleet(
        const std::vector<Vehicle_t> &vehicles, double factor) :
    m_used(),
    m_un_used() {
        build_fleet(vehicles, factor);
        Identifiers<size_t> unused(m_trucks.size());
        m_un_used = unused;
    }


Vehicle_pickDeliver
Fleet::get_truck() {
    ENTERING(msg());
    auto idx = m_un_used.front();
    msg().log << "Available vehicles: " << m_un_used << "\n";
    msg().log << "NOT Available vehicles: " << m_used << "\n";
    msg().log << "getting idx" << idx << "\n";
    pgassertwm(idx < m_trucks.size(), msg().log.str());
    m_used += idx;
    if (m_un_used.size() > 1) m_un_used -= idx;
    EXITING(msg());
    return m_trucks[idx];
}


Vehicle_pickDeliver
Fleet::get_truck(size_t order) {
#if 0
    msg().log << "Available vehicles: " << m_un_used << "\n";
    msg().log << "NOT Available vehicles: " << m_used << "\n";
#endif
    auto idx = m_un_used.front();

    for (const auto &i : m_un_used) {
        if (m_trucks[i].feasable_orders().has(order)) {
            idx = i;
            msg().log << "getting idx" << idx << "\n";
            m_used += idx;
            if (m_un_used.size() > 1) m_un_used -= idx;
            return m_trucks[idx];
        }
    }

    /*
     * using phoney truck
     */
    pgassert(false);
    return m_trucks.back();

    for (auto truck : m_trucks) {
        if (truck.feasable_orders().has(order)) {
            idx = truck.idx();
            msg().log << "idx" << idx << "size" << m_trucks.size();
            pgassertwm(idx < m_trucks.size(), msg().get_log());
            m_used += idx;
            if (m_un_used.size() > 1) m_un_used -= idx;
            break;
        }
    }
    return m_trucks[idx];
}




void
Fleet::add_vehicle(
        Vehicle_t vehicle,
        double factor,
        const Vehicle_node &starting_site,
        const Vehicle_node &ending_site) {
    pgassert(starting_site.is_start() && ending_site.is_end());
    pgassert(starting_site.opens() <= starting_site.closes());
    pgassert(ending_site.opens() <= ending_site.closes());


    for (int i = 0; i < vehicle.cant_v; ++i) {
        m_trucks.push_back(Vehicle_pickDeliver(
                    m_trucks.size(),
                    vehicle.id,
                    starting_site,
                    ending_site,
                    vehicle.capacity,
                    vehicle.speed,
                    factor));
#if 0
        msg().log << "inserting vehicle: " << m_trucks.back().tau() << "\n";
#endif
        pgassert((m_trucks.back().idx() + 1)  == m_trucks.size());
        pgassert(m_trucks.back().is_ok());
    }
}

/*!
  builds a fleet from a vector of Vehicle_t

  - creates a phoney truck with unlimited capacity and unlimited closing times
  - checks that the number of vehicles is a legal value
  - creates the requested vehicles

  @param[in] vehicles  the list of vehicles
  @param[in] factor    the multiplier to speed up or slow down
  */
bool
Fleet::build_fleet(
        std::vector<Vehicle_t> vehicles,
        double factor) {
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
        if (vehicle.cant_v < 0) {
            throw std::make_pair(std::string("Illegal number of vehicles found"), vehicle.cant_v);
        }

        if (vehicle.capacity < 0) {
            throw std::make_pair(std::string("Illegal value for capacity found"), vehicle.capacity);
        }

        if  (!problem->get_cost_matrix().empty()) {
            if (!problem->get_cost_matrix().has_id(vehicle.start_node_id)) {
                throw std::make_pair(std::string("Unable to find node on matrix"), vehicle.start_node_id);
            }
            if (!problem->get_cost_matrix().has_id(vehicle.end_node_id)) {
                throw std::make_pair(std::string("Unable to find node on matrix"), vehicle.end_node_id);
            }
        }

        if (!(vehicle.start_open_t  <= vehicle.start_close_t
                    && vehicle.end_open_t <= vehicle.end_close_t
                    && vehicle.start_open_t <= vehicle.end_close_t)) {
            msg().error << "Illegal values found on vehicle";
            msg().log << "On vehicle " << vehicle.id
                << " a condition is not met, verify that:"
                << "\nvehicle.start_open_t  <= vehicle.start_close_t\t"
                << vehicle.start_open_t << " <= " << vehicle.start_close_t
                << "\nvehicle.end_open_t <= vehicle.end_close_t\t"
                << vehicle.end_open_t << " <= " << vehicle.end_close_t
                << "\nvehicle.start_open_t <= vehicle.end_close_t\t"
                << vehicle.start_open_t << " <= " << vehicle.end_close_t;

            throw std::make_pair(msg().get_error(), msg().get_log());
        }

        /*
         * Matrix version
         */
        auto starting_site = Vehicle_node({problem->get_nodes().size(), vehicle, Tw_node::NodeType::kStart});
        problem->add_node(starting_site);
        auto ending_site = Vehicle_node({problem->get_nodes().size(), vehicle, Tw_node::NodeType::kEnd});
        problem->add_node(ending_site);

        pgassert(starting_site.opens() <= starting_site.closes());
        pgassert(ending_site.opens() <= ending_site.closes());
        pgassert(starting_site.is_start() && ending_site.is_end());

        add_vehicle(vehicle, factor, starting_site, ending_site);
    }
    Identifiers<size_t> unused(m_trucks.size());
    m_un_used = unused;
    return true;
}


bool
Fleet::is_fleet_ok() const {
    ENTERING(msg());
    if (!msg().get_error().empty()) return false;
    for (auto truck : m_trucks) {
        if (!truck.is_ok()) {
            msg().error << "Illegal values found on vehicle";
            msg().log << "On vehicle " << truck.id()
                << " a condition is not met, verify that:\n"
                << "-  start_open <= start_close\n"
                << "-  end_open <= end_close\n"
                << "-  capacity > 0\n";
            return false;
        }

        if (!(truck.start_site().is_start()
                    && truck.end_site().is_end())) {
            pgassertwm(false, "should never pass through here");
            msg().error << "Illegal values found on vehicle";
            return false;
        }
        if (!truck.is_feasable()) {
            msg().error << "Truck is not feasible";
            return false;
        }
    }
    EXITING(msg());
    return true;
}

/**
 * Given an order,
 * Cycle trhugh all the trucks to verify if the order can be served by
 * at least one truck
 */
bool
Fleet::is_order_ok(const Order &order) const {
    for (const auto &truck : m_trucks) {
        if (!order.is_valid(truck.speed())) continue;
        if (truck.is_order_feasable(order)) {
            return true;
        }
    }
    return false;
}

Fleet&
Fleet::operator=(const Fleet &fleet) {
    m_trucks = fleet.m_trucks;
    m_used = fleet.m_used;
    m_un_used = fleet.m_un_used;
    return *this;
}

Vehicle_pickDeliver&
Fleet::operator[](size_t i) {
    pgassert(i < m_trucks.size());
    return m_trucks[i];
}

void
Fleet::set_compatibles(const PD_Orders &orders) {
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
    for (const auto &v : f.m_trucks) {
        log << v;
    }
    log << "end fleet\n";

    return log;
}


}  //  namespace vrp
}  //  namespace pgrouting
