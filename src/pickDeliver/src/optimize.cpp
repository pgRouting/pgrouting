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
#include "./solution.h"
#include "./../../common/src/pgr_assert.h"
#include "./optimize.h"
#include "./pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {


Optimize::Optimize(
        int kind,
        const Solution &old_solution) :
    Solution(old_solution),
    best_solution(old_solution)  {
        switch (kind) {
            case 0:
                sort_by_duration();
                break;
            case 1:
                decrease_truck();
                break;
            case 2:
                move_duration_based();
                break;
            case 3:
                move_wait_time_based();
                break;
            case 4:
                inter_swap();
                break;
        }
        this->fleet = best_solution.fleet;
        sort_by_duration();
        delete_empty_truck();
    }


void
Optimize::inter_swap() {
    auto local_limit(fleet.size());
    size_t i(0);
    while (inter_swap(false) && (++i < local_limit)) {
    }
    i = 0;
    while (inter_swap(true) && (++i < local_limit)) {
    }
    sort_by_duration();
    delete_empty_truck();
    this->fleet = best_solution.fleet;
}

bool
Optimize::inter_swap(bool reversed) {
//    problem->log << tau("before sort");
    sort_by_duration();
    delete_empty_truck();
    save_if_best();
    if (reversed) {
        std::reverse(fleet.begin(), fleet.end());
    }
//    problem->log << tau("after sort");
    auto swapped = false;
    size_t from_pos(fleet.size()-1);
    while (from_pos > 1) {
        for (size_t to_pos = 0; to_pos < from_pos; ++to_pos) {
            swapped = swap_worse(from_pos, to_pos)? true : swapped;
            swapped = move_reduce_cost(from_pos, to_pos)? true : swapped;
        }
        delete_empty_truck();
        --from_pos;
    }
    return swapped;
}

bool
Optimize::swap_worse(size_t from_pos, size_t to_pos) {
    pgassert(to_pos < from_pos);
    auto from_truck = fleet[from_pos];
    auto to_truck = fleet[to_pos];
    auto swapped(false);
    auto from_orders(from_truck.orders_in_vehicle);
    auto to_orders(to_truck.orders_in_vehicle);
    auto local_limit(from_orders.size() * to_orders.size() + 1);

    while (!from_orders.empty() && --local_limit > 0) {
        auto from_order(from_truck.get_worse_order(from_orders));
        from_orders.erase(from_order.id());

        while (!to_orders.empty()) {
            auto to_order(to_truck.get_worse_order(to_orders));
            to_orders.erase(to_order.id());

            /*
             * delete from_order, and to order from their trucks
             */
            auto curr_from_duration(from_truck.duration());
            auto curr_to_duration(to_truck.duration());

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

                if (((from_truck.duration() + to_truck.duration())
                            < (curr_from_duration + curr_to_duration))
                        || (from_truck.duration() < curr_from_duration)) {
                    problem->log
                        << "\n    Swap order " << from_order.id()
                        << " from truck " << from_truck.id()
                        << " with order " << to_order.id() << " of truck " << to_truck.id();
#ifndef NDEBUG
                    problem->dbg_log << "\nswappping before:";
                    problem->dbg_log << "\n" << fleet[to_pos].tau();
                    problem->dbg_log << "\n" << fleet[from_pos].tau();
#endif

                    swap_order(from_order, fleet[from_pos], to_order, fleet[to_pos]);
                    swapped = true;
                    save_if_best();
                    from_orders.insert(to_order.id());
#ifndef NDEBUG
                    problem->dbg_log << "\nswappping after:";
                    problem->dbg_log << "\n" << fleet[to_pos].tau();
                    problem->dbg_log << "\n" << fleet[from_pos].tau();
#endif
                    break;
                }
            }
            /*
             * wasn't swapped
             */
            to_truck = fleet[to_pos];
            from_truck = fleet[from_pos];
        }
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
Optimize::sort_by_duration() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.duration() > rhs.duration();
            });
}

void
Optimize::delete_empty_truck() {
    while (fleet.back().empty()) {
        problem->log << "\nEmpty truck";
        fleet.pop_back();
        save_if_best();
    }
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

    size_t from_pos(fleet.size() - 1);
    while (from_pos > 1) {
        for (size_t to_pos = 0; to_pos < from_pos; ++to_pos) {
            // problem->log << "\nmove_reduce_cost (" << fleet[from_pos].id()  << ", " << fleet[to_pos].id() << ")";
            if (move_reduce_cost(from_pos, to_pos)) {
                if (fleet[from_pos].empty()) {
                    fleet.erase(fleet.begin() + from_pos);
                    save_if_best();
                }
                return true;
            }
        }
        --from_pos;
    }
    return false;
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

    auto orders(from_truck.orders_in_vehicle);
    while (!orders.empty()) {
        /*
         * get the order that decreases the duration the most
         * (there is allways a worse)
         */
        auto order = from_truck.get_worse_order(orders);
        orders.erase(order.id());

        /*
         * insert it in the next truck
         */
        to_truck.insert(order);
        if (to_truck.is_feasable()) {
            problem->log
                << "\n    Move order " << order.id()
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
    bool decreased(true);
    while (decreased) {
        decreased = false;
        sort_by_duration();
        std::reverse(fleet.begin(), fleet.end());
        decrease_truck(fleet.size(), decreased);
    }
    this->fleet = best_solution.fleet;
}

void
Optimize::decrease_truck(size_t cycle, bool &decreased) {
    /* end recursion */
    if (cycle == 0) return;

    std::ostringstream err_log;
    err_log << " --- Cycle " << cycle << "\n";

    /* the front truck move to back */
    std::rotate(fleet.begin(), fleet.begin() + 1, fleet.end());
    err_log << "\n after rotate" << tau();

    auto orders(fleet.back().orders_in_vehicle);
    while (!orders.empty()) {
        /* Step 2: grab an order */
        auto order(problem->orders()[*orders.begin()]);
        orders.erase(orders.begin());
        err_log << "\n truck with order: " << fleet.back().tau();
        err_log << "\nOrder" << order << "\n";

        /* Step 3: delete the order from the back of the fleet */
        pgassertwm(fleet.back().has_order(order), err_log.str());
        fleet.back().erase(order);
        pgassertwm(!fleet.back().has_order(order), err_log.str());

        /* Step 3: cycle the fleet & insert in best truck possible */
        /* current truck is tried last */
        err_log << " trying ";
        auto best_truck(fleet.size() - 1);
        auto current_duration(duration());
        auto min_delta_duration = (std::numeric_limits<double>::max)();
        size_t t_i(0);
        for (auto &truck : fleet) {
            truck.insert(order);
            if (!truck.is_feasable()) {
                err_log << "\n" << truck.tau();
            } else {
                err_log << "\n ******* success " << truck.tau() << "\n";
                auto delta_duration = duration()-current_duration;
                if (t_i != fleet.size() - 1
                        && (delta_duration < min_delta_duration)) {
                    min_delta_duration = delta_duration;
                    best_truck = t_i;
                }
            }
            truck.erase(order);
            ++t_i;
        }
        fleet[best_truck].insert(order);
        save_if_best();
    }

    if (fleet.back().empty()) {
        decreased = true;
        fleet.pop_back();
        save_if_best();
    }
    decrease_truck(--cycle, decreased);
}

void
Optimize::save_if_best() {
    if (duration() < best_solution.duration()) {
        best_solution = (*this);
        problem->log << "\n*********** best by duration" << best_solution.cost_str();
#ifndef NDEBUG
        problem->dbg_log << best_solution.tau("best by duration");
#endif
    }
    if (fleet.size() < best_solution.fleet.size()) {
        best_solution = (*this);
        problem->log << "\n*********** best by fleet size" << best_solution.cost_str();
#ifndef NDEBUG
        problem->dbg_log << best_solution.tau("best by fleet size");
#endif
    }
}


}  //  namespace vrp
}  //  namespace pgrouting
