/*PGR-GNU*****************************************************************
File: basePath_SSEC.cpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

********************************************************************PGR-GNU*/

#include "./basePath_SSEC.hpp"

#include <deque>
#include <iostream>
#include <algorithm>
#include <utility>

#include "./pgr_types.h"
#include "./pgr_assert.h"

void Path::push_front(Path_t data) {
    path.push_front(data);
    m_tot_cost += data.cost;
}

void Path::push_back(Path_t data) {
    path.push_back(data);
    m_tot_cost += data.cost;
}

void Path::reverse() {
    std::swap(m_start_id, m_end_id);
    if (path.size() <= 1) return;
    std::deque< Path_t > newpath;
    for (size_t i = 0; i < path.size(); ++i) {
        newpath.push_front({
                path[i].node,
                (i == 0? -1 : path[i - 1].edge),
                (i == 0? 0 : path[i - 1].cost),
                0
                });
    }
    for (size_t i = 0; i < newpath.size(); ++i) {
        newpath[i].agg_cost = (i == 0)?
            0 :
            newpath[i - 1].agg_cost +  newpath[i - 1].cost;
    }
    path = newpath;
}


void Path::clear() {
    path.clear();
    m_tot_cost = 0;
    m_start_id = 0;
    m_end_id = 0;
}

std::ostream& operator<<(std::ostream &log, const Path &path) {
    log << "Path: " << path.start_id() << " -> " << path.end_id() << "\n"
        << "seq\tnode\tedge\tcost\tagg_cost\n";
    int64_t i = 0;
    for (const auto &e : path) {
        log << i << "\t"
            << e.node << "\t"
            << e.edge << "\t"
            << e.cost << "\t"
            << e.agg_cost << "\n";
        ++i;
    }
    return log;
}



Path Path::getSubpath(unsigned int j) const {
    Path result(start_id(), end_id());
    if (j == 0)  return result;
    for (auto i = path.begin(); i != path.begin() + j; ++i) {
        result.push_back((*i));
    }
    pgassert(result.tot_cost() != 0);
    pgassert(this->tot_cost() != 0);
    return result;
}


bool Path::isEqual(const Path &subpath) const {
    if (subpath.empty()) return true;
    if (subpath.size() >= path.size()) return false;
    std::deque< Path_t >::const_iterator i, j;
    for (i = path.begin(),  j = subpath.begin();
            j != subpath.end();
            ++i, ++j)
        if ((*i).node != (*j).node) return false;
    return true;
}

void Path::appendPath(const Path &o_path) {
    path.insert(path.end(), o_path.path.begin(), o_path.path.end());
    m_tot_cost +=  o_path.m_tot_cost;
}


/*!
 
    Path: 2 -> 9
    seq   node    edge    cost    agg_cost
    0     2       4       1       0
    1     5       8       1       1
    2     6       9       1       2
    3     9       -1      0       3
    Path: 9 -> 3
    seq   node    edge    cost    agg_cost
    0     9       16      1       0
    1     4       3       1       1
    2     3       -1      0       2
    Path: 2 -> 3
    seq   node    edge    cost    agg_cost
    0     2       4       1       0
    1     5       8       1       1
    2     6       9       1       2
    3     9       16      1       3
    4     4       3       1       4
    5     3       -1      0       5

 */
void Path::append(const Path &other) {
    pgassert(m_end_id == other.m_start_id);
    if (other.m_start_id == other.m_end_id) {
        pgassert(other.path.empty());
        return;
    }
    if (m_start_id == m_end_id) {
        pgassert(path.empty());
        *this = other;
        return;
    }
    pgassert(path.back().cost == 0);
    pgassert(path.back().edge == -1);

    m_end_id = other.m_end_id;

    auto last = path.back();
    auto agg_cost = last.agg_cost;

    path.pop_back();

    for (auto item : other.path) {
        item.agg_cost += agg_cost;
        push_back(item);
    }
}


void Path::generate_postgres_data(
        General_path_element_t **postgres_data,
        size_t &sequence) const {
    int i = 1;
    for (const auto e : path) {
        (*postgres_data)[sequence] =
        {i, start_id(), end_id(), e.node, e.edge, e.cost, e.agg_cost};
        ++i;
        ++sequence;
    }
}

/* used by driving distance */
void Path::get_pg_dd_path(
        General_path_element_t **ret_path,
        size_t &sequence) const {
    for (unsigned int i = 0; i < path.size(); i++) {
        (*ret_path)[sequence].seq = i;
        (*ret_path)[sequence].start_id = start_id();
        (*ret_path)[sequence].end_id = start_id();
        (*ret_path)[sequence].node = path[i].node;
        (*ret_path)[sequence].edge = path[i].edge;
        (*ret_path)[sequence].cost = path[i].cost;
        (*ret_path)[sequence].agg_cost = path[i].agg_cost;
        sequence++;
    }
}

/* used by ksp */
void Path::get_pg_ksp_path(
        General_path_element_t **ret_path,
        size_t &sequence, int routeId) const {
    for (unsigned int i = 0; i < path.size(); i++) {
        (*ret_path)[sequence].seq = i + 1;
        (*ret_path)[sequence].start_id = routeId;
        (*ret_path)[sequence].end_id = end_id();
        (*ret_path)[sequence].node = path[i].node;
        (*ret_path)[sequence].edge = path[i].edge;
        (*ret_path)[sequence].cost = path[i].cost;
        (*ret_path)[sequence].agg_cost = (i == 0)?
            0 :
            (*ret_path)[sequence-1].agg_cost +  path[i-1].cost;
        sequence++;
    }
}
