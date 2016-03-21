/*PGR-GNU*****************************************************************

Copyright  (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

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

#include <vector>

#include "./Route.h"

class Solution {
 public:
     Solution() {
         cost_total = 0, twv_total = 0, cv_total = 0, dis_total = 0;
     }
     // Variables
     int twv_total;
     int cv_total;
     double dis_total;
     double cost_total;
     std::vector<Route> routes;
     // Methods
     void dump() const;
     double getCost() const;
     Solution getBestofNeighborhood(const Solution S,
             const std::vector<Customer_t> &c,
             const std::vector<Pickup> &p) const;
     void UpdateSol(const Customers &customers);
};

class Neighborhoods {
 public:
     Neighborhoods() {}
     Solution BestSPI(const Solution S,
             const std::vector<Customer_t> &customers,
             const std::vector<Pickup> &pickups) const;
};
