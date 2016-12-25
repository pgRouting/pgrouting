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


#include "./initial_solution.h"
#include <deque>
#include <algorithm>
#include <set>
#include "./../../common/src/pgr_assert.h"
#include "./solution.h"
#include "./pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {

void
Initial_solution::invariant() const {

    /* this checks there is no order duplicated */
    pgassert(all_orders == (assigned + unassigned));
    pgassert((assigned * unassigned).empty());
}


Initial_solution::Initial_solution(
        int kind) :
    Solution(),
    all_orders(problem->orders().size()),
    unassigned(problem->orders().size()),
    assigned()
{
    switch (kind) {
        case 0:
            one_truck_all_orders();
            break;
        case 1:
            one_truck_per_order();
            break;
        case 2:
            push_back_while_feasable();
            break;
        case 3:
            push_front_while_feasable();
            break;
        case 4:
            insert_while_feasable();
            break;
        case 5:
            insert_while_compatibleJ();
            break;
        case 6:
            insert_while_compatibleI();
            break;
    }
}



void
Initial_solution::fill_truck_while_compatibleJ(
        Vehicle_pickDeliver &truck,
        Identifiers<size_t> &possible_orders) {
    invariant();
    /*
     * Precondition:
     * all orders in the truck are in the assigned set
     */
    pgassert(truck.orders_in_vehicle == (truck.orders_in_vehicle * assigned));

    /*
     * Precondition:
     *  all possible orders are not in the assigned set
     */
    pgassert((possible_orders * assigned).empty());

    /*
     * termination of recursion
     */
    if (possible_orders.empty())
        return;

    /*
     * CODE
     */
    auto best_order = possible_orders.front();
    size_t max_size(0);

    /*
     * In the possible orders set look for the order that
     * has more compatible orders with the current possible orders
     */
    for (const auto &o : possible_orders) {
        auto intersect_orders = problem->compatibleJ(o) * possible_orders;
        if (max_size < intersect_orders.size()) {
            max_size = intersect_orders.size();
            best_order = o;
        }
    }
    auto intersect_orders = problem->compatibleJ(best_order) * possible_orders;

    truck.insert(problem->orders(best_order));
    if (!truck.is_feasable()) {
        truck.erase(problem->orders(best_order));
    } else {
        assigned += best_order;
        unassigned -= best_order;
    }

    possible_orders -= best_order;
    fill_truck_while_compatibleJ(truck, possible_orders);
    invariant();
}



std::deque<size_t>
Initial_solution::first_ordersIJ() const {
    /*
     * Sorted as:
     * (| {I}|, | {J}|)
     * orders: keep sorted based on the number of orders it is compatible with
     */
    std::deque<size_t> orders(unassigned.begin(), unassigned.end());
    const Pgr_pickDeliver *prob = problem;
    std::sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleJ.size()
            < prob->orders()[rhs].m_compatibleJ.size();
            });
    std::stable_sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleI.size()
            < prob->orders()[rhs].m_compatibleI.size();
            });
    return orders;
}



void
Initial_solution::insert_while_compatibleJ() {
    log << "\nInitial_solution::insert_while_compatible\n";
    invariant();


    // size_t v_id(0);
    auto truck = trucks.get_truck();
#if 0
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem->m_speed,
            problem);
#endif

    while (!unassigned.empty()) {
        std::deque<size_t> orders(first_ordersIJ());

        if (truck.empty()) {
            auto order(problem->orders()[orders.front()]);
            truck.insert(order);
            assigned += order.id();
            unassigned -= order.id();
            orders.pop_front();
            invariant();

            auto compatible_orders(
                    problem->orders()[order.id()].m_compatibleJ);
            auto possible_orders = compatible_orders * unassigned;

            fill_truck_while_compatibleJ(truck, possible_orders);
            fleet.push_back(truck);

            if (unassigned.empty())
                break;

            truck = trucks.get_truck();
#if 0
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem->m_speed,
                    problem);
            truck = newtruck;
#endif
        }
        invariant();
    }
}



void
Initial_solution::fill_truck_while_compatibleI(
        Vehicle_pickDeliver &truck,
        Identifiers<size_t> &possible_orders) {
    invariant();
    /*
     * Precondition:
     * all orders in the truck are in the assigned set
     */
    pgassert(truck.orders_in_vehicle == (truck.orders_in_vehicle * assigned));

    /*
     * Precondition:
     *  all possible orders are not in the assigned set
     */
    pgassert((possible_orders * assigned).empty());

    /*
     * termination of recursion
     */
    if (possible_orders.empty())
        return;

    /*
     * CODE
     */
    auto best_order = *possible_orders.begin();
    size_t max_size(0);

    /*
     * In the possible orders set look for the order that
     * has more compatible orders with the current possible orders
     */
    for (auto &o : possible_orders) {
        auto other_orders = problem->orders()[o].m_compatibleI;
        auto intersect_orders = problem->orders()[o].subsetI(possible_orders);
        if (max_size < intersect_orders.size()) {
            max_size = intersect_orders.size();
            best_order = o;
        }
    }
    auto intersect_orders =
        problem->orders()[best_order].subsetI(possible_orders);

    truck.insert(problem->orders()[best_order]);
    if (!truck.is_feasable()) {
        truck.erase(problem->orders()[best_order]);
    } else {
        assigned += best_order;
        unassigned -= best_order;
    }

    possible_orders -= best_order;
    fill_truck_while_compatibleI(truck, possible_orders);
    invariant();
}




std::deque<size_t>
Initial_solution::first_ordersJI() const {
    /*
     * Sorted as:
     * (| {J}|, | {I}|)
     * orders: keep sorted based on the number of orders it is compatible with
     */
    std::deque<size_t> orders(unassigned.begin(), unassigned.end());
    const Pgr_pickDeliver *prob = problem;
    std::sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleI.size()
            < prob->orders()[rhs].m_compatibleI.size();
            });
    std::stable_sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleJ.size()
            < prob->orders()[rhs].m_compatibleJ.size();
            });
    return orders;
}



void
Initial_solution::insert_while_compatibleI() {
    log << "\nInitial_solution::insert_while_compatible\n";
    invariant();


    auto truck = trucks.get_truck();

    while (!unassigned.empty()) {
        std::deque<size_t> orders(first_ordersJI());

        if (truck.empty()) {
            auto order(problem->orders()[orders.front()]);
            truck.insert(order);
            assigned += order.id();
            orders.pop_front();
            unassigned -= order.id();
            invariant();

            auto compatible_orders(
                    problem->orders()[order.id()].m_compatibleI);
            auto possible_orders = compatible_orders * unassigned;

            fill_truck_while_compatibleI(truck, possible_orders);
            fleet.push_back(truck);

            if (unassigned.empty())
                break;

            truck = trucks.get_truck();
        }
        invariant();
    }
}





void
Initial_solution::insert_while_feasable() {
    invariant();

    auto truck = trucks.get_truck();
    log << "\nInitial_solution::insert_while_feasable\n";
    while (!unassigned.empty()) {
        auto order(problem->orders()[unassigned.front()]);

        truck.insert(order);

        if (!truck.is_feasable()) {
            truck.erase(order);
            fleet.push_back(truck);
            truck = trucks.get_truck();
        } else {
            assigned += unassigned.front();
            unassigned.pop_front();
        }

        invariant();
    }

    if (truck.orders_size() !=0 ) {
        fleet.push_back(truck);
    }
}

void
Initial_solution::push_front_while_feasable() {
    auto truck = trucks.get_truck();
#if 0
    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem->m_speed,
            problem);
#endif
    while (!unassigned.empty()) {
        auto order(problem->orders()[unassigned.front()]);

        truck.push_front(order);
        if (!truck.is_feasable()) {
            truck.pop_front();
            fleet.push_back(truck);
            truck = trucks.get_truck();
#if 0
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem->m_speed,
                    problem);
            truck = newtruck;
#endif
        } else {
            assigned += unassigned.front();
            unassigned.pop_front();
        }

        invariant();
    }

    if (truck.orders_size() !=0 ) {
        fleet.push_back(truck);
    }
}

void
Initial_solution::push_back_while_feasable() {
    log << "\nInitial_solution::push_back_while_feasable\n";
    auto truck = trucks.get_truck();

    while (!unassigned.empty()) {
        log << "\nassigned" << assigned;
        log << "\ntau" << tau();
        auto order(problem->orders()[*unassigned.begin()]);

        truck.push_back(order);
        if (!truck.is_feasable()) {
            truck.pop_back();
            fleet.push_back(truck);
            truck = trucks.get_truck();
        } else {
            assigned += order.id();
            unassigned -= order.id();
        }

        invariant();
    }
    if (truck.orders_size() !=0 ) {
        fleet.push_back(truck);
    }
}



void
Initial_solution::one_truck_per_order() {
    // size_t v_id(0);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        auto truck = trucks.get_truck();
#if 0
        Vehicle_pickDeliver truck(
                v_id++,
                problem->m_starting_site,
                problem->m_ending_site,
                problem->max_capacity,
                problem->m_speed,
                problem);
#endif
        truck.push_back(order);
        fleet.push_back(truck);

        assigned += unassigned.front();
        unassigned.pop_front();

        invariant();
    }
}




void
Initial_solution::one_truck_all_orders() {
    // size_t v_id(0);
    auto truck = trucks.get_truck();
#if 0
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem->m_speed,
            problem);
#endif
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.insert(order);

        assigned += unassigned.front();
        unassigned.pop_front();

        invariant();
    }
    fleet.push_back(truck);
}




}  //  namespace vrp
}  //  namespace pgrouting
