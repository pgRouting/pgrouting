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


#include <string>
#include <vector>

#include "./fleet.h"
#include "./tw_node.h"
#include "./vehicle_pickDeliver.h"
#include "./pgr_pickDeliver.h"
#include "./../../common/src/identifiers.hpp"


namespace pgrouting {
namespace vrp {


Vehicle_pickDeliver
Fleet::get_truck() {
    auto id = un_used.front();
    log << "id" << id
        << "size" << m_trucks.size();
    pgassertwm(id < m_trucks.size(),log.str());
    used += id;
    if (un_used.size() > 1) un_used -= id;
    return m_trucks[id];
}


bool
Fleet::build_fleet(
        const std::vector<Vehicle_t> &vehicles,
        size_t &node_id
        ) {
    for (auto vehicle : vehicles) {

        if (vehicle.cant_v < 0) {
            error << "Illegal number of vehicles found vehicle";
            log << vehicle.cant_v << "< 0 on vehicle " << vehicle.id;
            continue;
        }

        auto starting_site = Vehicle_node(
                {node_id++, vehicle, Tw_node::NodeType::kStart});
        auto ending_site = Vehicle_node(
                {node_id++, vehicle, Tw_node::NodeType::kEnd});

        if (!(starting_site.is_start()
                    && ending_site.is_end())) {
            error << "Illegal values found on vehcile";
            return false;
        }

        problem->add_node(starting_site);
        problem->add_node(ending_site);

        for (int i = 0; i < vehicle.cant_v; ++i) {
            m_trucks.push_back(Vehicle_pickDeliver(
                        vehicle.id,
                        starting_site,
                        ending_site,
                        vehicle.capacity,
                        vehicle.speed,
                        problem));
        }
    }
    Identifiers<size_t> unused(m_trucks.size());
    un_used = unused;
    return true;
}

bool
Fleet::is_fleet_ok() const {
    for (auto truck : m_trucks) {
        if (!(truck.start_site().is_start()
                    && truck.end_site().is_end())) {
            error << "Illegal values found on vehcile";
            return false;
        }
        if (!truck.is_feasable()) {
            error << "Truck is not feasable";
            return false;
        }
    }
    return true;
}

bool
Fleet::is_order_ok(const Order &order) const {
    for (const auto truck : m_trucks) {
        if (!order.is_valid(truck.speed())) continue; 
        auto test_truck = truck;
        test_truck.push_back(order);

        if (test_truck.is_feasable()) {
            return true;
        }
    }
    return false;
}

Vehicle_pickDeliver&
Fleet::operator[](size_t i) {
    pgassert(i < m_trucks.size());
    return m_trucks[i];
}

}  //  namespace vrp
}  //  namespace pgrouting
