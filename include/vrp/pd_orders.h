/*PGR-GNU*****************************************************************

FILE: pd_orders.h

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

#ifndef INCLUDE_VRP_PD_ORDERS_H_
#define INCLUDE_VRP_PD_ORDERS_H_
#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "c_types/pickDeliver/pickDeliveryOrders_t.h"
#include "cpp_common/identifiers.hpp"

#include "vrp/base_node.h"
#include "vrp/pd_problem.h"


namespace pgrouting {
namespace vrp {

class Order;
class Vehicle_node;

class PD_Orders : public PD_problem {
     typedef std::vector<Order> Orders;

 public:
     typedef Orders::iterator o_iterator;
     typedef Orders::const_iterator o_const_iterator;

 public:
     /*! @name constructors
      * @{
      */
     PD_Orders() = default;
     PD_Orders(const PD_Orders&) = default;
     explicit PD_Orders(const std::vector<PickDeliveryOrders_t>&);
     /*!@}*/

     void set_compatibles(double speed);
     size_t find_best_J(Identifiers<size_t> &within_this_set) const;
     size_t find_best_I(Identifiers<size_t> &within_this_set) const;


     bool is_valid(double speed) const;

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
             const std::vector<PickDeliveryOrders_t> &pd_orders);

     void add_order(
             const PickDeliveryOrders_t &,
             std::unique_ptr<Base_node>,
             const Vehicle_node&,
             std::unique_ptr<Base_node>,
             const Vehicle_node&);

     template <typename T> std::unique_ptr<Base_node> create_b_pick(
             const PickDeliveryOrders_t &order,
             size_t node_id) {
         std::unique_ptr<Base_node> b_pick(new T(
                     node_id,
                     order.pick_node_id,
                     order.pick_x,
                     order.pick_y));
         return std::move(b_pick);
     }

     template <typename T> std::unique_ptr<Base_node> create_b_deliver(
             const PickDeliveryOrders_t &order,
             size_t node_id) {
         std::unique_ptr<Base_node> b_drop(new T(
                     node_id,
                     order.deliver_node_id,
                     order.deliver_x,
                     order.deliver_y));
         return std::move(b_drop);
     }

 private:
     Orders m_orders;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_PD_ORDERS_H_
