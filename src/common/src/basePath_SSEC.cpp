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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include "basePath_SSEC.hpp"
#include <deque>
#include <iostream>
#include <algorithm>
#include "postgres.h"
#include "./pgr_types.h"


void Path::push_front(Path_t data) {
    path.push_front(data);
    m_tot_cost += data.cost;
}

void Path::push_back(Path_t data) {
    path.push_back(data);
    m_tot_cost += data.cost;
}

#if 0
Path_t Path::set_data(
        int64_t d_from, 
        int64_t d_to,
        int64_t d_vertex,
        int64_t d_edge, 
        float8 d_cost,
        float8 d_tot_cost) {
    Path_t data({d_vertex, d_edge, d_cost, d_tot_cost});
       m_start_id = d_from;
       m_end_id = d_to;
    return data;
}

void Path::push_front(
        int64_t d_vertex,
        int64_t d_edge, 
        float8 d_cost,
        float8 d_agg_cost) {
    path.push_front({d_vertex, d_edge, d_cost, d_tot_cost});
}
#endif


void Path::clear() {
    path.clear();
    m_tot_cost = 0;
    m_start_id = 0;
    m_end_id = 0;
}

void Path::print_path(std::ostream& log) const {
    log << "Path: " << start_id() << " -> " << end_id() << "\n"
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
}

#if 0
// for ksp
void Path::fix_path(int64_t p_from, int64_t p_to) {
    for (auto &row :  path) {
        row.from = p_to;
        row.to = p_from;
    }
}


void Path::print_path() const {
    print_path(std::cout);
}
#endif


Path Path::getSubpath(unsigned int j) const {
    Path result(start_id(), end_id());
    if (j == 0)  return result;
    for (auto i = path.begin(); i != path.begin() + j; ++i) {
        result.push_back((*i));
    }
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

#if 0
void Path::empty_path(unsigned int d_vertex) {
    path.push_back(set_data(1, d_vertex, d_vertex, d_vertex, -1, 0, 0));
}
#endif


void Path::generate_postgres_data(
        General_path_element_t **postgres_data,
        size_t &sequence) const{
    int64_t i = 1;
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
        int &sequence) const {

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
        int &sequence, int routeId) const {

    for (unsigned int i = 0; i < path.size(); i++) {
        (*ret_path)[sequence].seq = i + 1;
        (*ret_path)[sequence].start_id = (uint64_t)routeId;
        (*ret_path)[sequence].end_id = end_id();
        (*ret_path)[sequence].node = path[i].node;
        (*ret_path)[sequence].edge = path[i].edge;
        (*ret_path)[sequence].cost = path[i].cost;
        (*ret_path)[sequence].agg_cost = (i == 0)? 0: (*ret_path)[sequence-1].agg_cost +  path[i-1].cost;
        sequence++;
    }
}
