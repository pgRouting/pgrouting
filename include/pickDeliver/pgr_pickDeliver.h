/*PGR-GNU*****************************************************************

FILE: pgr_pickDeliver.h

Copyright (c) 2017 pgRouting developers
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

/*! @file pgr_pickDeliver.h */

#ifndef INCLUDE_PICKDELIVER_PGR_PICKDELIVER_H_
#define INCLUDE_PICKDELIVER_PGR_PICKDELIVER_H_
#pragma once



#include <string>
#include <vector>
#include <sstream>

#include "c_types/pickDeliver/general_vehicle_orders_t.h"
#include "c_types/pickDeliver/vehicle_t.h"
#include "c_types/pickDeliver/pickDeliveryOrders_t.h"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/Dmatrix.h"

#include "pickDeliver/vehicle_node.h"
#include "pickDeliver/fleet.h"
#include "pickDeliver/solution.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {

class Order;
class PD_Orders;

class Pgr_pickDeliver : public PD_problem {
    friend class Initial_solution;
    friend class Optimize;
    friend class Dnode;

    typedef size_t ID;

 public:
    Pgr_pickDeliver(
            const std::vector<PickDeliveryOrders_t> &pd_orders,
            const std::vector<Vehicle_t> &vehicles,
            size_t max_cycles,
            int initial);

    Pgr_pickDeliver(
            const std::vector<PickDeliveryOrders_t> &pd_orders,
            const std::vector<Vehicle_t> &vehicles,
            const pgrouting::tsp::Dmatrix &cost_matrix,
            size_t max_cycles,
            int initial);

    void solve();

    std::vector<General_vehicle_orders_t>
        get_postgres_result() const;

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
    inline Identifiers<size_t> compatibleJ(size_t I) const {
        return m_orders[I].subsetJ(Identifiers<size_t>());
    }

    inline Order orders(size_t o) const {return m_orders[o];}

    inline size_t& node_id() {return m_node_id;}

    void add_node(const Vehicle_node &node) {
        m_nodes.push_back(node);
    }
    Fleet trucks() const {return m_trucks;}

    friend std::ostream& operator<<(
            std::ostream &log,
            const Pgr_pickDeliver &pd_prob);

    /// @{
    //! The Distance/time matrix
    pgrouting::tsp::Dmatrix m_cost_matrix;

 private:
    //! used define the initial solution algorithm to be used
    int m_initial_id;

    //! maximum cycles in the optimization
    size_t m_max_cycles;

    //! used to keep track of the next id the node gets in the eucledian version
    size_t m_node_id;

    std::vector<Vehicle_node> m_nodes;

    Fleet m_trucks;
    PD_Orders m_orders;
    std::vector<Solution> solutions;
};

}  // namespace pickdeliver
}  // namespace vrp
}  // namespace pgrouting

#endif  // INCLUDE_PICKDELIVER_PGR_PICKDELIVER_H_
