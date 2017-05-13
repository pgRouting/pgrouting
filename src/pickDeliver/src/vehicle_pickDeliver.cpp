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

#include "pickDeliver/vehicle_pickDeliver.h"

#include <iostream>
#include <deque>
#include <set>
#include <string>
#include <sstream>
#include <limits>


#include "cpp_common/pgr_assert.h"
#include "pickDeliver/order.h"
#include "pickDeliver/vehicle.h"
#include "pickDeliver/pgr_pickDeliver.h"



namespace pgrouting {
namespace vrp {
namespace pickdeliver {

Order
Vehicle_pickDeliver::get_worse_order(
        Identifiers<size_t> orders) const {
    ENTERING();
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
    EXITING();
    return worse_order;
}


Order
Vehicle_pickDeliver::get_first_order() const {
    ENTERING();
    invariant();
    pgassert(!empty());
    EXITING();
    return m_orders[m_path[1].idx()];
}


Vehicle_pickDeliver::Vehicle_pickDeliver(
        size_t idx,
        int64_t kind,
        const Vehicle_node &starting_site,
        const Vehicle_node &ending_site,
        double p_capacity,
        double p_speed) :
    Vehicle(idx, kind, starting_site, ending_site, p_capacity, p_speed),
    cost((std::numeric_limits<double>::max)()) {
    ENTERING();
        m_orders_in_vehicle.clear();

        invariant();
    EXITING();
    }



bool
Vehicle_pickDeliver::has_order(const Order &order) const {
    ENTERING();
    return m_orders_in_vehicle.has(order.idx());
}


void
Vehicle_pickDeliver::insert(const Order &order) {
    ENTERING();
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
        /* pickup generates twv evrywhere,
         *  so put the order as last */
        push_back(order);
    EXITING();
        return;
    }

    if (deliver_pos.second < deliver_pos.first) {
        /* delivery generates twv evrywhere,
         *  so put the order as last */
        push_back(order);
    EXITING();
        return;
    }
    /*
     * Because delivery positions were estimated without
     * the pickup:
     *   - increase the upper limit position estimation
     */
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

        while (deliver_pos.first <= deliver_pos.second) {
            Vehicle::insert(deliver_pos.first, order.delivery());
            m_orders_in_vehicle += order.idx();
            pgassertwm(has_order(order), err_log.str());
#ifndef NDEBUG
            err_log << "\ndelivery inserted: " << tau();
#endif
            if (is_feasable()) {
                pgassert(is_feasable());
                auto delta_duration = duration()-current_duration;
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
        /* order causes twv
         *  so put the order as last */
        push_back(order);
    EXITING();
        return;
    }
    Vehicle::insert(best_pick_pos, order.pickup());
    Vehicle::insert(best_deliver_pos, order.delivery());

    m_orders_in_vehicle += order.idx();
    pgassertwm(is_feasable(), err_log.str());
    pgassertwm(has_order(order), err_log.str());
    pgassertwm(!has_cv(), err_log.str());
    invariant();
    EXITING();
}


void
Vehicle_pickDeliver::push_back(const Order &order) {
    ENTERING();
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
    EXITING();
}


void
Vehicle_pickDeliver::push_front(const Order &order) {
    ENTERING();
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
    EXITING();
}


void
Vehicle_pickDeliver::do_while_feasable(
        int kind,
        Identifiers<PD_Orders::OID> &unassigned,
        Identifiers<PD_Orders::OID> &assigned) {
    ENTERING();
    pgassert(is_feasable());
    auto current_feasable = m_feasable_orders * unassigned;

    while (!current_feasable.empty()) {
        auto order = m_orders[current_feasable.front()];

        switch (kind) {
            case 1:
                push_back(order);
                pgassert(is_feasable());
                assigned += order.idx();
                unassigned -= order.idx();
                invariant();
                return;
                break;
            case 2:
                push_back(order);
                break;
            case 3:
                push_front(order);
                break;
            case 4:
                insert(order);
                break;
            case 5:
                order = m_orders[m_orders.find_best_J(current_feasable)];
                insert(order);
                break;
            case 6:
                order = m_orders[m_orders.find_best_I(current_feasable)];
                insert(order);
                break;
            default: pgassert(false);
        }
        if (orders_size() == 1 && !is_feasable()) {
            pgassert(false);
        }

        if (!is_feasable()) {
            erase(order);
        } else {
            assigned += order.idx();
            unassigned -= order.idx();
            if (kind == 5) {
                current_feasable = m_orders[order.idx()].subsetJ(
                        current_feasable);
            }
            if (kind == 6) {
                current_feasable = m_orders[order.idx()].subsetI(
                        current_feasable);
            }
        }

        current_feasable -= order.idx();
        invariant();
    }

    pgassert(is_feasable());
    invariant();
    EXITING();
}


void
Vehicle_pickDeliver::erase(const Order &order) {
    ENTERING();
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
    ENTERING();
    invariant();
    pgassert(!empty());

    auto pick_itr = m_path.rbegin();
    while (pick_itr != m_path.rend() &&  !pick_itr->is_pickup()) {
        ++pick_itr;
    }

    pgassert(pick_itr->is_pickup());

    size_t deleted_pick_idx = pick_itr->idx();

    for (const auto o : m_orders) {
        if (o.pickup().idx() == deleted_pick_idx) {
            erase(o);
            invariant();
            EXITING();
            return o.idx();
        }
    }
    pgassert(false);
    EXITING();
    return 0;
}



size_t
Vehicle_pickDeliver::pop_front() {
    ENTERING();
    invariant();
    pgassert(!empty());

    auto pick_itr = m_path.begin();
    while (pick_itr != m_path.end() &&  !pick_itr->is_pickup()) {
        ++pick_itr;
    }

    pgassert(pick_itr->is_pickup());

    size_t deleted_pick_idx = pick_itr->idx();

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
    ENTERING();
    m_orders = orders;
    for (const auto o : orders) {
        if (is_order_feasable(o)) m_feasable_orders += o.idx();
    }
    m_orders.set_compatibles(m_speed);
    EXITING();
}

bool
Vehicle_pickDeliver::is_order_feasable(const Order &order) const {
    ENTERING();
    auto test_truck =  *this;
    test_truck.push_back(order);
    msg.log << test_truck;
    pgassertwm(false, msg.log.str().c_str());
    EXITING();
    return test_truck.is_feasable();
}

std::ostream& operator << (std::ostream &log, const Vehicle_pickDeliver &v) {
    log << static_cast<const Vehicle&>(v);
    log << "feasable_orders " << v.m_feasable_orders;
    return log;
}


}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting


