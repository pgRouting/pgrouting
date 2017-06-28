/*PGR-GNU*****************************************************************
File: pgr_dijkstraTRSP.hpp
Copyright (c) 2017 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com
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
#ifndef INCLUDE_DIJKSTRATRSP_PGR_DIJKSTRATRSP_HPP_
#define INCLUDE_DIJKSTRATRSP_PGR_DIJKSTRATRSP_HPP_
#pragma once

#include "dijkstra/pgr_dijkstra.hpp"
#include "dijkstraTRSP/restriction.h"

#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <limits>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/basePath_SSEC.hpp"

template < class G >
class Pgr_dijkstraTRSP {
 public:
     Path dijkstraTRSP(
             G& graph,
             const std::vector< Restriction >& restrictions,
             int64_t source,
             int64_t target,
             bool only_cost,
             bool strict);
     void init();
 private:
     void executeDijkstraTRSP(G& graph);
     void getDijkstraSolution(G& graph);
     bool has_restriction();
     bool has_a_restriction(int64_t edge, int64_t index);
 private:
     typedef typename G::V V;
     V v_source;
     V v_target;
     int64_t m_start;
     int64_t m_end;
     std::vector< Restriction > m_restrictions;
     std::vector< int64_t > m_edges_in_path;
     bool m_only_cost;
     bool m_strict;

     Path curr_result_path;

 public:
     std::ostringstream log;
};

template < class G >
void Pgr_dijkstraTRSP< G >::init() {
}

template < class G>
Path
Pgr_dijkstraTRSP< G >::dijkstraTRSP(G& graph, const std::vector< Restriction >& restrictions,
int64_t start_vertex, int64_t end_vertex, bool only_cost, bool strict) {
    if (start_vertex == end_vertex)
        return Path();
    if (!graph.has_vertex(start_vertex) || !graph.has_vertex(end_vertex))
        return Path();

    m_only_cost = only_cost;
    v_source = graph.get_V(start_vertex);
    v_target = graph.get_V(end_vertex);
    m_start = start_vertex;
    m_end = end_vertex;
    m_restrictions = restrictions;
    m_strict = strict;
    executeDijkstraTRSP(graph);
    return curr_result_path;
}

template < class G >
void Pgr_dijkstraTRSP< G >::getDijkstraSolution(G& graph) {
     Path path;

     Pgr_dijkstra< G > fn_dijkstra;
     path = fn_dijkstra.dijkstra(graph, m_start, m_end);

     if (path.empty()) return;
     curr_result_path = path;
}

template < class G >
bool Pgr_dijkstraTRSP< G >::has_a_restriction(int64_t edge, int64_t index) {
    auto lower_bound_cmp = [](const Restriction& r, const int64_t& target) -> bool {
        return r.restricted_edges()[0] < target;
    };
    auto edge_index = std::lower_bound(m_restrictions.begin(),
        m_restrictions.end(), edge, lower_bound_cmp) - m_restrictions.begin();
    while (edge_index < m_restrictions.size()) {
        auto r_edges = m_restrictions[edge_index].restricted_edges();
        if (r_edges[0] != edge) break;

        bool okay = true;
        size_t temp_edge_index = index;

        for (auto &edge_id: r_edges) {
            if (temp_edge_index >= m_edges_in_path.size() or
                m_edges_in_path[temp_edge_index] != edge_id) {
                okay = false;
                break;
            }
            temp_edge_index++;
        }
        if (okay) return true;
        edge_index++;
    }
}

template < class G >
bool Pgr_dijkstraTRSP< G >::has_restriction() {
    auto sort_cmp = [](const Restriction& left,
         const Restriction& right) -> bool {
           return left.restricted_edges()[0] <= right.restricted_edges()[0];
       };
    std::stable_sort(m_restrictions.begin(), m_restrictions.end(),
        sort_cmp);

    size_t index = 0;
    for (auto &edge: m_edges_in_path) {
        if (has_a_restriction(edge, index))
            return true;
        index++;
    }
    return false;
}

template < class G >
void Pgr_dijkstraTRSP< G >::executeDijkstraTRSP(G& graph) {
    init();
    getDijkstraSolution(graph);
    log << curr_result_path;

    for (auto &path: curr_result_path) {
        m_edges_in_path.push_back(path.edge);
    }
    while (m_edges_in_path.size() and m_edges_in_path.back() == -1) {
        m_edges_in_path.pop_back();
    }

    bool sol = has_restriction();
    log << "Result of valid solution" << sol << "\n";
    if (sol)
        curr_result_path = Path();
}

#endif  // INCLUDE_DIJKSTRATRSP_PGR_DIJKSTRATRSP_HPP_
