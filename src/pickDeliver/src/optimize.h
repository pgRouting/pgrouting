/*PGR-GNU*****************************************************************

FILE: optimize.cpp

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

#include "./optimize.h"

namespace pgRouting {
namespace vrp {

class Solution;
class Pgr_pickDeliver;


class Optimize : public Solution {
 public:
     Optimize(int kind,
             const Solution &solution);

     /* @brief decrease_truck
      *
      * Optimization by decreasing trucks
      */
     void decrease_truck();
     bool move_reduce_cost();
     void move_wait_time_based();
     void inter_swap();
     void sort();
 private:
     void decrease_truck(size_t, bool&);
     bool move_reduce_cost(size_t, size_t);
     void sort_for_move();
     void sort_for_inter_swap();

     bool swap_worse(size_t from_pos, size_t to_pos);
     bool inter_swap(bool reversed);

     void move_order(
             Order order,
             Vehicle_pickDeliver &from_truck,
             Vehicle_pickDeliver &to_truck); 
};

}  //  namespace vrp
}  //  namespace pgRouting
