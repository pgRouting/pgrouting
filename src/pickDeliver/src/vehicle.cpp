
#include <deque>
#include <iostream>
#include <algorithm>
#include <cassert>


#include "./vehicle.h"



void Vehicle::invariant() const{
    assert(path.size() >= 2);
    assert(path.front().is_start());
    assert(path.back().is_end());
}

void Vehicle::insert(POS at, Vehicle_node node) {
    invariant();
    assert(at <= path.size());

    path.insert(path.begin() + at, node); 
    evaluate(at);

    assert(at < path.size());
    assert(path[at].id() == node.id());
    invariant();

}

/*
 * before: S E
 * after:  S N E
 *
 * before: S n1 n2 ... n E
 * after:  S n1 n2 ... n N E
 */
void Vehicle::push_back(Vehicle_node node) {
    invariant();

    insert(path.size() - 1, node);

    invariant();
}


void Vehicle::erase(POS at) {
    invariant();

    assert(path.size() > 2);
    assert(at < path.size());
    assert(!path[at].is_start());
    assert(!path[at].is_end());

    path.erase(path.begin() + at);
    evaluate(at);

    invariant();
}

void Vehicle::swap(POS i, POS j) {
    invariant();
    assert(path.size() > 3);
    assert(!path[i].is_start());
    assert(!path[i].is_end());
    assert(!path[j].is_start());
    assert(!path[j].is_end());

    std::swap(path[i], path[j]);
    i < j ? evaluate(i) : evaluate(j);

    invariant();
}


void Vehicle::evaluate() {
    // preconditions
    assert(path.size() >= 2);
    assert(path.front().is_start());
    assert(path.back().is_end());

    evaluate(0);
}

void Vehicle::evaluate(POS from) {
    // preconditions
    assert(from < path.size());
    assert(path.size() >= 2);
    assert(path.front().is_start());
    assert(path.back().is_end());

    auto node = path.begin() + from;

    while (node != path.end()) {
        if (node == path.begin()) node->evaluate(max_capacity);
        else node->evaluate(*(node - 1), max_capacity);

        ++node;
    }
}


bool operator<(const Vehicle &lhs, const Vehicle &rhs){
    lhs.invariant();
    rhs.invariant();

    if (lhs.path.size() < rhs.path.size()) return true;

    /* here because sizes are equal */

    if (lhs.path.back().total_travel_time()
            < lhs.path.back().total_travel_time()) return true;

    return false;
}

