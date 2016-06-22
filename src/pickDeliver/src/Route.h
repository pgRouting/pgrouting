/*PGR-GNU*****************************************************************

FILE: Route.h

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
#include "pdp.hpp"

#include <vector>
#include <algorithm>

class Route  {
 public:
     int twv;
     int cv;
     double dis;
     std::vector < int64_t > path;
     double capacity;
     Depot depot;

     Route(double _capacity, const Depot &_depot) :
         twv(0),
         cv(0),
         dis(0),
         capacity(_capacity),
         depot(_depot) {
             path.clear();
         }
     void update(const Customers &c);
     double cost() const;
     bool insertOrder(const Customers &c, const Pickup &order);
     void append(const Customers &c, const Pickup &order);
     void remove(const State &S);
     int RemoveOrder(
             const Customers &customers,
             const Pickup &p);
     bool has_violation(const Customers &customers);
     bool has_twv(const Customers &customers);
};

