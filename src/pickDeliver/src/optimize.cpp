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
            case 2:
                move_wait_time_based();
                break;
            case 3:    
                inter_swap();
                break;
        }
    }


void
Optimize::inter_swap() {
    auto local_limit(fleet.size());
    size_t i(0);
    while (inter_swap(false) && (++i < local_limit)) {
        problem->log << "\ti" << i;
    };
    i = 0;
    while (inter_swap(true) && (++i < local_limit)) {
        problem->log << "\ti" << i;
    };
    sort_for_inter_swap();
    if (fleet.back().empty()) {
        problem->log << "empty truck";
        fleet.pop_back();
    }
}

bool
Optimize::inter_swap(bool reversed) {
    problem->log << tau("before sort");
    sort_for_inter_swap();
    if (fleet.back().empty()) {
        problem->log << "empty truck";
        fleet.pop_back();
    }
    if (reversed) {
        std::reverse(fleet.begin(), fleet.end());
    }
    problem->log << tau("after sort");
    auto swapped = false;
    std::ostringstream title;
    size_t from_pos(fleet.size()-1);
    while (from_pos > 1) {
        size_t to_pos(0);
        while (to_pos < from_pos) {
            if (swap_worse(from_pos, to_pos)) {
                title << "\n swapped (" << fleet[from_pos].id()  << ", " << fleet[to_pos].id() << ")";
                problem->log << tau(title.str());
                swapped = true;
            }
            if (move_reduce_cost(from_pos, to_pos)) {
                swapped = true;
                title << "\n moved (" << fleet[from_pos].id()  << ", " << fleet[to_pos].id() << ")";
                problem->log << tau(title.str());
            }
            ++to_pos;
        }
        --from_pos;
    }
    problem->log << "\nswapped" << swapped;
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
    while (!from_orders.empty()) {
        auto from_order(from_truck.get_worse_order(from_orders));
        from_orders.erase(from_order.id());

        while (!to_orders.empty()) {
            auto to_order(to_truck.get_worse_order(to_orders));
            to_orders.erase(to_order.id());
            /*
             * from_order, to order are the ones I want to exchange
             */

            /*
             * delete from_order, and to order from their trucks
             */
            auto curr_from_duration(from_truck.duration());
            auto curr_to_duration(to_truck.duration());

#if 0
            problem->log << "\nORIGINAL";
            problem->log << "\n" << to_truck.tau();
            problem->log << "\n" << from_truck.tau();
#endif
            pgassert(from_truck.has_order(from_order));
            pgassert(to_truck.has_order(to_order));
            from_truck.erase(from_order);
            to_truck.erase(to_order);
            pgassert(!from_truck.has_order(from_order));
            pgassert(!to_truck.has_order(to_order));

            /*
             * insert them in the other truck
             */
            pgassert(!from_truck.has_order(to_order));
            pgassert(!to_truck.has_order(from_order));
            from_truck.insert(to_order);
            to_truck.insert(from_order);
            pgassert(from_truck.has_order(to_order));
            pgassert(to_truck.has_order(from_order));

#if 0
            problem->log << "\nTRYING";
            problem->log << "\n" << to_truck.tau();
            problem->log << "\n" << from_truck.tau();
#endif
            if (from_truck.is_feasable() && to_truck.is_feasable()) {
                /*
                 * Can swap but:
                 *   - only swap when the total duration is reduced
                 *   - or from_truck duration is reduced --TODO
                 */

                if (((from_truck.duration() + to_truck.duration())
                            < (curr_from_duration + curr_to_duration))
                        || (from_truck.duration() < curr_from_duration)) {
                    pgassert(fleet[from_pos].has_order(from_order));
                    pgassert(fleet[to_pos].has_order(to_order));
                    fleet[from_pos].erase(from_order);
                    fleet[to_pos].erase(to_order);
                    pgassert(!fleet[from_pos].has_order(from_order));
                    pgassert(!fleet[to_pos].has_order(to_order));

                    pgassert(!fleet[from_pos].has_order(to_order));
                    pgassert(!fleet[to_pos].has_order(from_order));
                    fleet[from_pos].insert(to_order);
                    fleet[to_pos].insert(from_order);
                    pgassert(fleet[from_pos].has_order(to_order));
                    pgassert(fleet[to_pos].has_order(from_order));
                    swapped = true;
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


void
Optimize::sort_for_inter_swap() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.duration() > rhs.duration();
            });
}


void
Optimize::move_wait_time_based() {
    auto local_limit(fleet.size());
    size_t i(0);
    while (move_reduce_cost() && i < local_limit) {++i;};
}

bool
Optimize::move_reduce_cost() {
    if (fleet.size() < 2) return false;

    auto moved(false);
    sort_for_move();
    size_t from_pos(fleet.size()-1);
    while (from_pos > 1) {
        size_t to_pos(0);
        while (to_pos < from_pos) {
            if (move_reduce_cost(from_pos, to_pos)) {
                moved = true;
                std::ostringstream title;
                title << "\n success (" << fleet[from_pos].id()  << ", " << fleet[to_pos].id() << ")";
                problem->log << title.str();
                if (fleet[from_pos].empty()) {
                    problem->log << " empty truck";
                    fleet.erase(fleet.begin() + from_pos);
                    sort_for_move();
                    break;
                }
                sort_for_move();
            }
            // if (to_pos == 0) break; 
            ++to_pos;
        }
        --from_pos;
    }
    return moved;
}

/*
 * The small wants to get smaller
 * (maybe dissapear)
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
            /* the order can be inserted without a swap */
            from_truck.erase(order);
            move_order(order, fleet[from_pos], fleet[to_pos]);
            moved = true;
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


void
Optimize::sort() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.duration() < rhs.duration();
            });

#if 0
    std::stable_sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.orders_size() < rhs.orders_size();
            });
#endif
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
    bool decreased(true);
    while (decreased) {
        decreased = false;
        sort();
        decrease_truck(fleet.size(), decreased);
    }
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

        /* Step 3: cycle the fleet & insert in first truck possible */
        /* current truck is tried last */
        err_log << " trying ";
        auto best_truck(fleet.size() - 1);
        auto current_duration(duration());
        auto min_delta_duration = std::numeric_limits<double>::max();
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
    }

    if (fleet.back().empty()) {
        decreased = true;
        fleet.pop_back();
    }
    decrease_truck(--cycle, decreased);
}

}  //  namespace vrp
}  //  namespace pgRouting
