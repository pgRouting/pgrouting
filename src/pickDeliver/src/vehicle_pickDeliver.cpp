/*PGR-GNU*****************************************************************

FILE: vehicle_pickDeliver.cpp

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
#include <iostream>
#include <deque>
#include <set>
#include <string>
#include <sstream>
#include <limits>


#include "./../../common/src/pgr_assert.h"
#include "./order.h"
#include "./vehicle.h"
#include "./vehicle_pickDeliver.h"
#include "./pgr_pickDeliver.h"



namespace pgrouting {
namespace vrp {

Order
Vehicle_pickDeliver::get_worse_order(
        std::set<size_t> orders) const {
    invariant();
    pgassert(!empty());

    // auto orders(of_this_subset);
    auto worse_order(problem->orders()[*orders.begin()]);
    auto delta_duration((std::numeric_limits<double>::max)());
    auto curr_duration(duration());
    while (!orders.empty()) {
        auto truck(*this);
        auto order(problem->orders()[*orders.begin()]);
        pgassert(truck.has_order(order));
        orders.erase(orders.begin());
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
    return problem->order_of(m_path[1]);
}


Vehicle_pickDeliver::Vehicle_pickDeliver(
        size_t id,
        const Vehicle_node &starting_site,
        const Vehicle_node &ending_site,
        double max_capacity,
        const Pgr_pickDeliver *p_problem) :
    Vehicle(id, starting_site, ending_site, max_capacity),
    cost((std::numeric_limits<double>::max)()),
    problem(p_problem) {
        orders_in_vehicle.clear();

        invariant();
    }



bool
Vehicle_pickDeliver::has_order(const Order &order) const {
    return !(orders_in_vehicle.find(order.id()) == orders_in_vehicle.end());
}


void
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
        /* pickup generates twv evrywhere,
         *  so put the order as last */
        push_back(order);
        return;
    }

    if (deliver_pos.second < deliver_pos.first) {
        /* delivery generates twv evrywhere,
         *  so put the order as last */
        push_back(order);
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
            orders_in_vehicle.insert(order.id());
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
        orders_in_vehicle.erase(order.id());
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
        return;
    }
    Vehicle::insert(best_pick_pos, order.pickup());
    Vehicle::insert(best_deliver_pos, order.delivery());

    orders_in_vehicle.insert(order.id());
    pgassertwm(is_feasable(), err_log.str());
    pgassertwm(has_order(order), err_log.str());
    pgassertwm(!has_cv(), err_log.str());
    invariant();
}

void
Vehicle_pickDeliver::push_back(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    orders_in_vehicle.insert(order.id());
    m_path.insert(m_path.end() - 1, order.pickup());
    m_path.insert(m_path.end() - 1, order.delivery());
    evaluate(m_path.size() - 3);

    pgassert(has_order(order));
    pgassert(!has_cv());
    invariant();
}

void
Vehicle_pickDeliver::push_front(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    orders_in_vehicle.insert(order.id());
    m_path.insert(m_path.begin() + 1, order.delivery());
    m_path.insert(m_path.begin() + 1, order.pickup());
    evaluate(1);

    pgassert(has_order(order));
    pgassert(!has_cv());
    invariant();
}


void
Vehicle_pickDeliver::erase(const Order &order) {
    invariant();
    pgassert(has_order(order));


    Vehicle::erase(order.pickup());
    Vehicle::erase(order.delivery());
    orders_in_vehicle.erase(orders_in_vehicle.find(order.id()));

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

    ID deleted_pick_id = pick_itr->id();


    auto delivery_id = problem->node(deleted_pick_id).Did();

    m_path.erase((pick_itr + 1).base());

    auto delivery_itr = m_path.rbegin();
    while (delivery_itr != m_path.rend()
            && !(delivery_itr->id() ==delivery_id)) {
        ++delivery_itr;
    }

    pgassert(delivery_itr->is_delivery());
    pgassert(delivery_itr->Pid() == deleted_pick_id);

    m_path.erase((delivery_itr + 1).base());


    /* figure out from where the evaluation is needed */
    evaluate(1);

    ID deleted_order_id(
            problem->order_of(problem->node(deleted_pick_id)).id());

    orders_in_vehicle.erase(orders_in_vehicle.find(deleted_order_id));

    invariant();
    return deleted_order_id;
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

    ID deleted_pick_id = pick_itr->id();


    auto delivery_id = problem->node(deleted_pick_id).Did();

    m_path.erase(pick_itr);

    auto delivery_itr = m_path.begin();
    while (delivery_itr != m_path.end()
            && !(delivery_itr->id() == delivery_id)) {
        ++delivery_itr;
    }

    pgassert(delivery_itr->is_delivery());
    pgassert(delivery_itr->Pid() == deleted_pick_id);

    m_path.erase(delivery_itr);

    evaluate(1);

    ID deleted_order_id(
            problem->order_of(problem->node(deleted_pick_id)).id());

    orders_in_vehicle.erase(orders_in_vehicle.find(deleted_order_id));

    invariant();
    return deleted_order_id;
}


}  //  namespace vrp
}  //  namespace pgrouting


