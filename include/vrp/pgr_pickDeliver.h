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

#include "c_types/pickDeliver/vehicle_t.h"
#include "c_types/pickDeliver/orders_t.h"
#include "vrp/pd_problem.h"
#include "cpp_common/Dmatrix.h"
#include "vrp/fleet.h"
#include "vrp/pd_orders.h"
#include "vrp/solution.h"

using Schedule_rt = struct Schedule_rt;

namespace pgrouting {
namespace vrp {

class Order;
class Vehicle_node;

class Dnode;
class Solution;
class Initial_solution;

class Pgr_pickDeliver : public PD_problem {
    friend Fleet;
    friend Dnode;
    friend PD_Orders;
    friend Solution;
    friend Initial_solution;

 public:
    Pgr_pickDeliver(
            const std::vector<Orders_t> &pd_orders,
            const std::vector<Vehicle_t> &vehicles,
            const pgrouting::tsp::Dmatrix &cost_matrix,
            double factor,
            size_t max_cycles,
            int initial);

    void solve();

    std::vector<Schedule_rt>
        get_postgres_result() const;


    size_t max_cycles() const {return m_max_cycles;}

    void add_node(const Vehicle_node &node);

    Initials_code get_kind() const {
        return (Initials_code) m_initial_id;
    }

    std::vector<Vehicle_node> get_nodes() const {
        return m_nodes;
    }

    pgrouting::tsp::Dmatrix get_cost_matrix() const {
        return m_cost_matrix;
    }

    Fleet trucks() const {return m_trucks;}

    /** message controller for all classes */
    Pgr_messages msg;


 private:
    //! used define the initial solution algorithm to be used
    int m_initial_id;

    //! maximum cycles in the optimization
    size_t m_max_cycles;

    std::vector<Vehicle_node> m_nodes;
    pgrouting::tsp::Dmatrix m_cost_matrix;

    PD_Orders m_orders;
    Fleet m_trucks;
    std::vector<Solution> solutions;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_PGR_PICKDELIVER_H_
