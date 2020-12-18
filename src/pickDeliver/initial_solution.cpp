/*PGR-GNU*****************************************************************

FILE: initial_solution.cpp

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


#include "vrp/initial_solution.h"
#include <deque>
#include <algorithm>
#include <set>
#include "cpp_common/pgr_assert.h"
#include "vrp/solution.h"
#include "vrp/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {

void
Initial_solution::invariant() const {
    /* this checks there is no order duplicated */
    pgassert(all_orders == (assigned + unassigned));
    pgassert((assigned * unassigned).empty());
}


Initial_solution::Initial_solution(
        Initials_code kind,
        size_t number_of_orders) :
    Solution(),
    all_orders(number_of_orders),
    unassigned(number_of_orders),
    assigned() {
        invariant();
        pgassert(kind >= 0 && kind <= OneDepot);

        switch (kind) {
            case OneTruck:
                one_truck_all_orders();
                break;
            case OnePerTruck:
            case FrontTruck:
            case BackTruck:
            case BestInsert:
            case BestBack:
            case BestFront:
            case OneDepot:
                do_while_foo(kind);
                break;
            default: pgassert(false);
        }

    invariant();
}



void
Initial_solution::do_while_foo(int kind) {
    invariant();
    pgassert(kind > 0 && kind <= OneDepot);

#if 0
    msg.log << "\nInitial_solution::do_while_foo\n";
#endif
    Identifiers<size_t> notused;

    while (!unassigned.empty()) {
#if 0
        msg.log << unassigned.size() << " unassigned: " << unassigned << "\n";
        msg.log << assigned.size() << " assigned:" << assigned << "\n";
#endif
        auto current = unassigned.size();
        auto truck = trucks.get_truck(unassigned.front());
#if 0
        msg.log << "got truck:" << truck.tau() << "\n";
#endif
        /*
         * kind 1 to 7 work with the same code structure
         */
        truck.do_while_feasable((Initials_code)kind, unassigned, assigned);
#if 0
        msg.log << unassigned.size() << " unassigned: " << unassigned << "\n";
        msg.log << assigned.size() << " assigned:" << assigned << "\n";
        msg.log << "current" << current << " unassigned: " << unassigned.size();
#endif
        pgassertwm(current > unassigned.size(), msg.get_log().c_str());

        fleet.push_back(truck);
        invariant();
    }

    pgassertwm(true, msg.get_log().c_str());
    pgassert(is_feasable());
    invariant();
}




void
Initial_solution::one_truck_all_orders() {
    invariant();
    msg.log << "\nInitial_solution::one_truck_all_orders\n";
    auto truck = trucks.get_truck();
    while (!unassigned.empty()) {
        auto order(truck.orders()[*unassigned.begin()]);

        truck.insert(order);

        assigned += unassigned.front();
        unassigned.pop_front();

        invariant();
    }
    fleet.push_back(truck);
    invariant();
}




}  //  namespace vrp
}  //  namespace pgrouting
