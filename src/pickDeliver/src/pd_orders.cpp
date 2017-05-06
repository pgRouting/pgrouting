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

#include "pickDeliver/pd_orders.h"

#include <vector>

#include "pickDeliver/order.h"
#include "pickDeliver/tw_node.h"
#include "pickDeliver/vehicle_node.h"
#include "pickDeliver/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {

PD_Orders::PD_Orders(
        const std::vector<PickDeliveryOrders_t> &pd_orders) {
    build_orders(pd_orders);
}

void
PD_Orders::build_orders(
        const std::vector<PickDeliveryOrders_t> &pd_orders
        ) {
    OID order_id(0);
    for (const auto order : pd_orders) {
        /*
         * SAMPLE CORRECT INFORMATION
         *
         *  id | demand | p_node_id | p_x | p_y | p_open | p_close | p_service | d_node_id | d_x | d_y | d_open | d_close | d_service 
         * ----+--------+-----------+-----+-----+--------+---------+-----------+-----------+-----+-----+--------+---------+-----------
         *  11 |     10 |        99 |  35 |  69 |    448 |     505 |        90 |        97 |  45 |  68 |    912 |     967 |        90
         *   
         */


        /*
         * Creating the pickup & delivery nodes
         */
        Vehicle_node pickup(
                {problem->node_id()++, order, Tw_node::NodeType::kPickup});

        pgassert(pickup.demand() == order.demand);
        pgassert(pickup.opens() == order.pick_open_t);
        pgassert(pickup.closes() == order.pick_close_t);
        pgassert(pickup.original_id() == order.pick_node_id);

        Vehicle_node delivery(
                {problem->node_id()++, order, Tw_node::NodeType::kDelivery});

        pgassert(delivery.demand() == -order.demand);
        pgassert(delivery.opens() == order.deliver_open_t);
        pgassert(delivery.closes() == order.deliver_close_t);
        pgassert(delivery.original_id() == order.deliver_node_id);

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
                    delivery));

#if 1
        auto o = m_orders.back();
        pgassert(o.delivery().demand() == -order.demand);
#if 0
        msg.log << "ORDER\n"
            << "original id of the order:" << o.original_id()
            << "internal idx of the order:" << o.id()
            << "pickup.demand:" << o.pickup().demand()
            << "internal idx of the order:" << o.id()
            << "\n";
#endif
#endif
    }  //  for (creating orders)

#if 0
    for (auto &o : m_orders) {
        o.setCompatibles();
    }
#endif
}

bool
PD_Orders::is_valid(double speed) const {
    for (const auto &o : m_orders) {
        if (!o.is_valid(speed)) {
            return false;
        }
        pgassert(o.pickup().is_pickup());
        pgassert(o.delivery().is_delivery());
        /* P -> D */
        pgassert(o.delivery().is_compatible_IJ(o.pickup(), speed));
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

void
PD_Orders::set_compatibles(double speed) {
    for (auto &I : m_orders) {
        for (const auto J : m_orders) {
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

std::ostream& operator << (std::ostream &log, const PD_Orders &orders) {
    log << "*******   ORDERS   *********\n";
    for (const auto o : orders) {
        log << o << "\n";
    }
    log << "*******\n";
    return log;
}

}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting
