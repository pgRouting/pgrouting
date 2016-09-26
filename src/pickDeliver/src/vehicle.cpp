/*PGR-GNU*****************************************************************

FILE: vehicle.cpp

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

#include <deque>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "../../common/src/pgr_assert.h"


#include "./pgr_pickDeliver.h"
#include "./vehicle.h"


namespace pgrouting {
namespace vrp {


void
Vehicle::invariant() const {
    pgassert(m_path.size() >= 2);
    pgassert(m_path.front().is_start());
    pgassert(m_path.back().is_end());
}

size_t
Vehicle::insert(std::pair<POS, POS> position_limits, const Vehicle_node &node) {
    invariant();
    pgassert(position_limits.first <= m_path.size());
    pgassert(position_limits.second <= m_path.size());

    auto low = position_limits.first;
    auto high = position_limits.second;
    auto best = low;


    insert(low, node);

    Vehicle::Cost best_cost(cost());


    while (low < high) {
        swap(low, low + 1);
        ++low;
        if (cost_compare(best_cost, cost())) {
            best_cost = cost();
            best = low;
        }
    }
    return best;

    pgassert(best < m_path.size());
    pgassert(m_path[best].id() == node.id());
    invariant();
}




bool
Vehicle::cost_compare(const Cost &lhs, const Cost &rhs) const {
    /*
     * capacity violations
     */
    if (std::get<1>(lhs) < std::get<1>(rhs))
        return true;
    if (std::get<1>(lhs) > std::get<1>(rhs))
        return false;

    /*
     * time window violations
     */
    if (std::get<0>(lhs) < std::get<0>(rhs))
        return true;
    if (std::get<0>(lhs) > std::get<0>(rhs))
        return false;

    /*
     * waiting time
     */
    if (std::get<3>(lhs) < std::get<3>(rhs))
        return true;
    if (std::get<3>(lhs) > std::get<3>(rhs))
        return false;

    /*
     * duration
     */
    if (std::get<4>(lhs) < std::get<4>(rhs))
        return true;
    if (std::get<4>(lhs) > std::get<4>(rhs))
        return false;

    /*
     * truck size
     */
    if (std::get<2>(lhs) < std::get<2>(rhs))
        return true;
    if (std::get<2>(lhs) > std::get<2>(rhs))
        return false;

    return false;
}



void
Vehicle::get_postgres_result(
        int vid,
        std::vector< General_vehicle_orders_t > &result) const {
    /* postgres numbering starts with 1 */
    int i(1);
    for (const auto p_stop : m_path) {
        General_vehicle_orders_t data =
                {vid, i,
                p_stop.original_id(),
                p_stop.travel_time(),
                p_stop.arrival_time(),
                p_stop.wait_time(),
                p_stop.service_time(),
                p_stop.departure_time()};
        result.push_back(data);
        ++i;
    }
}


Vehicle::Cost
Vehicle::cost() const {
    return std::make_tuple(
            twvTot(), cvTot(), m_path.size(),
            total_wait_time(), duration());
}


void
Vehicle::insert(POS at, Vehicle_node node) {
    invariant();
    pgassert(at <= m_path.size());

    m_path.insert(m_path.begin() + at, node);
    evaluate(at);

    pgassert(at < m_path.size());
    pgassert(m_path[at].id() == node.id());
    invariant();
}


double
Vehicle::deltaTime(const Vehicle_node &node, POS pos) const {
    /*
     * .... POS POS+1 ....
     * .... POS node POS+1 ....
     *
     */
    auto prev = m_path[pos-1];
    auto next = m_path[pos];
    auto original_time = next.travel_time();
    auto tt_p_n = prev.travel_time_to(node);
    tt_p_n = node.is_early_arrival(prev.departure_time() + tt_p_n) ?
        node.closes() - prev.departure_time()
        : tt_p_n;

    auto tt_n_x = node.travel_time_to(next);
    tt_p_n = next.is_early_arrival(
            prev.departure_time() + tt_p_n + node.service_time() + tt_n_x) ?
        next.closes() - (prev.departure_time() + tt_p_n + node.service_time())
        : tt_n_x;

    return (tt_p_n + tt_n_x) - original_time;
}




size_t
Vehicle::insert_less_travel_time(const Vehicle_node &node, POS after_pos) {
    invariant();

    double min_delta = (std::numeric_limits<double>::max)();
    POS min_pos = after_pos;

    for (POS pos = after_pos; pos < m_path.size(); ++pos) {
        if (!m_path[pos].is_start()) {
            auto tt = deltaTime(node, pos);

            if (tt < min_delta) {
                min_delta = tt;
                min_pos = pos;
            }
        }
    }
    insert(min_pos, node);

    invariant();
    return min_pos;
}

void
Vehicle::erase(const Vehicle_node &node) {
    invariant();

    POS pos = 0;
    for ( ; pos < m_path.size() ; ++pos) {
        if (node.id() == m_path[pos].id())
            break;
    }

    erase(pos);
    evaluate(pos);

    invariant();
}


/*
 * before: S E
 * after: S N E
 *
 * before: S n1 n2 ... n E
 * after: S N n1 n2 ... n E
 */
void
Vehicle::push_front(const Vehicle_node &node) {
    invariant();

    /* insert evaluates */
    insert(1, node);

    invariant();
}

/*
 * before: S E
 * after: S N E
 *
 * before: S n1 n2 ... n E
 * after: S n1 n2 ... n N E
 */
void
Vehicle::push_back(const Vehicle_node &node) {
    invariant();

    /* insert evaluates */
    insert(m_path.size() - 1, node);

    invariant();
}

void
Vehicle::pop_back() {
    invariant();
    pgassert(m_path.size() > 2);

    /* erase evaluates */
    erase(m_path.size() - 2);

    invariant();
}

void
Vehicle::pop_front() {
    invariant();
    pgassert(m_path.size() > 2);

    /* erase evaluates */
    erase(1);

    invariant();
}



void
Vehicle::erase(POS at) {
    invariant();

    pgassert(m_path.size() > 2);
    pgassert(at < m_path.size());
    pgassert(!m_path[at].is_start());
    pgassert(!m_path[at].is_end());

    m_path.erase(m_path.begin() + at);
    evaluate(at);

    invariant();
}

void
Vehicle::swap(POS i, POS j) {
    invariant();
    pgassert(m_path.size() > 3);
    pgassert(!m_path[i].is_start());
    pgassert(!m_path[i].is_end());
    pgassert(!m_path[j].is_start());
    pgassert(!m_path[j].is_end());

    std::swap(m_path[i], m_path[j]);
    i < j ? evaluate(i) : evaluate(j);

    invariant();
}


void
Vehicle::evaluate() {
    invariant();

    evaluate(0);

    invariant();
}

bool
Vehicle::empty() const {
    invariant();
    return m_path.size() <= 2;
}

void
Vehicle::evaluate(POS from) {
    invariant();
    // preconditions
    pgassert(from < m_path.size());


    auto node = m_path.begin() + from;

    while (node != m_path.end()) {
        if (node == m_path.begin()) {
            node->evaluate(max_capacity);
        } else {
            node->evaluate(*(node - 1), max_capacity);
        }

        ++node;
    }
    invariant();
}

std::deque< Vehicle_node >
Vehicle::path() const {
    invariant();
    return m_path;
}


std::pair<size_t, size_t>
Vehicle::position_limits(const Vehicle_node node) const {
    POS high = getPosHighLimit(node);
    POS low = getPosLowLimit(node);
    return std::make_pair(low, high);
}


/*
 * start searching from postition low = pos(E)
 *
 * S 1 2 3 4 5 6 7 ..... E
 * node -> E
 * node -> ...
 * node -> 7
 * node -> 6
 * node -> 5
 * node /-> 4
 *
 * return low_limit = 5
 *
 */
size_t
Vehicle::getPosLowLimit(const Vehicle_node &nodeI) const {
    invariant();

    POS low = 0;
    POS high = m_path.size();
    POS low_limit = high;

    /* J == m_path[low_limit - 1] */
    while (low_limit > low
             && m_path[low_limit - 1].is_compatible_IJ(nodeI)) {
        --low_limit;
    }

    invariant();
    return low_limit;
}


/*
 * start searching from postition low = pos(S)
 *
 * S 1 2 3 4 5 6 7 ..... E
 * S -> node
 * 1 -> node
 * 2 -> node
 * ...
 * 6 -> node
 * 7 /-> node
 *
 * returns high_limit = 7
 */
size_t
Vehicle::getPosHighLimit(const Vehicle_node &nodeJ) const {
    invariant();

    POS low = 0;
    POS high = m_path.size();
    POS high_limit = low;

    /* I == m_path[high_limit] */
    while (high_limit < high
             && nodeJ.is_compatible_IJ(m_path[high_limit])) {
        ++high_limit;
    }

    invariant();
    return high_limit;
}



Vehicle::Vehicle(
        ID p_id,
        const Vehicle_node &starting_site,
        const Vehicle_node &ending_site,
        double p_max_capacity) :
    m_id(p_id),
    max_capacity(p_max_capacity) {
        m_path.clear();
        m_path.push_back(starting_site);
        m_path.push_back(ending_site);
        evaluate(0);
        invariant();
    }




std::string
Vehicle::tau() const {
    std::ostringstream log;
    log << "Truck " << id() << " (";
    for (const auto p_stop : m_path) {
        if (!(p_stop == m_path.front()))
            log << ", ";
        log << p_stop.original_id();
    }
    log << ")" << " \t(cv, twv, wait_time, duration) = ("
        << cvTot() << ", "
        << twvTot() << ", "
        << total_wait_time() << ", "
        << duration() << ")";

    return log.str();
}

/****** FRIENDS *******/

std::ostream&
operator << (std::ostream &log, const Vehicle &v) {
    v.invariant();
    int i(0);
    log << "\n\n****************** TRUCK " << v.id() << "***************";
    for (const auto &path_stop : v.path()) {
        log << "\nPath_stop" << ++i << "\n";
        log << path_stop;
    }
    return log;
}

bool
operator<(const Vehicle &lhs, const Vehicle &rhs) {
    lhs.invariant();
    rhs.invariant();

    if (lhs.m_path.size() < rhs.m_path.size()) return true;

    /* here because sizes are equal */

    if (lhs.m_path.back().total_travel_time()
            < lhs.m_path.back().total_travel_time()) return true;

    return false;
}

}  //  namespace vrp
}  //  namespace pgrouting

