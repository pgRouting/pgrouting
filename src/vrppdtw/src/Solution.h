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
     int dis_total;
     double cost_total;
     std::vector<Route> routes;
     // Methods
     void dump() const;
     double getCost() const;
     Solution getBestofNeighborhood(const Solution S,
             const std::vector<Customer> &c,
             const std::vector<Pickup> &p) const;
     void UpdateSol(const Customers &customers);
};

class Neighborhoods {
 public:
     Neighborhoods() {}
     Solution BestSPI(const Solution S,
             const std::vector<Customer> &customers,
             const std::vector<Pickup> &pickups) const;
};

void
Solution::UpdateSol(const Customers &customers) {
    cost_total = 0, twv_total = 0, cv_total = 0, dis_total = 0;
    routes.erase(std::remove_if(routes.begin(), routes.end(),
                [] (const Route &r)
                {return r.path.empty();}),
            routes.end());
    for (auto &r : routes) {
        r.update(customers);
        twv_total += r.twv;
        dis_total += r.dis;
        cv_total  += r.cv;
        cost_total += r.cost();
    }
}


// Methods in Solution


double
Solution::getCost() const {
    double cost_total = 0;
    for  (const auto &r : routes) {
        cost_total += r.cost();
    }
    return cost_total;
}

Solution
Solution::getBestofNeighborhood(const Solution S,
        const std::vector<Customer> &customers,
        const std::vector<Pickup> &pickups) const {
    Neighborhoods N;
    Solution S1;
    S1 = N.BestSPI(S, customers, pickups);
    return S1;
}


Solution
Neighborhoods::BestSPI(const Solution S,
        const Customers &customers,
        const Pickups &pickups) const {
    Solution CurrSol, BestSol, TempSol;
    CurrSol = BestSol = S;
    std::vector<Pickup> OrderRequests = pickups;
    State TempState;

    BestSol.UpdateSol(customers);
    // Main SPI
    for (const auto &order : OrderRequests) {
        // Order Find and Remove it!
        CurrSol = BestSol;
        for (unsigned int route_remove = 0;
                route_remove < CurrSol.routes.size();
                route_remove++) {

            int OK = CurrSol.routes[route_remove].RemoveOrder(customers,order);

            if  (OK != 1) continue;
            TempSol = CurrSol;
            TempSol.UpdateSol(customers);

            for (auto &route : TempSol.routes) {
                int OK = route.insertOrder(customers, order);
                if (!OK) continue;
                TempSol.UpdateSol(customers);
                if (TempSol.getCost() < BestSol.getCost()) {
                    BestSol = TempSol;
                    CurrSol = TempSol;
                    break;
                }
                /*
                 * the order was inserted but the distance was not best
                 */
                route.RemoveOrder(customers, order);
            }
        }
    }

    return BestSol;
}
