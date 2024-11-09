/*PGR-GNU*****************************************************************
File: initial_solution.cpp

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


#include "vrp/initial_solution.hpp"
#include <deque>
#include <algorithm>
#include <set>
#include "cpp_common/assert.hpp"
#include "vrp/solution.hpp"
#include "vrp/pickDeliver.hpp"

namespace pgrouting {
namespace vrp {

void
Initial_solution::invariant() const {
    /* this checks there is no order duplicated */
    pgassert(m_all_orders == (m_assigned + m_unassigned));
    pgassert((m_assigned * m_unassigned).empty());
}


Initial_solution::Initial_solution(
        Initials_code kind,
        size_t number_of_orders) :
    Solution(),
    m_all_orders(number_of_orders),
    m_unassigned(number_of_orders),
    m_assigned() {
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

    Identifiers<size_t> notused;

    while (!m_unassigned.empty()) {
        auto current = m_unassigned.size();
        auto truck = trucks.get_truck(m_unassigned.front());
        /*
         * kind 1 to 7 work with the same code structure
         */
        truck.do_while_feasable((Initials_code)kind, m_unassigned, m_assigned);
        pgassertwm(current > m_unassigned.size(), msg().get_log().c_str());

        fleet.push_back(truck);
        invariant();
    }

    pgassertwm(true, msg().get_log().c_str());
    pgassert(is_feasable());
    invariant();
}




void
Initial_solution::one_truck_all_orders() {
    invariant();
    msg().log << "\nInitial_solution::one_truck_all_orders\n";
    auto truck = trucks.get_truck();
    while (!m_unassigned.empty()) {
        auto order(truck.orders()[*m_unassigned.begin()]);

        truck.insert(order);

        m_assigned += m_unassigned.front();
        m_unassigned.pop_front();

        invariant();
    }
    fleet.push_back(truck);
    invariant();
}




}  //  namespace vrp
}  //  namespace pgrouting
