/*PGR-GNU*****************************************************************

FILE: initial_solution.h

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
#include <deque>
#include "./solution.h"

namespace pgRouting {
namespace vrp {


class Pgr_pickDeliver;


class Initial_solution : public Solution {
 public:
     Initial_solution(
             int kind,
             const Pgr_pickDeliver *problem);

     void invariant() const;

 private:
     /*
      * one truck per order
      */
     void one_truck_per_order();
     void one_truck_all_orders();
     void push_back_while_feasable();
     void push_front_while_feasable();
     void insert_while_feasable();

     void insert_while_compatibleJ();
     void fill_truck_while_compatibleJ(
             Vehicle_pickDeliver &truck,
             std::set<size_t> &possible_orders);
     std::deque<size_t> first_ordersIJ() const;

     void insert_while_compatibleI();
     void fill_truck_while_compatibleI(
             Vehicle_pickDeliver &truck,
             std::set<size_t> &possible_orders);
     std::deque<size_t> first_ordersJI() const;


 private:
     std::set<size_t> all_orders;
     std::set<size_t> unassigned;
     std::set<size_t> assigned;
};

}  //  namespace vrp
}  //  namespace pgRouting
