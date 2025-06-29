/*PGR-GNU*****************************************************************
File: optimize.cpp

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

#include "cpp_common/assert.hpp"

#include "vrp/solution.hpp"
#include "vrp/optimize.hpp"
#include "vrp/pickDeliver.hpp"

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
        msg().log << tau("bestSol before sort by size");
        sort_by_size();
        msg().log << tau("bestSol after sort by size");
        msg().log <<  tau();
    }


void
Optimize::inter_swap(size_t times) {
    msg().log << tau("before sort by size");
    sort_by_size();
    msg().log << tau("before decrease");
    decrease_truck();
    msg().log << tau("after decrease");
    sort_by_size();
    msg().log << tau("after sort by size");

    size_t i = 0;
    while (i++ < times) {
        msg().log << "\n*************************** CYCLE" << i;
        inter_swap();
        msg().log << tau("after inter swap");
        std::rotate(fleet.begin(), fleet.begin() + 1, fleet.end());
        msg().log << tau("before next cycle");
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
    msg().log
        << "\n" <<tau("before inter swap");
    delete_empty_truck();
    auto swapped_f = false;
    /*
     *   .. to ... from ....
     */
    for (auto &from : fleet) {
        for (auto &to : fleet) {
            if (&from == &to) break;

            swap_worse(to, from);
            move_reduce_cost(from, to);
        }
    }

    msg().log
        << "\n" <<tau("after");
    delete_empty_truck();

    return swapped_f;
}



/*
 *   .. to ... from ....
 */
bool
Optimize::swap_worse(Vehicle_pickDeliver &to, Vehicle_pickDeliver &from) {
    /*
     * working on a copy of the vehicles
     */
    auto from_truck = from;
    auto to_truck = to;

    auto swapped = false;

    /*
     * To avoid invalidation of cycles
     */
    auto o_from_orders = from_truck.orders_in_vehicle();
    auto o_to_orders = to_truck.orders_in_vehicle();

    pgassert((o_from_orders * o_to_orders).empty());

    for (auto from_orders = from_truck.orders_in_vehicle();
            !from_orders.empty();
            from_orders.pop_front()) {
        auto from_order = from_truck.orders()[from_orders.front()];

        pgassert(from_truck.has_order(from_order));

        if (move_order(from_order, from_truck, to_truck)) {
            pgassert(!from_truck.has_order(from_order));
            pgassert(to_truck.has_order(from_order));
            /*
             * The order could be moved to "to" truck
             */
            to = to_truck;
            from = from_truck;
            /*
             * go to next order
             */
            pgassert(!from.has_order(from_order));
            pgassert(to.has_order(from_order));
            pgassert(!from_truck.has_order(from_order));
            pgassert(to_truck.has_order(from_order));
            pgassert(!(from.has_order(from_order) && to.has_order(from_order)));
            pgassert(from.has_order(from_order) || to.has_order(from_order));
            continue;
        }

        pgassert(from_truck.has_order(from_order));
        auto curr_from_duration = from_truck.duration();

        for (auto to_order_id : o_to_orders) {
            auto to_order = to.orders()[to_order_id];
            /*
             * The orders might have being swapped before
             */
            if (!to_truck.has_order(to_order)) continue;
            // if (!from_truck.has_order(from_order)) break; // TODO delete line

            pgassert(from_truck.has_order(from_order));
            pgassert(to_truck.has_order(to_order));

            auto curr_to_duration = to_truck.duration();

            /*
             * delete from_order, and to order from their trucks
             */
            pgassert(from_truck.has_order(from_order));
            from_truck.erase(from_order);
            pgassert(to_truck.has_order(to_order));
            to_truck.erase(to_order);

            /*
             * insert them in the other truck
             */
            if (this->get_kind() == OneDepot) {
                from_truck.semiLIFO(to_order);
                to_truck.semiLIFO(from_order);
            } else {
                from_truck.insert(to_order);
                to_truck.insert(from_order);
            }

            pgassert((from_truck.has_order(to_order) && from_truck.is_feasable()) || !from_truck.has_order(to_order));
            pgassert((to_truck.has_order(from_order) && to_truck.is_feasable()) || !to_truck.has_order(from_order));

            if (from_truck.has_order(to_order) && to_truck.has_order(from_order)) {
                auto new_from_duration = from_truck.duration();
                auto new_to_duration = to_truck.duration();

                auto estimated_delta =
                    - (curr_from_duration + curr_to_duration)
                    + (new_to_duration + new_from_duration);

                auto estimated_duration = duration() + estimated_delta;

                /*
                 * Can swap when:
                 *   - or from_truck duration is reduced
                 *   - the total fleet duration is reduced
                 *   - or the new fleet duration is better than best_solution duration
                 */
                if (new_from_duration < curr_from_duration ||
                        estimated_delta < 0 ||
                        estimated_duration < best_solution.duration()) {
                    /*
                     * this actually makes the swapping
                     */
                    to = to_truck;
                    from = from_truck;

                    pgassert(!(from.has_order(from_order) && to.has_order(from_order)));
                    pgassert(from.has_order(from_order) || to.has_order(from_order));
                    pgassert(!(from.has_order(to_order) && to.has_order(to_order)));
                    pgassert(from.has_order(to_order) || to.has_order(to_order));

                    swapped = true;
                    break;
                }
            }
            /*
             * Can't swap, restore vehicles
             */
            to_truck = to;
            from_truck = from;

            pgassert(!(from.has_order(from_order) && to.has_order(from_order)));
            pgassert(from.has_order(from_order) || to.has_order(from_order));
            pgassert(!(from.has_order(to_order) && to.has_order(to_order)));
            pgassert(from.has_order(to_order) || to.has_order(to_order));
        }
    }

    return swapped;
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
    pgassert(&from != &to);

    auto from_truck = from;
    auto to_truck = to;
    /*
     * don't move to empty truck
     */
    if (to_truck.empty()) return false;

    /*
     * don't move from a real truck to a phoney truck
     */
    if (!from_truck.is_phony() && to_truck.is_phony()) {
        return false;
    }

    auto moved = false;

    auto from_orders = from_truck.orders_in_vehicle();
    for (const auto o_id : from_orders) {
        /*
         * removing an order decreases the duration
         */
        auto order = from_truck.orders()[o_id];

        auto curr_duration = from_truck.duration() + to_truck.duration();
        /*
         * insert it in the "to" truck
         */
        pgassert(!to_truck.has_order(order));
        this->get_kind() == OneDepot?
            to_truck.semiLIFO(order) :
            to_truck.insert(order);
        pgassert((to_truck.has_order(order) && to_truck.is_feasable()) || !to_truck.has_order(order));

        if (to_truck.has_order(order)) {
            from_truck.erase(order);

            auto new_duration = from_truck.duration() + to_truck.duration();

            /*
             * cost is reduced
             */
            if (new_duration < curr_duration
                   || from_truck.empty()
                   || new_duration < best_solution.duration()) {
                moved = true;
                save_if_best();
                continue;
            }

            /*
             * cost is not reduced
             * revert changes
             */
            to_truck.erase(order);
            this->get_kind() == OneDepot?
                from_truck.semiLIFO(order) :
                from_truck.insert(order);
        }
    }
    return moved;
}



/*! @brief moves an order to an non empty vehicle
 *
 * order: order to be moved
 * from_truck: here is the order
 * to truck: truck to put the order
 *
 *
 * to check if the order was inserted:
 * to_truck.has_order(order)
 *
 * @return bool: when move was done
 */
bool
Optimize::move_order(
        Order order,
        Vehicle_pickDeliver &from_truck,
        Vehicle_pickDeliver &to_truck) {
    pgassert(from_truck.has_order(order));
    pgassert(!to_truck.has_order(order));
    /*
     * don't move to empty truck
     */
    if (to_truck.empty()) return false;

    /*
     * don't move from a real truck to a phoney truck
     */
    if (!from_truck.is_phony() && to_truck.is_phony()) return false;

    /*
     * Don't move from a vehicle with more orders
     */
    if (from_truck.size() > to_truck.size()) return false;

    /*
     * insert the order
     */
    this->get_kind() == OneDepot?
        to_truck.semiLIFO(order) :
        to_truck.insert(order);

    if (to_truck.has_order(order)) {
        from_truck.erase(order);

        pgassert(!(from_truck.has_order(order) && to_truck.has_order(order)));
        pgassert(!from_truck.has_order(order));
        pgassert(to_truck.has_order(order));
        return true;
    }

    pgassert(!(from_truck.has_order(order) && to_truck.has_order(order)));
    pgassert(from_truck.has_order(order));
    pgassert(!to_truck.has_order(order));
    return false;
}



/*
 * Optimize decreasing truck
 *
 * - Objective: try to remove truck with less duration
 * - Secondary objective, acts like a shake operation
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
            pgassert((fleet[i].has_order(order) && fleet[i].is_feasable()) || !fleet[i].has_order(order));
            if (fleet[i].has_order(order)) {
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
        msg().log << "\n*********** best by duration"
            << best_solution.cost_str();
    }
    if (fleet.size() < best_solution.fleet.size()) {
        best_solution = (*this);
        msg().log << "\n*********** best by fleet size"
            << best_solution.cost_str();
    }
}


}  //  namespace vrp
}  //  namespace pgrouting
