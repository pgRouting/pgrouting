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

/*! @file */

#ifndef INCLUDE_VRP_OPTIMIZE_H_
#define INCLUDE_VRP_OPTIMIZE_H_
#pragma once

#include "vrp/optimize.h"
#include "vrp/book_keeping.h"

namespace pgrouting {
namespace vrp {

class Solution;
class Pgr_pickDeliver;


class Optimize : public Solution {
 public:
     explicit Optimize(const Solution &solution);
     Optimize(const Solution &solution, size_t times);

     /* @brief decrease_truck
      *
      * Optimization by decreasing trucks
      */
     void decrease_truck();
     void move_wait_time_based();
     void move_duration_based();
     void inter_swap(size_t times);
     Solution best_solution;

 private:
     bool decrease_truck(size_t);
     void sort_for_move();
     void sort_by_duration();
     void sort_by_size();
     void sort_by_id();
     void delete_empty_truck();

     bool swap_worse(Vehicle_pickDeliver &from, Vehicle_pickDeliver &to);
     bool move_reduce_cost(Vehicle_pickDeliver &from, Vehicle_pickDeliver &to);
     bool inter_swap();

     void move_order(
             Order order,
             Vehicle_pickDeliver &from_truck,
             Vehicle_pickDeliver &to_truck);
     bool swap_order();
     bool swap_order(
             Order from_order,
             Vehicle_pickDeliver &from_truck,
             Order to_order,
             Vehicle_pickDeliver &to_truck);
     void save_if_best();

 private:
     Swap_bk p_swaps;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_OPTIMIZE_H_
