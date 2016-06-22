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


#include "../../common/src/pgr_assert.h"
#include "solution.h"
#include "initial_solution.h"
#include "pgr_pickDeliver.h"

namespace pgRouting {
namespace vrp {

void
Initial_solution::invariant() const {
    std::set<size_t> orders(assigned);

    orders.insert(unassigned.begin(), unassigned.end());

    /* check the local book keeping is ok */
    pgassert(all_orders == orders);

    /* this checks there is no order duplicated */
    pgassert(all_orders.size() == orders.size());
}


Initial_solution::Initial_solution(
        int kind,
        const Pgr_pickDeliver *p_problem) :
    Solution(p_problem) {

        for (const auto &order : problem->orders()) {
            unassigned.insert(order.id());
        }
        all_orders = unassigned;
        assigned.clear();

        switch  (kind) {
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
        std::set<size_t> &possible_orders) {
    invariant();
    /*
     * Precondition:
     *  truck.orders_in_vehicle intersection assigned == truck.orders_in_vehicle
     *  (all orders in the truck are in the assigned set)
     */
    std::set<size_t> invariant_set;
    std::set_intersection(truck.orders_in_vehicle.begin(), truck.orders_in_vehicle.end(),
            assigned.begin(), assigned.end(),
            std::inserter(invariant_set, invariant_set.begin()));
    pgassert(invariant_set == truck.orders_in_vehicle);

    invariant_set.clear();
    /*
     * Precondition:
     *  possible_orders intersection unassigned == possible_orders
     *  (all possible orders are not in the assigned set)
     */
    std::set_intersection(possible_orders.begin(), possible_orders.end(),
            assigned.begin(), assigned.end(),
            std::inserter(invariant_set, invariant_set.begin()));
    pgassert(invariant_set.empty());

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
        auto  other_orders = problem->orders()[o].m_compatibleJ;
#if 0
        problem->log << "\n [" << o << "]";
        for (auto &oo : other_orders) {
            problem->log <<  oo <<",  ";
        }
#endif
        auto  intersect_orders = problem->orders()[o].subsetJ(possible_orders);
        if (max_size < intersect_orders.size()) {
#if 0
            problem->log << "\n [" << o << "]";
            for (auto &oo : intersect_orders) {
                problem->log <<  oo <<",  ";
            }
#endif
            max_size = intersect_orders.size();
            best_order = o;
        }
    }
    auto  intersect_orders = problem->orders()[best_order].subsetJ(possible_orders);
#if 0
    problem->log << "\n [" << best_order << "]";
    for (auto &oo : intersect_orders) {
        problem->log <<  oo <<",  ";
    }
#endif

    truck.insert(problem->orders()[best_order]);
#if 0
    problem->log << truck;
#endif
    if (!truck.is_feasable()) {
        truck.erase(problem->orders()[best_order]);
#if 0
        problem->log << truck;
#endif
    } else {
        assigned.insert(best_order);
        unassigned.erase(unassigned.find(best_order));
    }

    possible_orders.erase(possible_orders.find(best_order));
    fill_truck_while_compatibleJ(truck, possible_orders);
    invariant();
}














std::deque<size_t>
Initial_solution::first_ordersIJ() const {
    /*
     * Sorted as:
     * (|{I}|, |{J}|)
     * orders: keep sorted based on the number of orders it is compatible with
     */
    std::deque<size_t> orders(unassigned.begin(), unassigned.end());
    const Pgr_pickDeliver *prob = problem;
    std::sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleJ.size()
            < prob->orders()[rhs].m_compatibleJ.size();
            } ); 
    std::stable_sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleI.size()
            < prob->orders()[rhs].m_compatibleI.size();
            } ); 

#if 0
    problem->log << "\n Sorted orders by compatibleI.size\n";
    for (const auto &o: orders) {
        problem->log << "\n|"<< o <<"| = " << problem->orders()[o].m_compatibleI.size();
        problem->log << "\t|"<< o <<"| = " << problem->orders()[o].m_compatibleJ.size();
    };
#endif
    return orders;
}





void
Initial_solution::insert_while_compatibleJ() {
    problem->log << "\nInitial_solution::insert_while_compatible\n";
    invariant();


    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);

    while (!unassigned.empty()) {
        std::deque<size_t> orders(first_ordersIJ());

        if (truck.empty()) {
            auto order(problem->orders()[orders.front()]);
            truck.insert(order);
            assigned.insert(order.id());
            orders.pop_front();
            unassigned.erase(unassigned.find(order.id()));
            invariant();

            std::set<size_t> compatible_orders(problem->orders()[order.id()].m_compatibleJ);
            std::set<size_t> possible_orders;
            std::set_intersection(
                    compatible_orders.begin(), compatible_orders.end(),
                    unassigned.begin(), unassigned.end(),
                    std::inserter(possible_orders, possible_orders.begin()));


            fill_truck_while_compatibleJ(truck, possible_orders);
            problem->log << truck;

            fleet.push_back(truck);

            if (unassigned.empty())
                break;

            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        }
        invariant();
    };
}



void
Initial_solution::fill_truck_while_compatibleI(
        Vehicle_pickDeliver &truck,
        std::set<size_t> &possible_orders) {
    invariant();
    /*
     * Precondition:
     *  truck.orders_in_vehicle intersection assigned == truck.orders_in_vehicle
     *  (all orders in the truck are in the assigned set)
     */
    std::set<size_t> invariant_set;
    std::set_intersection(truck.orders_in_vehicle.begin(), truck.orders_in_vehicle.end(),
            assigned.begin(), assigned.end(),
            std::inserter(invariant_set, invariant_set.begin()));
    pgassert(invariant_set == truck.orders_in_vehicle);

    invariant_set.clear();
    /*
     * Precondition:
     *  possible_orders intersection unassigned == possible_orders
     *  (all possible orders are not in the assigned set)
     */
    std::set_intersection(possible_orders.begin(), possible_orders.end(),
            assigned.begin(), assigned.end(),
            std::inserter(invariant_set, invariant_set.begin()));
    pgassert(invariant_set.empty());

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
        auto  other_orders = problem->orders()[o].m_compatibleI;
#if 0
        problem->log << "\n [" << o << "]";
        for (auto &oo : other_orders) {
            problem->log <<  oo <<",  ";
        }
#endif
        auto  intersect_orders = problem->orders()[o].subsetI(possible_orders);
        if (max_size < intersect_orders.size()) {
#if 0
            problem->log << "\n [" << o << "]";
            for (auto &oo : intersect_orders) {
                problem->log <<  oo <<",  ";
            }
#endif
            max_size = intersect_orders.size();
            best_order = o;
        }
    }
    auto  intersect_orders = problem->orders()[best_order].subsetI(possible_orders);
#if 0
    problem->log << "\n [" << best_order << "]";
    for (auto &oo : intersect_orders) {
        problem->log <<  oo <<",  ";
    }
#endif

    truck.insert(problem->orders()[best_order]);
#if 0
    problem->log << truck;
#endif
    if (!truck.is_feasable()) {
        truck.erase(problem->orders()[best_order]);
#if 0
        problem->log << truck;
#endif
    } else {
        assigned.insert(best_order);
        unassigned.erase(unassigned.find(best_order));
    }

    possible_orders.erase(possible_orders.find(best_order));
    fill_truck_while_compatibleI(truck, possible_orders);
    invariant();
}











std::deque<size_t>
Initial_solution::first_ordersJI() const {
    /*
     * Sorted as:
     * (|{J}|, |{I}|)
     * orders: keep sorted based on the number of orders it is compatible with
     */
    std::deque<size_t> orders(unassigned.begin(), unassigned.end());
    const Pgr_pickDeliver *prob = problem;
    std::sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleI.size()
            < prob->orders()[rhs].m_compatibleI.size();
            } ); 
    std::stable_sort(orders.begin(), orders.end(), [&prob]
            (const size_t &lhs, const size_t &rhs) -> bool
            {return prob->orders()[lhs].m_compatibleJ.size()
            < prob->orders()[rhs].m_compatibleJ.size();
            } ); 

#if 0
    problem->log << "\n Sorted orders by compatibleI.size\n";
    for (const auto &o: orders) {
        problem->log << "\n|"<< o <<"| = " << problem->orders()[o].m_compatibleI.size();
        problem->log << "\t|"<< o <<"| = " << problem->orders()[o].m_compatibleJ.size();
    };
#endif
    return orders;
}



void
Initial_solution::insert_while_compatibleI() {
    problem->log << "\nInitial_solution::insert_while_compatible\n";
    invariant();


    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);

    while (!unassigned.empty()) {
        std::deque<size_t> orders(first_ordersJI());

        if (truck.empty()) {
            auto order(problem->orders()[orders.front()]);
            truck.insert(order);
            assigned.insert(order.id());
            orders.pop_front();
            unassigned.erase(unassigned.find(order.id()));
            invariant();

            std::set<size_t> compatible_orders(problem->orders()[order.id()].m_compatibleI);
            std::set<size_t> possible_orders;
            std::set_intersection(
                    compatible_orders.begin(), compatible_orders.end(),
                    unassigned.begin(), unassigned.end(),
                    std::inserter(possible_orders, possible_orders.begin()));


            fill_truck_while_compatibleI(truck, possible_orders);
            problem->log << truck;
            fleet.push_back(truck);

            if (unassigned.empty())
                break;

            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        }
        invariant();
    };
}





void
Initial_solution::insert_while_feasable() {
    invariant();

    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    problem->log << "\nInitial_solution::insert_while_feasable\n";
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.insert(order);

        if (!truck.is_feasable()) {
            truck.erase(order); 
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
}

void
Initial_solution::push_front_while_feasable() {
    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.push_front(order);
        problem->log << truck;
        if (!truck.is_feasable()) {
            truck.pop_front();
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
}

void
Initial_solution::push_back_while_feasable() {
    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.push_back(order);
        if (!truck.is_feasable()) {
            truck.pop_back();
            fleet.push_back(truck);
            Vehicle_pickDeliver newtruck(
                    v_id++,
                    problem->m_starting_site,
                    problem->m_ending_site,
                    problem->max_capacity,
                    problem);
            truck = newtruck;
        } else {
            assigned.insert(*unassigned.begin());
            unassigned.erase(unassigned.begin());
        }

        invariant();
    };
}



void
Initial_solution::one_truck_per_order() {
    size_t v_id(0);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        Vehicle_pickDeliver truck(
                v_id++,
                problem->m_starting_site,
                problem->m_ending_site,
                problem->max_capacity,
                problem);
        truck.push_back(order);
        fleet.push_back(truck);

        assigned.insert(*unassigned.begin());
        unassigned.erase(unassigned.begin());

        invariant();
    };
}




void
Initial_solution::one_truck_all_orders() {
    size_t v_id(0);
    Vehicle_pickDeliver truck(
            v_id++,
            problem->m_starting_site,
            problem->m_ending_site,
            problem->max_capacity,
            problem);
    while (!unassigned.empty()) {
        auto order(problem->orders()[*unassigned.begin()]);

        truck.insert(order);

        assigned.insert(*unassigned.begin());
        unassigned.erase(unassigned.begin());

        invariant();
    };
    fleet.push_back(truck);
}




}  // namespace pgRouting
}  // namespace vrp
