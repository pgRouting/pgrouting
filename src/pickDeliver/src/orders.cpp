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

#include "./orders.h"
#include "./order.h"

#include "./tw_node.h"
#include "./vehicle_node.h"
#include "./pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {

void
PD_Orders::build_orders(
        const std::vector<PickDeliveryOrders_t> &pd_orders,
        size_t &node_id
        ) {
    OID order_id(0);
    for (const auto order : pd_orders) {
        /*
         * SAMPLE CORRECT INFORMATION
         *
         * id | demand | pick_x | pick_y | pick_open_t | pick_close_t | pick_service_t | deliver_x | deliver_y | deliver_open_t | deliver_open_t | deliver_close_t | deliver_service_t
         * 1  | 10     |   35   |   69   |   448       |   505        |    90          |    45     |   68      |    912         |   967          |    90           |    35
         */

        /*
         * Creating the pickup & delivery nodes
         */
        Vehicle_node pickup(
                {node_id++, order, Tw_node::NodeType::kPickup, problem});
        Vehicle_node delivery(
                {node_id++, order, Tw_node::NodeType::kDelivery, problem});

        pickup.set_Did(delivery.id());
        delivery.set_Pid(pickup.id());

        problem->add_node(pickup);
        problem->add_node(delivery);

        /*
         * add into an order
         */
        m_orders.push_back(
                Order(order_id++,
                    pickup,
                    delivery,
                    problem));
    }  //  for (creating orders)

    for (auto &o : m_orders) {
        o.setCompatibles();
    }
}

bool
PD_Orders::is_valid() const {
    for (const auto &o : m_orders) {
        if (!o.is_valid()) {
            error << "The order " << o.pickup().original_id() << " is not feasible";
            log << "The order " << o.pickup().original_id() << " is not feasible";
            return false;
        }
        pgassert(o.pickup().is_pickup());
        pgassert(o.delivery().is_delivery());
            /* P -> D */
        pgassert(o.delivery().is_compatible_IJ(o.pickup()));
    }
    return true;
}

Order&
PD_Orders::operator[](OID i) {
    pgassert(i < m_orders.size());
    return m_orders[i];
}

const Order&
PD_Orders::operator[](OID i) const {
    pgassert(i < m_orders.size());
    return m_orders[i];
}

}  //  namespace vrp
}  //  namespace pgrouting
