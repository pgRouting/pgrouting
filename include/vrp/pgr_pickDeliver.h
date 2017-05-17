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

/*! @file */

#ifndef INCLUDE_VRP_PGR_PICKDELIVER_H_
#define INCLUDE_VRP_PGR_PICKDELIVER_H_
#pragma once



#include <vector>
#include <memory>
#include <utility>

#include "c_types/pickDeliver/general_vehicle_orders_t.h"
#include "c_types/pickDeliver/vehicle_t.h"
#include "c_types/pickDeliver/pickDeliveryOrders_t.h"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/Dmatrix.h"
#include "vrp/fleet.h"
#include "vrp/pd_orders.h"
#include "vrp/solution.h"

namespace pgrouting {
namespace vrp {

class Order;
class Vehicle_node;
class Base_node;

class Pgr_pickDeliver : public PD_problem {
 public:
    Pgr_pickDeliver(
            const std::vector<PickDeliveryOrders_t> &pd_orders,
            const std::vector<Vehicle_t> &vehicles,
            double factor,
            size_t max_cycles,
            int initial);

    Pgr_pickDeliver(
            const std::vector<PickDeliveryOrders_t> &pd_orders,
            const std::vector<Vehicle_t> &vehicles,
            const pgrouting::tsp::Dmatrix &cost_matrix,
            double factor,
            size_t max_cycles,
            int initial);

    void solve();

    std::vector<General_vehicle_orders_t>
        get_postgres_result() const;


    Solution optimize(const Solution init_solution);
    size_t max_cycles() const {return m_max_cycles;}

    inline size_t& node_id() {return m_node_id;}

    void add_node(const Vehicle_node &node) {
        m_nodes.push_back(node);
    }

    void add_base_node(std::unique_ptr<Base_node> node_ptr) {
        m_base_nodes.push_back(std::move(node_ptr));
    }

#if 1
    // TODO(vicky) delete this function
    bool nodesOK() const;
#endif
    Fleet trucks() const {return m_trucks;}

 private:
    //! used define the initial solution algorithm to be used
    int m_initial_id;

    //! maximum cycles in the optimization
    size_t m_max_cycles;

    //! used to keep track of the next id the node gets in the eucledian version
    size_t m_node_id;

    std::vector<Vehicle_node> m_nodes;

 public:
    // TODO(vicky) make this private
    std::vector<std::unique_ptr<Base_node>> m_base_nodes;
    pgrouting::tsp::Dmatrix m_cost_matrix;

 private:
    PD_Orders m_orders;
    Fleet m_trucks;
    std::vector<Solution> solutions;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_PGR_PICKDELIVER_H_
