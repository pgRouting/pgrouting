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
#include <sstream>
#include <limits>


#include "./../../common/src/pgr_assert.h"
#include "./order.h"
#include "./vehicle.h"
#include "./vehicle_pickDeliver.h"
#include "./pgr_pickDeliver.h"



namespace pgRouting {
namespace vrp {


Vehicle_pickDeliver::Vehicle_pickDeliver(
        size_t id,
        const Vehicle_node &starting_site,
        const Vehicle_node &ending_site,
        double max_capacity,
        const Pgr_pickDeliver *p_problem) :
    Vehicle(id, starting_site, ending_site, max_capacity),
    cost(std::numeric_limits<double>::max()),
    problem(p_problem) {
        orders_in_vehicle.clear();

    invariant();
    }



bool
Vehicle_pickDeliver::has_order(const Order &order) const {
    return !(orders_in_vehicle.find(order.id()) == orders_in_vehicle.end());
}

/************ INSERTING ORDERS **************/

#if 0
void
Vehicle_pickDeliver::insert_less_travel_time(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    auto pick_pos = Vehicle::insert_less_travel_time(order.pickup());


    problem->log << "\n ----------------------------------after inserting pickup";
    problem->log << (*this);

    Vehicle::insert_less_travel_time(order.delivery(), pick_pos + 1);

    problem->log << "\n ----------------------------------after inserting delivery";
    problem->log << (*this);


    orders_in_vehicle.insert(order.id());
    pgassert(has_order(order));
    pgassert(!has_cv());
    invariant();
}
#endif



void
Vehicle_pickDeliver::insert(const Order &order) {
    invariant();
    pgassert(!has_order(order));

    auto pick_pos(position_limits(order.pickup()));

#if 0
    problem->log << "\n\n\n\n/////////////////////BEFORE";
    if (id() == 2) {
        problem->log << (*this);
    } else {
        problem->log << tau();
    }
    problem->log << "\n(pickup, delivery) ="
        << order.pickup().original_id() << ", "
        << order.delivery().original_id() << ")";
#endif

    if (pick_pos.first > pick_pos.second) {
#if 0
        problem->log << "\ninsert by push_back";
#endif
        push_back(order);

#if 0
        problem->log << "\n/////////////////////AFTER";
        if (id() == 2) {
            problem->log << (*this);
        } else {
            problem->log << tau();
        }
#endif
        return;
    }


#if 0
    problem->log << "\n\tpickup limits (low, high) = ("
        << pick_pos.first << ", "
        << pick_pos.second << ") ";
#endif

    // Vehicle::insert(pick_pos.first, order.pickup());
    Vehicle::insert(pick_pos, order.pickup());

    auto deliver_pos(position_limits(order.delivery()));

#if 0
    problem->log << "\n\tdeliver limits (low, high) = ("
        << deliver_pos.first << ", "
        << deliver_pos.second << ") ";
#endif
    pgassert(pick_pos.first < deliver_pos.second);


    Vehicle::insert(deliver_pos, order.delivery());


#if 0
    problem->log << "\n/////////////////////AFTER";
    if (id() == 2) {
        problem->log << (*this);
    } else {
        problem->log << tau();
    }
#endif


    orders_in_vehicle.insert(order.id());
    pgassert(has_order(order));
    pgassert(!has_cv());
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
    while (delivery_itr != m_path.rend() &&  !(delivery_itr->id() ==delivery_id)) {
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
    while (delivery_itr != m_path.end() &&  !(delivery_itr->id() ==delivery_id)) {
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
}  //  namespace pgRouting


