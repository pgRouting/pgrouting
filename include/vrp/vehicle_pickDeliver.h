/*PGR-GNU*****************************************************************

FILE: vehicle_pickDeliver.h

Copyright (c) 2016 pgRouting developers
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

#ifndef INCLUDE_VRP_VEHICLE_PICKDELIVER_H_
#define INCLUDE_VRP_VEHICLE_PICKDELIVER_H_
#pragma once

#include <set>
#include "vrp/order.h"
#include "vrp/pd_orders.h"
#include "vrp/tw_node.h"
#include "vrp/vehicle.h"
#include "cpp_common/identifiers.hpp"

namespace pgrouting {
namespace vrp {


class Initial_solution;
class Optimize;

class Vehicle_pickDeliver : public Vehicle {
 protected:
     double cost;
     //! orders inserted in this vehicle
     Identifiers<size_t> m_orders_in_vehicle;
     PD_Orders m_orders;
     //! orders that fit in the truck
     Identifiers<size_t> m_feasable_orders;


 public:
     friend class Initial_solution;
     friend class Optimize;

     Vehicle_pickDeliver(
             size_t id,
             size_t kind,
             const Vehicle_node &starting_site,
             const Vehicle_node &ending_site,
             double p_capacity,
             double p_speed,
             double factor);

     Vehicle_pickDeliver(const Vehicle_pickDeliver &) = default;


     void set_compatibles(const PD_Orders &orders);
     bool is_order_feasable(const Order &order) const;
     Identifiers<size_t> feasable_orders() const {return m_feasable_orders;}

     const PD_Orders& orders() const {return m_orders;}
     size_t orders_size() const {return m_orders_in_vehicle.size();}
     Identifiers<size_t> orders_in_vehicle() const {return m_orders_in_vehicle;}

     bool has_order(const Order &order) const;

     /*! @brief puts an order at the end of the truck
      *
      * Precondition:
      * !has_order(order)
      *
      * Postcondition:
      * has_order(order)
      * !has_cv();
      *
      * ~~~~{.c}
      * Before: S <nodes> E
      *   After: S <nodes> P D E
      * ~~~~
      *
      * Can generate time window violation
      * No capacity violation
      */
     void push_back(const Order &order);


     /*! @brief Puts an order at the end front of the truck
      *
      * Precondition:
      * !has_order(order)
      *
      * Postcondition:
      * has_order(order)
      * !has_cv();
      *
      * ~~~~{.c}
      * Before: S <nodes> E
      *   After: S P D <nodes> E
      * ~~~~
      *
      * Can generate time window violation
      * No capacity violation
      */
     void push_front(const Order &order);


     /*! @brief Inserts an order
      *
      * Precondition:
      * !has_order(order)
      *
      * Postcondition:
      * has_order(order)
      * !has_cv();
      *
      * ~~~~{.c}
      * Before: S <nodes> E
      *   After: S ....P .... D .... E
      * ~~~~
      *
      * push_back is performed when
      *   - pickup
      *
      * Can generate time window violation
      * No capacity violation
      */
     void insert(const Order &order);

#if 0
     void insert_while_compatibleJ(
             Identifiers<PD_Orders::OID> &unassigned,
             Identifiers<PD_Orders::OID> &assigned);
#endif
     /* @brief erases the order from the vehicle
      *
      * Precondition:
      * has_order(order)
      *
      * Precondition:
      * !has_order(order)
      */
     void erase(const Order &order);

     /* @brief 
      */
     Order get_first_order() const;
     Order get_worse_order(Identifiers<size_t> of_this_subset) const;

     void do_while_feasable(
             int kind,
             Identifiers<size_t> &unassigned,
             Identifiers<size_t> &assigned);


     /*!
      * The order that is picked last is removed
      *
      * \returns id of the removed order
      */

     size_t pop_back();
     size_t pop_front();
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_VEHICLE_PICKDELIVER_H_
