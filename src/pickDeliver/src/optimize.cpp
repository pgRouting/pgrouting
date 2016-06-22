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


#include "solution.h"
#include "optimize.h"
#include "pgr_pickDeliver.h"

namespace pgRouting {
namespace vrp {


Optimize::Optimize(
        int kind,
        const Solution &old_solution) :
    Solution(old_solution) {

        switch (kind) {
            case 1:
                decrease_truck();
                break;
        }
    }

/*
 * Optimize decreasing truck
 *
 * - Objective: remove shortest truck
 *
 */
void
Optimize::decrease_truck() {
    /* sort fleet by truck.size smallest is last*/

    problem->log << "Before sort" << tau();


    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs) -> bool {
            return rhs.orders_size() < lhs.orders_size();
            }
            );

    problem->log << "After first sort" << tau();

    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs) -> bool {
            return rhs.duration() < lhs.duration();
            }
            );
    problem->log << "After sort" << tau();


}


}  //  namespace pgRouting
}  //  namespace vrp

