/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Maoguang Wang 
Mail: xjtumg1007@gmail.com

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

#ifndef INCLUDE_CHPP_PGR_DIRECTEDCHPP_HPP_
#define INCLUDE_CHPP_PGR_DIRECTEDCHPP_HPP_
#pragma once

#include "costFlow/pgr_minCostMaxFlow.hpp"
#include "c_types/general_path_element_t.h"
#include "c_types/pgr_edge_t.h"

namespace pgrouting {
namespace graph {

class PgrDirectedChPPGraph {
 public:
     PgrDirectedChPPGraph(
             const pgr_edge_t *dataEdges,
             const size_t totalEdges);

     double DirectedChPP() {
         double minAddedCost = flowGraph.MinCostMaxFlow();
         int64_t maxFlow = flowGraph.GetMaxFlow();
         if (maxFlow == totalDeg)
             return minAddedCost + totalCost;
         return -1.0;
     }

     std::vector<General_path_element_t> GetPathEdges();

 private:
     int64_t totalDeg;
     double totalCost;
     int64_t superSource, superTarget;

     graph::PgrCostFlowGraph flowGraph;
     std::vector<pgr_edge_t> resultEdges;
};

PgrDirectedChPPGraph::PgrDirectedChPPGraph(
        const pgr_edge_t *dataEdges,
        const size_t totalEdges) {
    resultEdges.clear();
    for (size_t i = 0; i < totalEdges; i++) {
        pgr_edge_t edge;
        edge.id = dataEdges[i].id;
        edge.source = dataEdges[i].source;
        edge.target = dataEdges[i].target;
        edge.reverse_cost = -1;
        if (dataEdges[i].cost > 0) {
            edge.cost = dataEdges[i].cost;
            totalCost += edge.cost;
            resultEdges.push_back(edge);
        }
        if (dataEdges[i].reverse_cost > 0) {
            std::swap(edge.source, edge.target);
            edge.cost = dataEdges[i].reverse_cost;
            totalCost += edge.cost;
            resultEdges.push_back(edge);
        }
    }

    std::vector<pgr_costFlow_t> edges;
    std::set<int64_t> sources;
    std::set<int64_t> targets;

    // calcu deg & build part of edges
    std::map<int64_t, int> deg;
    for (size_t i = 0; i < resultEdges.size(); i++) {
        deg[resultEdges[i].source]++;
        deg[resultEdges[i].target]--;

        pgr_costFlow_t edge;
        edge.edge_id = resultEdges[i].id;
        edge.reverse_capacity = -1;
        edge.reverse_cost = -1.0;
        edge.source = resultEdges[i].source;
        edge.target = resultEdges[i].target;
        edge.capacity = (std::numeric_limits<int32_t>::max)();
        edge.cost = resultEdges[i].cost;
        edges.push_back(edge);
    }

    // find source & target
    superSource = superTarget = -1;
    int64_t iPointId = 1;
    while (superSource == -1 || superTarget == -1) {
        if (deg.find(iPointId) == deg.end()) {
            if (superSource == -1)
                superSource = iPointId;
            else
                superTarget = iPointId;
        }
        iPointId++;
    }
    sources.insert(superSource);
    targets.insert(superTarget);

    // build full edges
    std::map<int64_t, int>::iterator iter;
    totalDeg = 0;
    for (iter = deg.begin(); iter != deg.end(); iter++) {
        int64_t p = iter->first;
        int d = iter->second;
        if (d == 0)
            continue;
        if (d > 0)
            totalDeg += d;
        pgr_costFlow_t edge;
        edge.reverse_capacity = -1;
        edge.reverse_cost = -1.0;
        edge.cost = 0.0;
        edge.capacity = abs(d);
        edge.edge_id = 0;
        if (d > 0)
            edge.source = p, edge.target = superTarget;
        if (d < 0)
            edge.source = superSource, edge.target = p;
        edges.push_back(edge);
    }
    
    PgrCostFlowGraph graph(edges, sources, targets);
    flowGraph = graph;
}

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_CHPP_PGR_DIRECTEDCHPP_HPP_
