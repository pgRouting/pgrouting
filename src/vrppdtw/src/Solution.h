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
             const Depot &d,
             const std::vector<Pickup> &p) const;
     void UpdateSol();
};

class Neighborhoods {
 public:
     Neighborhoods() {}
     Solution BestSPI(const Solution S,
             const std::vector<Customer> &customers,
             const Depot &depot,
             const std::vector<Pickup> &pickups) const;
};

void
Solution::UpdateSol() {
    cost_total = 0, twv_total = 0, cv_total = 0, dis_total = 0;
    routes.erase(std::remove_if(routes.begin(), routes.end(),
                [] (const Route &r)
                {return r.path.empty();}),
            routes.end());
    for  (const auto &r : routes) {
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
        const Depot &depot,
        const std::vector<Pickup> &pickups) const {
    Neighborhoods N;
    Solution S1;
    S1 = N.BestSPI(S, customers, depot, pickups);
    return S1;
}


Solution
Neighborhoods::BestSPI(const Solution S,
        const std::vector<Customer> &customers,
        const Depot &depot,
        const std::vector<Pickup> &pickups) const {
    Solution CurrSol, BestSol, TempSol;
    CurrSol = BestSol = S;
    std::vector<Pickup> OrderRequests = pickups;
    int Ro_flag, Hc_flag;
    State TempState;

    // Main SPI
    for (size_t order = 1; order <= pickups.size(); order++) {
        // Order Find and Remove it!
        for (unsigned int route_remove = 0;
                route_remove < CurrSol.routes.size();
                route_remove++) {
            Ro_flag = CurrSol.routes[route_remove].RemoveOrder(OrderRequests[order]);
            TempSol = CurrSol;
            if  (Ro_flag != 1) continue;

            // Insert, Total Cost,
            //   (if it is more copy back the solution) ,
            //   (else store best = temp, checked = 2, break)

            for (auto &route : CurrSol.routes) {
                TempState = route.append(OrderRequests[order], TempState);
                Hc_flag = route.insertOrder(customers, depot);
                // Hc flag tells us about insertion
                if  (Hc_flag == 0) {
                    if  (route.cost() <= BestSol.routes[&route-&CurrSol.routes[0]].cost()) {
                        CurrSol.UpdateSol();
                        BestSol = CurrSol;
                    }
                }
                TempSol.UpdateSol();
                CurrSol = TempSol;
            }
            BestSol.UpdateSol();
            CurrSol = BestSol;
            break;
        }
    }
    BestSol.UpdateSol();
    return BestSol;
}



