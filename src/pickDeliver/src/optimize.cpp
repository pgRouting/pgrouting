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


#include <algorithm>
#include <limits>
#include <set>

#include "./../../common/src/pgr_assert.h"

#include "./solution.h"
#include "./optimize.h"
#include "./pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {


Optimize::Optimize(
        const Solution &old_solution) :
    Solution(old_solution),
    best_solution(old_solution)  {
    }


void
Optimize::inter_swap() {
#if 1
    auto local_limit(fleet.size());
    local_limit = 10;
    size_t i(0);
    i = 0;
    while (inter_swap(true) && (++i < local_limit)) {
        problem-> log << "\n***************************" << i;
    }
#else
#if 1
#endif
    inter_swap(true);
#endif
#if 0
    sort_by_duration();
    delete_empty_truck();
    this->fleet = best_solution.fleet;
#endif
}

/*
 * ordering the trucks by number of orders it has
 * - the truck with more orders wants to:
 *   - make less time
 *   - do more orders
 * The inter_swap objective is to make the trucks with more orders
 *  - less time consuming
 */ 
bool
Optimize::inter_swap(bool reversed) {
    if (reversed) {};
    problem->log << tau("before sort");
    sort_by_size();
    problem->log << tau("after sort");
    auto swapped_f = false;
    /*
     *   .. to ... from ....
     */
    for (auto &from : fleet) {
        for (auto &to : fleet) {
            if (&from == &to) break;

#if 0
            problem->log
                << "\nto " << to.id()
                << "from " << from.id();
#endif
            auto swapped = false;
            swapped = swap_worse(to, from) || swapped;
#if 0
            if (swapped) problem->log
                << "\n" <<tau("after  movements");
#endif
            swapped_f = swapped_f || swapped;
        }
    }

    if (swapped_f) problem->log
        << "\n" <<tau("after  swaps");
    move_reduce_cost();
    problem->log
        << "\n" <<tau("after  move");
    delete_empty_truck();

    return swapped_f;
}



/*
 *   .. to ... from ....
 */
bool
Optimize::swap_worse(Vehicle_pickDeliver &to, Vehicle_pickDeliver &from) {
#if 0
    pgassert(from.orders_in_vehicle().size() <= to.orders_in_vehicle().size());
#endif
    auto from_truck = from;
    auto to_truck = to;

    auto swapped = false;

    for (auto from_orders = from_truck.orders_in_vehicle();
            !from_orders.empty();
            from_orders.pop_front()) {
        auto from_order = from_truck.orders()[from_orders.front()];
#if 0
        pgassert(from_truck.has_order(from_order));
        problem->log << "\n" << from_orders;
        problem->log << "\n from " << from_order.id()
            << "," << from_order.pickup().original_id();
        pgassert(from_truck.has_order(from_order));
#endif
        auto curr_from_duration = from_truck.duration();
        pgassert(from_truck.has_order(from_order));

        for (auto to_orders = to_truck.orders_in_vehicle();
                !to_orders.empty();
                to_orders.pop_front()) {
            pgassert(from_truck.has_order(from_order));

            auto to_order = to.orders()[to_orders.front()];
#if 0
            problem->log << "\n" << to_orders;
            problem->log << "\n To " << to_order.id();
#endif
            auto curr_to_duration = to_truck.duration();

            /*
             * delete from_order, and to order from their trucks
             */
#if 0
            pgassert(from_truck.has_order(from_order));
            problem->log << "\n" << from_truck.tau();
            problem->log << "\n" << from_order.id();
            pgassert(from_truck.has_order(from_order));
#endif
            from_truck.erase(from_order);
            to_truck.erase(to_order);

            /*
             * insert them in the other truck
             */
            from_truck.insert(to_order);
            to_truck.insert(from_order);

            if (from_truck.is_feasable() && to_truck.is_feasable()) {
                /*
                 * Can swap but:
                 *   - only swap when the total duration is reduced
                 *   - or from_truck duration is reduced 
                 */
#if 0
                problem->log << "\n Can swap";
                problem->log << "\n Curr_from_duration " << curr_from_duration;
                problem->log << " Curr_to_duration " << curr_to_duration;
                problem->log << "\n  new_from_duration " << from_truck.duration();
                problem->log << "  new_to_duration " << to_truck.duration();
#endif
                auto estimated_duration = duration()
                    - (curr_from_duration + curr_to_duration)
                    + (to_truck.duration() + from_truck.duration());

                if (to_truck.duration() < curr_to_duration ||
                    estimated_duration < best_solution.duration()) {
                    problem->log
                        << "\n    Swap order " << from_order.pickup().original_id()
                        << " from truck " << from_truck.id()
                        << " with order " << to_order.pickup().original_id()
                        << " of truck " << to_truck.id();

#if 0
                    problem->log << "\nswappping before:";
                    problem->log << "\n" << to.tau();
                    problem->log << "\n" << from.tau();
#endif
                    swap_order(
                            from_order, from,
                            to_order, to);
                    swapped = true;
                    save_if_best();
#if 0
                    problem->log << "\nswappping after:";
                    problem->log << "\n" << to.tau();
                    problem->log << "\n" << from.tau();
                    return true;
#endif
                    break;
                }

            }
            to_truck = to;
            from_truck = from;
        }
        from_truck = from;
    }

    return swapped;
}

/*
 * from_truck: position of the truck where the order is
 * to truck: truck to put the order
 */
void
Optimize::swap_order(
        const Order from_order,
        Vehicle_pickDeliver &from_truck,
        const Order to_order,
        Vehicle_pickDeliver &to_truck) {
    pgassert(from_truck.has_order(from_order));
    pgassert(to_truck.has_order(to_order));

    from_truck.erase(from_order);
    to_truck.erase(to_order);

    from_truck.insert(to_order);
    to_truck.insert(from_order);


    pgassert(from_truck.has_order(to_order));
    pgassert(to_truck.has_order(from_order));
}

void
Optimize::sort_by_size() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.orders_in_vehicle().size() > rhs.orders_in_vehicle().size();
            });
}

void
Optimize::sort_by_duration() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.duration() > rhs.duration();
            });
}

void
Optimize::delete_empty_truck() {
    fleet.erase(std::remove_if(
                fleet.begin(), 
                fleet.end(),
                [](const Vehicle_pickDeliver &v){
                return v.orders_in_vehicle().empty();}),
            fleet.end());
    save_if_best();
}

void
Optimize::move_duration_based() {
    auto local_limit(fleet.size());
    size_t i(0);

    sort_by_duration();
    problem->log << tau("\nmove duration based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    delete_empty_truck();

    i = 0;
    sort_by_duration();
    std::reverse(fleet.begin(), fleet.end());
    problem->log << tau("\nmove duration based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    sort_by_duration();
    delete_empty_truck();
    this->fleet = best_solution.fleet;
}


void
Optimize::move_wait_time_based() {
    this->fleet = best_solution.fleet;

    auto local_limit(fleet.size());
    size_t i(0);

    sort_for_move();
    problem->log << tau("\nmove wait_time based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    delete_empty_truck();

    i = 0;
    sort_for_move();
    std::reverse(fleet.begin(), fleet.end());
    problem->log << tau("\nmove wait_time based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    sort_by_duration();
    delete_empty_truck();
    this->fleet = best_solution.fleet;
}

/*
 * On the current order of the fleet
 * T1 .......Tn-1  Tn Tn+1...... Tsize
 * Tn tries to move orders to trucks
 *      T1 .... Tn-1 
 * So that it gets space for the orders given by
 *      Tn+1 .... Tsize
 * On the first move possible it returns
 *
 * When a truck is emptied, then it removes the truck from the fleet
 *
 * Returns true: when a move was possible
 * Returns false: when a move was not possible
 */ 


bool
Optimize::move_reduce_cost() {
    if (fleet.size() < 2) return false;
    bool moved = false;

    size_t from_pos(fleet.size() - 1);
    while (from_pos > 1) {
        for (size_t to_pos = 0; to_pos < from_pos; ++to_pos) {
            moved = move_reduce_cost(from_pos, to_pos) || moved;
        }
        --from_pos;
    }
    return moved;
}

/*
 * from_truck trying to make from_truck's duration smaller
 * - maybe all orders can be moved
 *   - if that is the case, the from_truck could be removed
 *
 * Deleting an order on the from_truck
 * - number of truck remains the same
 * - from_truk duration() can not get larger
 * - the overall duration can get larger
 * 
 */
bool
Optimize::move_reduce_cost(size_t from_pos, size_t to_pos) {
    pgassert(to_pos < from_pos);
    auto from_truck = fleet[from_pos];
    auto to_truck = fleet[to_pos];
    auto moved(false);

    auto from_orders = from_truck.orders_in_vehicle();
    while (!from_orders.empty()) {
        /*
         * removing an order decreases the duration
         */
        auto order = from_truck.orders()[from_orders.front()];
        from_orders -= order.id();

        /*
         * insert it in the next truck
         */
        to_truck.insert(order);
        if (to_truck.is_feasable()) {
            problem->log
                << "\n    Move order " << order.pickup().original_id()
                << " from truck " << from_truck.id()
                << " to truck " << to_truck.id();
#ifndef NDEBUG
            problem->dbg_log << "\nMove before:";
            problem->dbg_log << "\n" << fleet[to_pos].tau();
            problem->dbg_log << "\n" << fleet[from_pos].tau();
#endif

            from_truck.erase(order);
            move_order(order, fleet[from_pos], fleet[to_pos]);
            moved = true;
            save_if_best();

#ifndef NDEBUG
            problem->dbg_log << "\nMove after:";
            problem->dbg_log << "\n" << fleet[to_pos].tau();
            problem->dbg_log << "\n" << fleet[from_pos].tau();
#endif
        }
    }
    return moved;
}



/*
 * from_truck: position of the truck where the order is
 * to truck: truck to put the order
 */
void
Optimize::move_order(
        Order order,
        Vehicle_pickDeliver &from_truck,
        Vehicle_pickDeliver &to_truck) {
    pgassert(from_truck.has_order(order));
    pgassert(!to_truck.has_order(order));

    from_truck.erase(order);
    to_truck.insert(order);

    pgassert(!from_truck.has_order(order));
    pgassert(to_truck.has_order(order));
}

void
Optimize::sort_for_move() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.total_wait_time() > rhs.total_wait_time();
            });

    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.orders_size() > rhs.orders_size();
            });
}


/*
 * Optimize decreasing truck
 *
 * - Objective: try to remove truck with less duration
 *
 * Step 1: Sort the fleet, duration DESC
 * Step 2: grab an order from the back of the the fleet
 * Step 3: cycle the fleet & insert in best truck possible
 */
void
Optimize::decrease_truck() {
    bool decreased(false);
    for (auto i = fleet.size(); i > 1; --i) {
        decreased = decrease_truck(i) || decreased;
    }
    if (decreased) {
        delete_empty_truck();
        save_if_best();
        decrease_truck();
    }
    save_if_best();
}

bool
Optimize::decrease_truck(size_t cycle) {
    /* end recursion */
    if (cycle < 2) return false;


    auto position = cycle - 1;
    for (auto orders = fleet[position].orders_in_vehicle();
            !orders.empty();
            orders.pop_front()) {

        /* Step 2: grab an order */
        auto order = fleet[position].orders()[orders.front()];
        pgassert(order.id() == orders.front());


        /* Step 3: cycle the fleet (in reverse order) & insert in first truck possible */

        // auto best_truck = fleet[position -1];
        for (size_t i = 0; i < position; ++i) {
            fleet[i].insert(order);
            if (!fleet[i].is_feasable()) {
                fleet[i].erase(order);
            } else {
                /* 
                 * delete the order from the current truck
                 */
                fleet[position].erase(order);
                break;
            }
        }
    }
    return fleet[position].orders_in_vehicle().empty();
}

void
Optimize::save_if_best() {
    if (duration() < best_solution.duration()) {
        best_solution = (*this);
        problem->log << "\n*********** best by duration"
            << best_solution.cost_str();
#ifndef NDEBUG
        problem->dbg_log << best_solution.tau("best by duration");
#endif
    }
    if (fleet.size() < best_solution.fleet.size()) {
        best_solution = (*this);
        problem->log << "\n*********** best by fleet size"
            << best_solution.cost_str();
#ifndef NDEBUG
        problem->dbg_log << best_solution.tau("best by fleet size");
#endif
    }
}


}  //  namespace vrp
}  //  namespace pgrouting
