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

#include "vrp/pd_orders.h"

#include <vector>
#include <memory>
#include <utility>
#include <string>

#include "vrp/pgr_pickDeliver.h"
#include "vrp/dnode.h"

namespace pgrouting {
namespace vrp {

Pgr_pickDeliver* PD_Orders::problem;

PD_Orders::PD_Orders(
        const std::vector<PickDeliveryOrders_t> &pd_orders
        ) {
    build_orders(pd_orders);
}


void
PD_Orders:: add_order(
        const PickDeliveryOrders_t &order,
        const Vehicle_node &pick,
        const Vehicle_node &drop) {
    /*
     * add into an order
     */
    m_orders.push_back(
            Order(m_orders.size(), order.id,
                pick,
                drop));
}


void
PD_Orders::build_orders(
        const std::vector<PickDeliveryOrders_t> &pd_orders
        ) {
#if 0
    ENTERING();
#endif
    for (const auto &order : pd_orders) {
        /*
         * SAMPLE CORRECT INFORMATION
         *
         * id | demand | pick_x | pick_y | pick_open_t | pick_close_t | pick_service_t | deliver_x | deliver_y | deliver_open_t | deliver_open_t | deliver_close_t | deliver_service_t
         * 1  | 10     |   35   |   69   |   448       |   505        |    90          |    45     |   68      |    912         |   967          |    90           |    35
         */

        /*
         * matrix version
         */

        if (!problem->get_cost_matrix().has_id(order.pick_node_id)) {
            throw std::make_pair(std::string("Unable to find node on matrix"), order.pick_node_id);
        }
        if (!problem->get_cost_matrix().has_id(order.deliver_node_id)) {
            throw std::make_pair(std::string("Unable to find node on matrix"), order.deliver_node_id);
        }

        Vehicle_node pickup({problem->get_nodes().size(), order, Tw_node::NodeType::kPickup});
        problem->add_node(pickup);
        Vehicle_node delivery({problem->get_nodes().size(), order, Tw_node::NodeType::kDelivery});
        problem->add_node(delivery);

        add_order(order, pickup, delivery);
    }  //  for (creating orders)

#if 0
    EXITING();
#endif
}


Order&
PD_Orders::operator[](size_t i) {
    pgassert(i < m_orders.size());
    return m_orders[i];
}

const Order&
PD_Orders::operator[](size_t i) const {
    pgassert(i < m_orders.size());
    return m_orders[i];
}

void
PD_Orders::set_compatibles(double speed) {
    for (auto &I : m_orders) {
        for (const auto &J : m_orders) {
            I.set_compatibles(J, speed);
        }
    }
}

size_t
PD_Orders::find_best_J(
        Identifiers<size_t> &within_this_set) const {
    pgassert(!within_this_set.empty());
    auto best_order = within_this_set.front();
    size_t max_size = 0;


    for (auto o : within_this_set) {
        auto size_J =  m_orders[o].subsetJ(within_this_set).size();
        if (max_size < size_J) {
            max_size = size_J;
            best_order = o;
        }
    }
    return best_order;
}


size_t
PD_Orders::find_best_I(
        Identifiers<size_t> &within_this_set) const {
    pgassert(!within_this_set.empty());
    auto best_order = within_this_set.front();
    size_t max_size = 0;


    for (auto o : within_this_set) {
        auto size_I =  m_orders[o].subsetI(within_this_set).size();
        if (max_size < size_I) {
            max_size = size_I;
            best_order = o;
        }
    }
    return best_order;
}


}  //  namespace vrp
}  //  namespace pgrouting
