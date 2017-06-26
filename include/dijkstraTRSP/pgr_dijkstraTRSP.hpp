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

#include <sstream>
#include <deque>
#include <vector>
#include <set>
#include <limits>

#include "cpp_common/pgr_assert.h"
#include "cpp_common/basePath_SSEC.hpp"

#include "c_types/restrict_t.h"

template < class G >
class Pgr_dijkstraTRSP {
 public:
     Path dijkstraTRSP(
             G& graph,
             const std::vector<Restrict_t>& restrictions,
             int64_t source,
             int64_t target,
             bool only_cost,
             bool strict);
     void clear();
 private:
     void executeDijkstraTRSP(G& graph);
     void getFirstSolution(G& graph);
     bool checkFirstSolution();
 private:
     typedef typename G::V V;
     V v_source;
     V v_target;
     int64_t m_start;
     int64_t m_end;
     std::vector< std::pair<int64_t, Restrict_t> > m_restrictions;
     bool m_only_cost;
     bool m_strict;

     Path curr_result_path;

 public:
     std::ostringstream log;
};

template < class G >
void Pgr_dijkstraTRSP< G >::clear() {
}

template < class G>
Path
Pgr_dijkstraTRSP< G >::dijkstraTRSP(G& graph, const std::vector<Restrict_t>& restrictions,
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
    for(auto &restriction: restrictions)
        m_restrictions.push_back( {restriction.restricted_edges[0], restriction} );
    m_strict = strict;
    executeDijkstraTRSP(graph);
    return curr_result_path;
}

template < class G >
void Pgr_dijkstraTRSP< G >::getFirstSolution(G& graph) {
     Path path;

     Pgr_dijkstra< G > fn_dijkstra;
     path = fn_dijkstra.dijkstra(graph, m_start, m_end);

     if (path.empty()) return;
     curr_result_path = path;
}

template < class G >
bool Pgr_dijkstraTRSP< G >::checkFirstSolution() {
#if 0
    auto sort_cmp = [](const std::pair<int64_t, Restrict_t>& left,
       const std::pair<int64_t, Restrict_t>& right) -> bool {
           return left.first <= right.first;
       };
    auto lower_bound_cmp = [](const std::pair<int64_t, Restrict_t>& p,
        int64_t target) -> bool {
        return p.first < target;
    };
    std::stable_sort(m_restrictions.begin(), m_restrictions.end(),
        sort_cmp);
    std::vector< int64_t > edges_in_path;
    for (auto &path: curr_result_path) edges_in_path.push_back(path.edge);
    if (edges_in_path.size() and edges_in_path[0] == -1) reverse(edges_in_path.begin(), edges_in_path.end());
    while (edges_in_path.size() and edges_in_path.back() == -1) edges_in_path.pop_back();
    size_t index = 0;
    for (auto &edge: edges_in_path) {
        auto edge_index = std::lower_bound(m_restrictions.begin(),
            m_restrictions.end(), edge, lower_bound_cmp) - m_restrictions.begin();
        while (edge_index < m_restrictions.size() and
            m_restrictions[edge_index].first == edge) {
            size_t temp_edge_index = index;
            bool okay = true;
            for (auto &edge_id: m_restrictions[edge_index].second.restricted_edges) {
                if (edge_id == -1) break;
                if(edges_in_path[temp_edge_index] != edge_id) okay = false;
                temp_edge_index++;
            }
            if (okay) {
                log << "Restriction is not applicable to this case.";
                return false;
            }
            edge_index++;
        }
        index++;
    }
#endif
    log << "Restriction is applicable to this case.";
    return false;
}

template < class G >
void Pgr_dijkstraTRSP< G >::executeDijkstraTRSP(G& graph) {
    clear();
    getFirstSolution(graph);
    log << curr_result_path;
    bool sol = checkFirstSolution();
    if (sol)
        curr_result_path = Path();
}

#endif  // INCLUDE_DIJKSTRATRSP_PGR_DIJKSTRATRSP_HPP_
