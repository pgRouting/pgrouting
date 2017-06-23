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
             G &graph,
             std::vector<Restrict_t>& restrictions,
             int64_t source,
             int64_t target,
             bool only_cost,
             bool strict);
     void clear();
 private:
     void executeDijkstraTRSP(G &graph);
     void getFirstSolution(G &graph);
     void doNextCycle(G &graph);
 private:
     typedef typename G::V V;
     V v_source;
     V v_target;
     int64_t m_start;
     int64_t m_end;

     bool m_only_cost;

     Path curr_result_path;
     std::ostringstream log;
};

template < class G >
void Pgr_dijkstraTRSP< G >::clear() {
}

template < class G>
Path
Pgr_dijkstraTRSP< G >::dijkstraTRSP(G &graph, std::vector<Restrict_t>& restrictions,
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
    Path l_ResultList;

    executeDijkstraTRSP(graph);
    return l_ResultList;
}

template < class G >
void Pgr_dijkstraTRSP< G >::getFirstSolution(G &graph) {
     Path path;

     Pgr_dijkstra< G > fn_dijkstra;
     path = fn_dijkstra.dijkstra(graph, m_start, m_end);

     if (path.empty()) return;
     curr_result_path = path;
}

template < class G >
void Pgr_dijkstraTRSP< G >::doNextCycle(G &graph) {
    int64_t spurNodeId;


    for (unsigned int i = 0; i < curr_result_path.size(); ++i) {
        spurNodeId = curr_result_path[i].node;

        auto rootPath = curr_result_path.getSubpath(i);

        Pgr_dijkstra< G > fn_dijkstra;
        auto spurPath = fn_dijkstra.dijkstra(graph, spurNodeId, m_end);

        if (spurPath.size() > 0) {
            rootPath.appendPath(spurPath);

        }

        graph.restore_graph();
    }
}

template < class G >
void Pgr_dijkstraTRSP< G >::executeDijkstraTRSP(G &graph) {
    clear();
    getFirstSolution(graph);
}

#endif  // INCLUDE_DIJKSTRATRSP_PGR_DIJKSTRATRSP_HPP_
