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
#pragma once

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include "./../../common/src/pgr_types.h"

#include <string>
#include <vector>
#include <sstream>

#include "./vehicle_node.h"
#include "./order.h"
#include "./solution.h"

namespace pgRouting {
namespace vrp {


class Optimize;
class Initial_solution;

class Pgr_pickDeliver {
    friend class Vehicle_pickDeliver;
    friend class Optimize;
    friend class Initial_solution;
    friend class Solution;
    typedef size_t ID;

 public:
    Pgr_pickDeliver(
            const Customer_t *c1, size_t total_customers,
            int VehicleLength,
            double capacity,
            size_t max_cycles,
            std::string &error);

    void solve();

    void get_postgres_result(
            std::vector< General_vehicle_orders_t > &result) const;
    /*****************/

    const Order order_of(const Vehicle_node &node) const;
    const Vehicle_node& node(ID id) const;
    const std::vector<Order>& orders() const {return m_orders;}

    /*! \brief get_log
     *
     * \returns the current contents of the log and clears the log
     *
     */
    std::ostream& get_log(std::ostream &p_log) const {
        p_log << log.str() << "\n";
        log.str("");
        log.clear();
        return p_log;
    }

    Solution solve(const Solution init_solution);
    size_t max_cycles() const {return m_max_cycles;};

 private:
    double max_capacity;
    size_t m_max_cycles;
    int max_vehicles;
    Vehicle_node m_starting_site, m_ending_site;
    std::vector<Customer_t> m_original_data;
    std::vector<Vehicle_node> m_nodes;
    std::vector<Order> m_orders;
    std::vector<Solution> solutions;
    mutable std::ostringstream log;
#ifndef NDEBUG
    mutable std::ostringstream dbg_log;
#endif
};

}  //  namespace vrp
}  //  namespace pgRouting
