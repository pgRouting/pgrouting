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

void
Fleet::build_fleet(
        const std::vector<Vehicle_t> &vehicles,
        size_t &node_id
        ) {
    for (auto vehicle : vehicles) {

        if (vehicle.cant_v < 0) {
            problem->error << "Illegal number of vehicles found vehicle";
            problem->log << vehicle.cant_v << "< 0 on vehicle " << vehicle.id;
            continue;
        }

        auto starting_site = Vehicle_node(
                {node_id++, vehicle, Tw_node::NodeType::kStart, problem});
        auto ending_site = Vehicle_node(
                {node_id++, vehicle, Tw_node::NodeType::kEnd, problem});

        (problem->m_nodes).push_back(starting_site);
        (problem->m_nodes).push_back(ending_site);

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
}

bool
Fleet::is_fleet_ok() const {
    for (auto truck : m_trucks) {
        if (!(truck.start_site().is_start()
                    && truck.end_site().is_end())) {
            problem->error << "Illegal values found on vehcile";
            return false;
        }
        if (!truck.is_feasable()) {
            problem->error << "Truck is not feasable";
            return false;
        }
    }
    return true;
}

bool
Fleet::is_order_ok(const Order &order) const {
    for (const auto truck : m_trucks) {
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
