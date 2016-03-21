
#include <deque>
#include <iostream>
#include <algorithm>
#include "../../common/src/pgr_assert.h"


#include "./vehicle.h"



void
Vehicle::invariant() const{
    assert(m_path.size() >= 2);
    assert(m_path.front().is_start());
    assert(m_path.back().is_end());
}

void
Vehicle::insert(POS at, Vehicle_node node) {
    invariant();
    assert(at <= m_path.size());

    m_path.insert(m_path.begin() + at, node); 
    evaluate(at);

    assert(at < m_path.size());
    assert(m_path[at].id() == node.id());
    invariant();

}

/*
 * before: S E
 * after:  S N E
 *
 * before: S n1 n2 ... n E
 * after:  S n1 n2 ... n N E
 */
void
Vehicle::push_back(Vehicle_node node) {
    invariant();

    insert(m_path.size() - 1, node);

    invariant();
}


void
Vehicle::erase(POS at) {
    invariant();

    assert(m_path.size() > 2);
    assert(at < m_path.size());
    assert(!m_path[at].is_start());
    assert(!m_path[at].is_end());

    m_path.erase(m_path.begin() + at);
    evaluate(at);

    invariant();
}

void
Vehicle::swap(POS i, POS j) {
    invariant();
    assert(m_path.size() > 3);
    assert(!m_path[i].is_start());
    assert(!m_path[i].is_end());
    assert(!m_path[j].is_start());
    assert(!m_path[j].is_end());

    std::swap(m_path[i], m_path[j]);
    i < j ? evaluate(i) : evaluate(j);

    invariant();
}


void
Vehicle::evaluate() {
    // preconditions
    assert(m_path.size() >= 2);
    assert(m_path.front().is_start());
    assert(m_path.back().is_end());

    evaluate(0);
}

void
Vehicle::evaluate(POS from) {
    // preconditions
    assert(from < m_path.size());
    assert(m_path.size() >= 2);
    assert(m_path.front().is_start());
    assert(m_path.back().is_end());

    
    auto node = m_path.begin() + from;

    while (node != m_path.end()) {
        if (node == m_path.begin()) node->evaluate(max_capacity);
        else node->evaluate(*(node - 1), max_capacity);

        ++node;
    }
    
}

std::deque< Vehicle_node > 
Vehicle::path() const {
    return m_path;
}


Vehicle::Vehicle(
        const Vehicle_node &starting_site, 
        const Vehicle_node &ending_site, 
        double p_max_capacity) :
    max_capacity(p_max_capacity) { 
        m_path.clear();
        m_path.push_back(starting_site);
        m_path.push_back(ending_site);
        evaluate(0);
    }


/****** FRIENDS *******/

std::ostream&
operator<<(std::ostream &log, const Vehicle &v){
    for (const auto &path_stop : v.path()) {
        log << path_stop;
    }
    return log;
}

bool
operator<(const Vehicle &lhs, const Vehicle &rhs){
    lhs.invariant();
    rhs.invariant();

    if (lhs.m_path.size() < rhs.m_path.size()) return true;

    /* here because sizes are equal */

    if (lhs.m_path.back().total_travel_time()
            < lhs.m_path.back().total_travel_time()) return true;

    return false;
}
