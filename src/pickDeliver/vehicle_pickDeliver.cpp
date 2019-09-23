/*PGR-GNU*****************************************************************

FILE: vehicle_pickDeliver.cpp

Copyright (c) 2016 pgRouting developers
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

#include "vrp/vehicle_pickDeliver.h"

#include <iostream>
#include <deque>
#include <set>
#include <string>
#include <sstream>
#include <limits>


#include "cpp_common/pgr_assert.h"
#include "vrp/order.h"
#include "vrp/vehicle.h"
#include "vrp/pgr_pickDeliver.h"



namespace pgrouting {
namespace vrp {

Order
Vehicle_pickDeliver::get_worse_order(
        Identifiers<size_t> orders) const {
    invariant();
    pgassert(!empty());

    // auto orders(of_this_subset);
    auto worse_order(m_orders[*orders.begin()]);
    auto delta_duration((std::numeric_limits<double>::max)());
    auto curr_duration(duration());
    while (!orders.empty()) {
        auto truck(*this);
        auto order = m_orders[*orders.begin()];
        pgassert(truck.has_order(order));
        orders -= order.idx();
        truck.erase(order);
        auto delta = truck.duration() - curr_duration;
        if (delta < delta_duration) {
            worse_order = order;
            delta_duration = delta;
        }
    }
    return worse_order;
}


Order
Vehicle_pickDeliver::get_first_order() const {
    invariant();
    pgassert(!empty());
    return m_orders[m_path[1].idx()];
}


Vehicle_pickDeliver::Vehicle_pickDeliver(
        size_t id,
        size_t kind,
        const Vehicle_node &starting_site,
        const Vehicle_node &ending_site,
        double p_capacity,
        double p_speed,
        double factor) :
    Vehicle(id, kind, starting_site, ending_site, p_capacity, p_speed, factor),
    cost((std::numeric_limits<double>::max)()) {
#if 0
        ENTERING();
#endif
        m_orders_in_vehicle.clear();
        invariant();
#if 0
        EXITING();
#endif
    }



bool
Vehicle_pickDeliver::has_order(const Order &order) const {
    return m_orders_in_vehicle.has(order.idx());
}


bool
Vehicle_pickDeliver::insert(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    auto pick_pos(position_limits(order.pickup()));
    auto deliver_pos(position_limits(order.delivery()));
#ifndef NDEBUG
    std::ostringstream err_log;
    err_log << "\n\tpickup limits (low, high) = ("
        << pick_pos.first << ", "
        << pick_pos.second << ") "
        << "\n\tdeliver limits (low, high) = ("
        << deliver_pos.first << ", "
        << deliver_pos.second << ") "
        << "\noriginal" << tau();
#endif

    if (pick_pos.second < pick_pos.first) {
        /*
         *  pickup generates twv evrywhere
         */
        return false;
    }

    if (deliver_pos.second < deliver_pos.first) {
        /*
         *  delivery generates twv evrywhere
         */
        return false;
    }
    /*
     * Because delivery positions were estimated without
     * the pickup:
     *   - increase the upper limit position estimation
     */
    ++deliver_pos.first;
    ++deliver_pos.second;


    auto d_pos_backup(deliver_pos);
    auto best_pick_pos = m_path.size();
    auto best_deliver_pos = m_path.size() + 1;
    auto current_duration(duration());
    auto min_delta_duration = (std::numeric_limits<double>::max)();
    auto found(false);
    pgassertwm(!has_order(order), err_log.str());
    while (pick_pos.first <= pick_pos.second) {
#ifndef NDEBUG
        err_log << "\n\tpickup cycle limits (low, high) = ("
            << pick_pos.first << ", "
            << pick_pos.second << ") ";
#endif
        Vehicle::insert(pick_pos.first, order.pickup());
#ifndef NDEBUG
        err_log << "\npickup inserted: " << tau();
#endif

        if (deliver_pos.first <= pick_pos.first) deliver_pos.first = pick_pos.first + 1;

        while (deliver_pos.first <= deliver_pos.second) {
            Vehicle::insert(deliver_pos.first, order.delivery());
            m_orders_in_vehicle += order.idx();
            pgassertwm(has_order(order), err_log.str());
#ifndef NDEBUG
            err_log << "\ndelivery inserted: " << tau();
#endif
            if (is_feasable()) {
                pgassert(is_feasable());
                auto delta_duration = duration() - current_duration;
                if (delta_duration < min_delta_duration) {
#ifndef NDEBUG
                    err_log << "\nsuccess" << tau();
#endif
                    min_delta_duration = delta_duration;
                    best_pick_pos = pick_pos.first;
                    best_deliver_pos = deliver_pos.first;
                    found = true;
                }
            }
            Vehicle::erase(deliver_pos.first);
#ifndef NDEBUG
            err_log << "\ndelivery erased: " << tau();
#endif
            ++deliver_pos.first;
        }
        Vehicle::erase(pick_pos.first);
#ifndef NDEBUG
        err_log << "\npickup erased: " << tau();
#endif
        m_orders_in_vehicle -= order.idx();
        pgassertwm(!has_order(order), err_log.str());

        deliver_pos = d_pos_backup;
#ifndef NDEBUG
        err_log << "\n\trestoring deliver limits (low, high) = ("
            << deliver_pos.first << ", "
            << deliver_pos.second << ") ";
#endif
        ++pick_pos.first;
    }
    pgassertwm(!has_order(order), err_log.str());
    if (!found) {
        /* order causes twv or cv */
        return false;
    }
    Vehicle::insert(best_pick_pos, order.pickup());
    Vehicle::insert(best_deliver_pos, order.delivery());

    m_orders_in_vehicle += order.idx();
    pgassertwm(is_feasable(), err_log.str());
    pgassertwm(has_order(order), err_log.str());
    pgassertwm(!has_cv(), err_log.str());
    invariant();
    return true;
}


void
Vehicle_pickDeliver::push_back(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    m_orders_in_vehicle += order.idx();
    m_path.insert(m_path.end() - 1, order.pickup());
    m_path.insert(m_path.end() - 1, order.delivery());
    evaluate(m_path.size() - 3);

    pgassert(has_order(order));
#if 0
    pgassert(!has_cv());
#endif
    invariant();
}


void
Vehicle_pickDeliver::push_front(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    m_orders_in_vehicle += order.idx();
    m_path.insert(m_path.begin() + 1, order.delivery());
    m_path.insert(m_path.begin() + 1, order.pickup());
    evaluate(1);

    pgassert(has_order(order));
#if 0
    pgassert(!has_cv());
#endif
    invariant();
}


void
Vehicle_pickDeliver::do_while_feasable(
        Initials_code kind,
        Identifiers<size_t> &unassigned,
        Identifiers<size_t> &assigned) {
    pgassert(is_feasable());
#if 0
    msg.log << "unasigned" << unassigned << "\n";
    msg.log << "m_feasable_orders" << m_feasable_orders << "\n";
#endif
    auto current_feasable = m_feasable_orders * unassigned;

    while (!current_feasable.empty()) {
#if 0
        msg.log << "current_feasable" << current_feasable << "\n";
#endif
        auto order = m_orders[current_feasable.front()];

        switch (kind) {
            case OnePerTruck:
                push_back(order);
                pgassert(is_feasable());
                assigned += order.idx();
                unassigned -= order.idx();
                invariant();
                return;
                break;
            case FrontTruck:
                push_front(order);
                break;
            case BackTruck:
                push_back(order);
                break;
            case BestInsert:
                insert(order);
                break;
            case BestBack:
                order = m_orders[m_orders.find_best_J(current_feasable)];
                insert(order);
                break;
            case BestFront:
                order = m_orders[m_orders.find_best_I(current_feasable)];
                insert(order);
                break;
            case OneDepot:
                semiLIFO(order);
                break;
            default: pgassert(false);
        }

        if (orders_size() == 1 && !is_feasable()) {
            pgassert(false);
        }

        if (!is_feasable()) {
            erase(order);
        } else if (has_order(order)) {
            assigned += order.idx();
            unassigned -= order.idx();
            if (kind == BestBack) {
                current_feasable = m_orders[order.idx()].subsetJ(
                        current_feasable);
            }
            if (kind == BestFront) {
                current_feasable = m_orders[order.idx()].subsetI(
                        current_feasable);
            }
        }

        current_feasable -= order.idx();
        invariant();
    }

    pgassert(is_feasable());
    invariant();
}


void
Vehicle_pickDeliver::erase(const Order &order) {
    invariant();
    pgassert(has_order(order));


    Vehicle::erase(order.pickup());
    Vehicle::erase(order.delivery());
    m_orders_in_vehicle -= order.idx();

    invariant();
    pgassert(!has_order(order));
}



size_t
Vehicle_pickDeliver::pop_back() {
    invariant();
    pgassert(!empty());

    auto pick_itr = m_path.rbegin();
    while (pick_itr != m_path.rend() &&  !pick_itr->is_pickup()) {
        ++pick_itr;
    }

    pgassert(pick_itr->is_pickup());

    auto deleted_pick_idx = pick_itr->idx();

    for (const auto o : m_orders) {
        if (o.pickup().idx() == deleted_pick_idx) {
            erase(o);
            invariant();
            return o.idx();
        }
    }
    pgassert(false);
    return 0;
}



size_t
Vehicle_pickDeliver::pop_front() {
    invariant();
    pgassert(!empty());

    auto pick_itr = m_path.begin();
    while (pick_itr != m_path.end() &&  !pick_itr->is_pickup()) {
        ++pick_itr;
    }

    pgassert(pick_itr->is_pickup());

    auto deleted_pick_idx = pick_itr->idx();

    for (const auto o : m_orders) {
        if (o.pickup().idx() == deleted_pick_idx) {
            erase(o);
            invariant();
            return o.idx();
        }
    }

    pgassert(false);
    return 0;
}

void
Vehicle_pickDeliver::set_compatibles(const PD_Orders &orders) {
    m_orders = orders;
    for (const auto o : orders) {
        if (is_order_feasable(o)) m_feasable_orders += o.idx();
    }
    m_orders.set_compatibles(speed());
}

bool
Vehicle_pickDeliver::is_order_feasable(const Order &order) const {
    auto test_truck =  *this;
    test_truck.push_back(order);
    return test_truck.is_feasable();
}

bool
Vehicle_pickDeliver::semiLIFO(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    /*
     * Insert pick up as first picked
     */
    Vehicle::insert(1, order.pickup());

    auto deliver_pos(drop_position_limits(order.delivery()));

    /*
     * delivery generates twv in all positions
     */
    if (deliver_pos.second < deliver_pos.first) {
        /*
         * Remove inserted pickup
         */
        Vehicle::erase(1);
        invariant();
        return false;
    }

    pgassert(!has_order(order));

    while (deliver_pos.first <= deliver_pos.second) {
        Vehicle::insert(deliver_pos.second, order.delivery());

        if (is_feasable() && !m_path[deliver_pos.second + 1].is_pickup()) {
            /*
             * Found a position to insert the delivery
             */


            m_orders_in_vehicle += order.idx();

            /*
             * There is one more order in the vehicle
             */
            pgassert(has_order(order));
            pgassert(is_feasable());
            pgassert(!has_cv());
            pgassert(!has_twv());
            pgassert(has_order(order));
            invariant();
            return true;
        }

        /*
         * This position in path is not suitable
         */
        Vehicle::erase(deliver_pos.second);

        /*
         * got to next position
         */
        --deliver_pos.second;
    }

    /*
     * Order could not be inserted
     */
    Vehicle::erase(1);

    pgassert(!has_order(order));
    invariant();
    return false;
}

}  //  namespace vrp
}  //  namespace pgrouting


