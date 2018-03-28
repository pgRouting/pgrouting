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

#include "cpp_common/pgr_assert.h"

#include "vrp/solution.h"
#include "vrp/book_keeping.h"
#include "vrp/optimize.h"
#include "vrp/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {


Optimize::Optimize(
        const Solution &old_solution) :
    Solution(old_solution),
    best_solution(old_solution)  {
        pgassert(false);
        decrease_truck();
        inter_swap(fleet.size());
    }

Optimize::Optimize(
        const Solution &old_solution,
        size_t times) :
    Solution(old_solution),
    best_solution(old_solution)  {
        inter_swap(times);

        this->fleet = best_solution.fleet;
        msg.log << tau("bestSol before sort by size");
        sort_by_size();
        msg.log << tau("bestSol after sort by size");
        msg.log <<  tau();
    }


void
Optimize::inter_swap(size_t times) {
    msg.log << tau("before sort by size");
    sort_by_size();
    msg.log << tau("before decrease");
    decrease_truck();
    msg.log << tau("after decrease");
    sort_by_size();
    msg.log << tau("after sort by size");

    size_t i = 0;
    while ((i++ < times) && inter_swap()) {
        msg.log << tau("after inter swap");
        msg.log << "\n***************************" << i;
        std::rotate(fleet.begin(), fleet.begin() + 1, fleet.end());
        msg.log << tau("before next cycle");
    }
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
Optimize::inter_swap() {
    msg.log
        << "\n" <<tau("before inter swap");
    delete_empty_truck();
    auto swapped_f = false;
    /*
     *   .. to ... from ....
     */
    for (auto &from : fleet) {
        for (auto &to : fleet) {
            if (&from == &to) break;

#if 0
            msg.log
                << "\n to " << to.id()
                << "from " << from.id();
            auto swapped = false;
#endif
            swap_worse(to, from);
            swapped_f = swap_order() || swapped_f;
            move_reduce_cost(from, to);
#if 0
            msg.log << "++++++++" << p_swaps;
#endif
        }
    }
    while (!p_swaps.empty()) {
        swapped_f = swap_order() || swapped_f;
    }

    msg.log
        << "\n" <<tau("after");
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

#if 0
    auto best_from_order = from_truck.orders_in_vehicle().front();
    auto best_to_order = to_truck.orders_in_vehicle().front();
#endif
    for (auto from_orders = from_truck.orders_in_vehicle();
            !from_orders.empty();
            from_orders.pop_front()) {
        auto from_order = from_truck.orders()[from_orders.front()];
#if 0
        pgassert(from_truck.has_order(from_order));
        msg.log << "\n" << from_orders;
        msg.log << "\n from " << from_order.idx()
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
            msg.log << "\n" << to_orders;
            msg.log << "\n To " << to_order.idx();
#endif
            auto curr_to_duration = to_truck.duration();

            /*
             * delete from_order, and to order from their trucks
             */
#if 0
            pgassert(from_truck.has_order(from_order));
            msg.log << "\n" << from_truck.tau();
            msg.log << "\n" << from_order.idx();
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
                msg.log << "\n Can swap";
                msg.log << "\n Curr_from_duration " << curr_from_duration;
                msg.log << " Curr_to_duration " << curr_to_duration;
                msg.log << "\n  new_from_duration "
                    << from_truck.duration();
                msg.log << "  new_to_duration " << to_truck.duration();
#endif
                auto estimated_delta =
                    - (curr_from_duration + curr_to_duration)
                    + (to_truck.duration() + from_truck.duration());

#if 1
                auto estimated_duration = duration() + estimated_delta;

                if (from_truck.duration() < curr_from_duration ||
                        estimated_delta < 0 ||
                        estimated_duration < best_solution.duration()) {
#endif
                    msg.log
                        << "\n Found Swap order "
                        << from_order.pickup().id()
                        << " from truck " << from_truck.idx()
                        << " with order " << to_order.pickup().id()
                        << " of truck " << to_truck.idx();

                    swapped = true;
#if 0
                    best_to_order = to_order.idx();
                    best_from_order = from_order.idx();
#endif
                    p_swaps.push(
                            Swap_info(
                                from,
                                to,
                                from_order.idx(),
                                to_order.idx(),
                                estimated_delta));
#if 1
                }
#endif
            }
            to_truck = to;
            from_truck = from;
        }
        from_truck = from;
    }

    return false && swapped;
}


bool
Optimize::swap_order() {
#if 0
    msg.log << "++++++++" << p_swaps;
#endif
    while (!p_swaps.empty()) {
        auto swap_data = p_swaps.top();
        p_swaps.pop();
        size_t from_pos = 0;
        size_t to_pos = 0;

        for (; from_pos < fleet.size()
                && fleet[from_pos].idx() != swap_data.from_truck.idx()
                ; ++from_pos) {
        }
        pgassert(from_pos < fleet.size());
        for (; to_pos < fleet.size()
                && fleet[to_pos].idx() != swap_data.to_truck.idx()
                ; ++to_pos) {
        }
        pgassert(to_pos < fleet.size());

        if (swap_order(
                fleet[from_pos].orders()[swap_data.from_order], fleet[from_pos],
                fleet[to_pos].orders()[swap_data.to_order], fleet[to_pos])) {
            save_if_best();
#if 0
            msg.log
                << "\n Swapping order "
                << fleet[from_pos].orders()[
                    swap_data.from_order].pickup().original_id()
                << " from truck " << fleet[from_pos].id()
                << " with order "
                << fleet[to_pos].orders()[
                    swap_data.to_order].pickup().original_id()
                << " of truck " <<  fleet[to_pos].id();
#endif
#if 0
            msg.log << "\nswappping after:";
            msg.log << "\n" <<  fleet[to_pos].tau();
            msg.log << "\n" << fleet[from_pos].tau();
#endif
            return true;
        }
    }
    return false;
}

/*
 * from_truck: position of the truck where the order is
 * to truck: truck to put the order
 */
bool
Optimize::swap_order(
        const Order from_order,
        Vehicle_pickDeliver &from_truck,
        const Order to_order,
        Vehicle_pickDeliver &to_truck) {
    if (!from_truck.has_order(from_order)
            || !to_truck.has_order(to_order)) {
        return false;
    }

    pgassert(from_truck.has_order(from_order));
    pgassert(to_truck.has_order(to_order));

    from_truck.erase(from_order);
    to_truck.erase(to_order);

    from_truck.insert(to_order);
    to_truck.insert(from_order);


    pgassert(from_truck.has_order(to_order));
    pgassert(to_truck.has_order(from_order));
    return true;
}

void
Optimize::sort_by_id() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.orders_in_vehicle().size()
            > rhs.orders_in_vehicle().size();
            });
}

void
Optimize::sort_by_size() {
    sort_by_duration();
    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.orders_in_vehicle().size()
            > rhs.orders_in_vehicle().size();
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

#if 0
void
Optimize::move_duration_based() {
    auto local_limit(fleet.size());
    size_t i(0);

    sort_by_duration();
    msg.log << tau("\nmove duration based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    delete_empty_truck();

    i = 0;
    sort_by_duration();
    std::reverse(fleet.begin(), fleet.end());
    msg.log << tau("\nmove duration based");
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
    msg.log << tau("\nmove wait_time based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    delete_empty_truck();

    i = 0;
    sort_for_move();
    std::reverse(fleet.begin(), fleet.end());
    msg.log << tau("\nmove wait_time based");
    while (move_reduce_cost() && (++i < local_limit)) { }
    sort_by_duration();
    delete_empty_truck();
    this->fleet = best_solution.fleet;
}
#endif

#if 0
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
#endif

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
Optimize::move_reduce_cost(
        Vehicle_pickDeliver &from,
        Vehicle_pickDeliver &to) {
    auto from_truck = from;
    auto to_truck = to;

    /*
     * don't move from a real truck to a phoney truck
     */
    if (!from_truck.is_phony() && to_truck.is_phony()) {
        return false;
    }
#if 0
    from.id()  > to.id()
        ?  to : from;
#endif
    size_t from_pos = 0;
    size_t to_pos = 0;

    for (; from_pos < fleet.size()
            && fleet[from_pos].idx() != from_truck.idx()
            ; ++from_pos) {
    }
    pgassert(from_pos < fleet.size());
    for (; to_pos < fleet.size()
            && fleet[to_pos].idx() != to_truck.idx()
            ; ++to_pos) {
    }
    pgassert(to_pos < fleet.size());

    auto moved = false;

    auto from_orders = from_truck.orders_in_vehicle();
    while (!from_orders.empty()) {
        /*
         * removing an order decreases the duration
         */
        auto order = from_truck.orders()[from_orders.front()];
        from_orders -= order.idx();

        /*
         * insert it in the "to" truck
         */
        to_truck.insert(order);
        if (to_truck.is_feasable()) {
            msg.log
                << "\n    Move order " << order.pickup().id()
                << " from truck " << from_truck.idx()
                << " to truck " << to_truck.idx();
#ifndef NDEBUG
            msg.dbg_log << "\nMove before:";
            msg.dbg_log << "\n" << fleet[to_pos].tau();
            msg.dbg_log << "\n" << fleet[from_pos].tau();
#endif

#if 1
            from_truck.erase(order);
#else
            to_truck.insert(order);
            move_order(order, fleet[from_pos], fleet[to_pos]);
#endif
            moved = true;
            save_if_best();

#ifndef NDEBUG
            msg.dbg_log << "\nMove after:";
            msg.dbg_log << "\n" << fleet[to_pos].tau();
            msg.dbg_log << "\n" << fleet[from_pos].tau();
#endif
        } else {
            to_truck.erase(order);
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
 * - Secundary objective, acts like a shake operation
 *
 */
void
Optimize::decrease_truck() {
    bool decreased(false);
    for (size_t i = 1; i < fleet.size(); ++i) {
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
    auto position = cycle;
    for (auto orders = fleet[position].orders_in_vehicle();
            !orders.empty();
            orders.pop_front()) {
        /* Step 2: grab an order */
        auto order = fleet[position].orders()[orders.front()];
        pgassert(order.idx() == orders.front());


        /* Step 3:
         * cycle the fleet
         * insert in first truck possible
         */

        for (size_t i = 0; i < position; ++i) {
            fleet[i].insert(order);
            if (fleet[i].is_feasable()) {
                /*
                 * delete the order from the current truck
                 */
                fleet[position].erase(order);
                break;
            } else {
                fleet[i].erase(order);
            }
        }
    }
    return fleet[position].orders_in_vehicle().empty();
}

void
Optimize::save_if_best() {
    if (duration() < best_solution.duration()) {
        best_solution = (*this);
        msg.log << "\n*********** best by duration"
            << best_solution.cost_str();
#ifndef NDEBUG
        msg.dbg_log << best_solution.tau("best by duration");
#endif
    }
    if (fleet.size() < best_solution.fleet.size()) {
        best_solution = (*this);
        msg.log << "\n*********** best by fleet size"
            << best_solution.cost_str();
#ifndef NDEBUG
        msg.dbg_log << best_solution.tau("best by fleet size");
#endif
    }
}


}  //  namespace vrp
}  //  namespace pgrouting
