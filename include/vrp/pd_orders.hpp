/*PGR-GNU*****************************************************************
File: pd_orders.hpp

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

/*! @file */

#ifndef INCLUDE_VRP_PD_ORDERS_HPP_
#define INCLUDE_VRP_PD_ORDERS_HPP_
#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "cpp_common/orders_t.hpp"
#include "cpp_common/identifiers.hpp"

#include "vrp/pd_problem.hpp"
#include "vrp/order.hpp"
#include "vrp/vehicle_node.hpp"


namespace pgrouting {
namespace vrp {

class PD_Orders {
    /** PD_rpblem needs access to set up the problem pointer */
     friend class PD_problem;

     typedef std::vector<Order> Orders;

 public:
     typedef Orders::iterator o_iterator;
     typedef Orders::const_iterator o_const_iterator;

 public:
     /*! @name constructors
      * @{
      */
     PD_Orders() = default;
     explicit PD_Orders(const std::vector<Orders_t>&);
     /*!@}*/

     void set_compatibles(double speed);
     size_t find_best_J(Identifiers<size_t> &within_this_set) const;
     size_t find_best_I(Identifiers<size_t> &within_this_set) const;



     /*! @name std container functions
      * functions with same "meaning" as an std container
      * @{
      */
     Order& operator[](size_t o);
     const Order& operator[](size_t o) const;
     size_t size() const {return m_orders.size();}
     o_iterator begin() {return m_orders.begin();}
     o_iterator end() {return m_orders.end();}
     o_const_iterator begin() const {return m_orders.begin();}
     o_const_iterator end() const {return m_orders.end();}
     /*!@}*/

 private:
     void build_orders(
             const std::vector<Orders_t> &pd_orders);

     void add_order(
             const Orders_t &,
             const Vehicle_node&,
             const Vehicle_node&);


 private:
     Orders m_orders;

     /** @brief Access to the problem's message */
     Pgr_messages& msg() const;

     /** The problem */
     static Pgr_pickDeliver* problem;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_PD_ORDERS_HPP_
