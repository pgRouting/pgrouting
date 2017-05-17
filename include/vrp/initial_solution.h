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

/*! @file */

#ifndef INCLUDE_VRP_INITIAL_SOLUTION_H_
#define INCLUDE_VRP_INITIAL_SOLUTION_H_
#pragma once

#include <set>
#include <deque>
#include "vrp/pd_orders.h"
#include "vrp/solution.h"
#include "cpp_common/identifiers.hpp"

namespace pgrouting {
namespace vrp {


class Pgr_pickDeliver;


class Initial_solution : public Solution {
 public:
     Initial_solution(
             int kind,
             size_t);

     void invariant() const;

 private:
     /*
      * one truck per order
      */
     void one_truck_all_orders();

     void do_while_foo(int kind);


 private:
     Identifiers<size_t> all_orders;
     Identifiers<size_t> unassigned;
     Identifiers<size_t> assigned;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_INITIAL_SOLUTION_H_
