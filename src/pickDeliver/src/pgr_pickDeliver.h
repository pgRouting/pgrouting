/*PGR-GNU*****************************************************************

FILE: pgr_pickDeliver.h

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

#ifndef SRC_PICKDELIVER_SRC_PGR_PICKDELIVER_H_
#define SRC_PICKDELIVER_SRC_PGR_PICKDELIVER_H_
#pragma once


#include "./../../common/src/pgr_types.h"
#include "./../../common/src/identifiers.hpp"

#include <string>
#include <vector>
#include <sstream>

#include "./pgr_messages.h"
#include "./vehicle_node.h"
#include "./fleet.h"
#include "./orders.h"
#include "./solution.h"

namespace pgrouting {
namespace vrp {

class Order;

class Pgr_pickDeliver : public Pgr_messages{
    friend class Initial_solution;
    friend class Optimize;

    typedef size_t ID;

 public:
    Pgr_pickDeliver(
            const std::vector<PickDeliveryOrders_t> &pd_orders,
            const std::vector<Vehicle_t> &vehicles,
            size_t max_cycles,
            std::string &error);

    void solve();

    std::vector<General_vehicle_orders_t>
        get_postgres_result() const;

    /*****************/

    const Order order_of(const Vehicle_node &node) const;
    const Vehicle_node& node(ID id) const;
#if 0
    const PD_Orders& orders() const {return m_orders;}
#endif

    Solution optimize(const Solution init_solution);
    size_t max_cycles() const {return m_max_cycles;}

    //! name orders handling (TODO? in a class?
    /// @{

    /*! \brief I -> {J}
     *
     * gets the orders {J} that can be visited after visiting order I
     */
    inline Identifiers<size_t> compatibleJ(size_t I) const{
        return m_orders[I].m_compatibleJ;
    }

    inline Order orders(size_t o) const {return m_orders[o];}

    void add_node(const Vehicle_node &node) {
        m_nodes.push_back(node);
    }
    Fleet trucks() const {return m_trucks;}

    /// @{
 private:
#if 1
    double m_speed;
#endif
    size_t m_max_cycles;
    std::vector<Vehicle_node> m_nodes;
    Fleet m_trucks;
#if 1
    PD_Orders m_orders;
#endif
    std::vector<Solution> solutions;

};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // SRC_PICKDELIVER_SRC_PGR_PICKDELIVER_H_
