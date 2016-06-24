/*PGR-GNU*****************************************************************

FILE: vehicle_pickDeliver.h

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

#include <set>
#include "./order.h"
#include "./tw_node.h"
#include "./vehicle.h"

namespace pgRouting {
namespace vrp {


class Initial_solution;
class Optimize;

class Vehicle_pickDeliver : public Vehicle {
 protected:
     double cost;
     std::set<ID> orders_in_vehicle;  // /< orders inserted in this vehicle
     const Pgr_pickDeliver *problem;  // /< The vehicle belongs to this problem


 public:
     friend Initial_solution;
     friend Optimize;

     Vehicle_pickDeliver(
             ID id,
             const Vehicle_node &starting_site,
             const Vehicle_node &ending_site,
             double max_capacity,
             const Pgr_pickDeliver *p_problem);

     Vehicle_pickDeliver(const Vehicle_pickDeliver &v) = default;

     size_t orders_size() const {return orders_in_vehicle.size();}

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
      * Before: S <nodes> E
      *   After: S <nodes> P D E
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
      * Before: S <nodes> E
      *   After: S P D <nodes> E
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
      * Before: S <nodes> E
      *   After: S ....P .... D .... E
      *
      * push_back is performed when
      *   - pickup
      *
      * Can generate time window violation
      * No capacity violation
      */
     void insert(const Order &order);

     /* @brief erases the order from the vehicle
      *
      * Precondition:
      * has_order(order)
      *
      * Precondition:
      * !has_order(order)
      */
     void erase(const Order &order);

     /* @brief erases the order from the vehicle
      *
      * Precondition:
      * 0 < orders_size() 
      *
      * Postcondition:
      * has_order(returned order)
      *
      */
     Order get_first_order() const;


     /*!
      * The order that is picked last is removed
      *
      * \returns id of the removed order
      */

     ID pop_back();
     ID pop_front();
};

}  //  namespace vrp
}  //  namespace pgRouting

