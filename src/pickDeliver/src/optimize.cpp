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


#include <set>
#include "./solution.h"
#include "./../../common/src/pgr_assert.h"
#include "./optimize.h"
#include "./pgr_pickDeliver.h"

namespace pgRouting {
namespace vrp {


Optimize::Optimize(
        int kind,
        const Solution &old_solution) :
    Solution(old_solution) {
        switch (kind) {
            case 0:
                sort();
                break;
            case 1:
                decrease_truck();
                break;
        }
    }

void
Optimize::sort() {
    /* Step 1: Sort the fleet, less orders with less time at back */
    // problem->log << "Before sort (number of orders, duration)" << tau();

    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return rhs.duration() > lhs.duration();
            });
    // problem->log << "After sort by duration" << tau();

    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return rhs.orders_size() > lhs.orders_size();
            });

    // problem->log << "After sort by number of orders" << tau();
}


/*
 * Optimize decreasing truck
 *
 * - Objective: try to remove shortest truck
 *
 * Step 1: Sort the fleet, less orders with less time at back
 * Step 2: grab an order from the back of the the fleet
 * Step 3: cycle the fleet & insert in best truck possible
 */
void
Optimize::decrease_truck() {
    sort();
    size_t cycle(fleet.size());
    decrease_truck(cycle);
}

void
Optimize::decrease_truck(size_t cycle) {
    /* end recursion */
    if (cycle == 0) return;
    problem->log << " --- Cycle " << cycle << "\n";

    /* the front truck move to back */ 
    std::rotate(fleet.begin(), fleet.begin() + 1, fleet.end());
    // problem->log << " after rotate" << tau();

    auto orders(fleet.back().orders_in_vehicle);
    while (!orders.empty()) {
        /* Step 2: grab an order */
        auto order(problem->orders()[*orders.begin()]);
        orders.erase(orders.begin());
        // problem->log << "Order" << order << "\n";

        /* Step 3: delete the order from the back of the fleet */
        pgassertwm(fleet.back().has_order(order), problem->log.str());
        fleet.back().erase(order);
        // problem->log << " after erase" << tau() << "\n";
        pgassertwm(!fleet.back().has_order(order), problem->log.str());

        /* Step 3: cycle the fleet & insert in first truck possible */
        /* current truck is tried last */
        bool inserted(false);
        // problem->log << " trying ";
        for (auto &truck : fleet) {
            truck.insert(order);
            if (!truck.is_feasable()) {
                // problem->log << truck.tau();
                truck.erase(order);
                continue;
            }
            inserted = true;
            problem->log << "\n ******* success " << truck.tau() << "\n";
            break;
        }
        pgassertwm(inserted, problem->log.str());
    }
    if (fleet.back().empty()) {
        fleet.pop_back();
    }
    decrease_truck(--cycle);
}

}  //  namespace vrp
}  //  namespace pgRouting
