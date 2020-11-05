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

#ifndef INCLUDE_CHINESE_PGR_CHINESEPOSTMAN_HPP_
#define INCLUDE_CHINESE_PGR_CHINESEPOSTMAN_HPP_
#pragma once

#include <map>
#include <vector>
#include <utility>
#include <set>
#include <limits>
#include <stack>

#include "max_flow/pgr_minCostMaxFlow.hpp"
#include "c_types/general_path_element_t.h"
#include "c_types/pgr_edge_t.h"
#include "c_types/pgr_flow_t.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/identifiers.hpp"


namespace pgrouting {
namespace graph {

class PgrDirectedChPPGraph {
 public:
     PgrDirectedChPPGraph(
             const pgr_edge_t *dataEdges,
             const size_t totalEdges);

     double DirectedChPP() const {
         return m_cost;
     }

     std::vector<General_path_element_t> GetPathEdges() const {
         return resultPath;
     }

      ~PgrDirectedChPPGraph();


 private:
     bool EulerCircuitDFS(int64_t p);
     void BuildResultGraph();
     void BuildResultPath();
     void setPathEdges(graph::PgrCostFlowGraph &flowGraph);

 private:
     int64_t totalDeg;
     double totalCost;
     int64_t superSource, superTarget;
     int64_t startPoint;
     double m_cost;
     Identifiers<int64_t> vertices;

     /** (source, target) -> idx to originalEdges;
      * Only the one with the lower cost is kept
      */
     std::map<std::pair<int64_t, int64_t>, const pgr_edge_t*> edgeToIdx;
     std::map<std::pair<int64_t, int64_t>,  // source, target
              size_t> edgeToId;  // index in resultEdges

     std::vector<pgr_edge_t> originalEdges;
     std::vector<pgr_edge_t> resultEdges;

     /** vector of vertex -> vector of edges **/
     std::vector<std::pair<int64_t, std::vector<size_t>>> resultGraph;
     std::map<int64_t, size_t> VToVecid;
     Identifiers<int64_t> edgeVisited;
     Identifiers<int64_t> vertexVisited;

     std::stack<int64_t> pathStack;  // node stack
     std::vector<General_path_element_t> resultPath;

     /* for the flow graph */
     std::vector<pgr_costFlow_t> edges;
     std::set<int64_t> sources;
     std::set<int64_t> targets;
};

PgrDirectedChPPGraph::~PgrDirectedChPPGraph() {
    edgeToIdx.clear();
}
PgrDirectedChPPGraph::PgrDirectedChPPGraph(
        const pgr_edge_t *dataEdges,
        const size_t totalEdges) :
    totalDeg(0), totalCost(0), vertices(),
    edgeToIdx(), originalEdges(),
    resultGraph(), VToVecid(), edgeVisited(),
    pathStack(), resultPath(),
    edges(), sources(), targets()
{
    pgassert(totalEdges > 0);
    pgassert(pathStack.empty());

    pgassert(originalEdges.empty());
    startPoint = dataEdges[0].source;
    for (size_t i = 0; i < totalEdges; ++i) {
        if (dataEdges[i].cost > 0) {
            auto edge(dataEdges[i]);
            edge.reverse_cost = -1.0;
            totalCost += edge.cost;
            originalEdges.push_back(edge);
            vertices += dataEdges[i].source;
            vertices += dataEdges[i].target;
        }
        if (dataEdges[i].reverse_cost > 0) {
            auto edge(dataEdges[i]);
            std::swap(edge.source, edge.target);
            std::swap(edge.cost, edge.reverse_cost);
            edge.reverse_cost = -1.0;
            totalCost += edge.cost;
            originalEdges.push_back(edge);
            vertices += dataEdges[i].source;
            vertices += dataEdges[i].target;
            pgassert(dataEdges[i].source == edge.target);
            pgassert(dataEdges[i].target == edge.source);
        }
    }

    // calcu deg & build part of edges
    std::map<int64_t, int> deg;
    size_t i(0);
    for (const auto &e : originalEdges) {
        pgassert(e.cost > 0);
        /* has out going edge */
        deg[e.source]++;
        /* has out incoming edge */
        deg[e.target]--;

        auto current_edge(std::make_pair(e.source, e.target));
        if (edgeToIdx.find(current_edge) == edgeToIdx.end()) {
            edgeToIdx[current_edge] = &e;
        } else {
            if (edgeToIdx[current_edge]->cost > e.cost) {
                edgeToIdx[current_edge] = &e;
            }
        }

        pgr_costFlow_t edge;
        edge.edge_id = e.id;
        edge.reverse_capacity = -1;
        edge.reverse_cost = -1.0;
        edge.source = e.source;
        edge.target = e.target;
        edge.capacity = (std::numeric_limits<int32_t>::max)();
        edge.cost = e.cost;
        edges.push_back(edge);
        ++i;
    }

    superSource = deg.rbegin()->first + 1;
    superTarget = deg.rbegin()->first + 2;

    sources.insert(superSource);
    targets.insert(superTarget);

    // build full edges
    std::map<int64_t, int>::iterator iter;
    totalDeg = 0;
    for (iter = deg.begin(); iter != deg.end(); ++iter) {
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
        if (d > 0)
            edge.capacity = d;
        else
            edge.capacity = -d;
        edge.edge_id = 0;
        if (d > 0)
            edge.source = p, edge.target = superTarget;
        if (d < 0)
            edge.source = superSource, edge.target = p;
        edges.push_back(edge);
    }

    PgrCostFlowGraph flowGraph(edges, sources, targets);
    pgassert(pathStack.empty());
    try {
        pgrouting::graph::PgrCostFlowGraph digraph1(edges, sources, targets);
        auto minAddedCost = digraph1.MinCostMaxFlow();
        auto maxFlow = digraph1.GetMaxFlow();
        m_cost = (maxFlow == totalDeg)? minAddedCost + totalCost : -1.0;
    } catch (...) {
        m_cost = -1;
    }
    setPathEdges(flowGraph);
}


void
PgrDirectedChPPGraph::setPathEdges(graph::PgrCostFlowGraph &flowGraph) {
    pgassert(pathStack.empty());
    resultPath.clear();
    if (m_cost == -1) return;
    // catch new edges
    try {
        flowGraph.MinCostMaxFlow();
        flowGraph.GetMaxFlow();
        std::vector<pgr_flow_t> addedEdges = flowGraph.GetFlowEdges();
        resultEdges = originalEdges;
        for (auto &flow_t : addedEdges) {
            if (flow_t.source != superSource && flow_t.source != superTarget
                    && flow_t.target != superSource && flow_t.target != superTarget) {
                auto current_edge(std::make_pair(flow_t.source, flow_t.target));
                pgr_edge_t newEdge = *edgeToIdx[current_edge];
                /* adding edges that need to be traversed twice */
                while (flow_t.flow--) resultEdges.push_back(newEdge);
            }
        }
    } catch (...) {
        resultPath.clear();
        return;
    }

    pgassert(pathStack.empty());
    BuildResultGraph();

    pgassert(pathStack.empty());
    pgassert(edgeVisited.empty());


    EulerCircuitDFS(startPoint);

    if (!(vertices - vertexVisited).empty()) {
        resultPath.clear();
        return;
    }
    pgassert(!pathStack.empty());

    BuildResultPath();


    return;
}

void
PgrDirectedChPPGraph::BuildResultPath() {
    if (pathStack.empty()) return;
    pgassert(resultPath.empty());

    int64_t preNode = pathStack.top();
    pathStack.pop();

    General_path_element_t newElement;
    while (!pathStack.empty()) {
        int64_t nowNode = pathStack.top();
        pathStack.pop();

        auto edge_t = *edgeToIdx[std::make_pair(preNode, nowNode)];
        newElement.node = edge_t.source;
        newElement.edge = edge_t.id;
        newElement.cost = edge_t.cost;
        if (resultPath.empty()) {
            /* adding the first row because is a cycle */
            newElement.seq = 1;
            newElement.agg_cost = 0.0;
        } else {
            newElement.seq = resultPath.back().seq + 1;
            newElement.agg_cost = resultPath.back().agg_cost + resultPath.back().cost;
        }
        resultPath.push_back(newElement);
        preNode = nowNode;
    }
    newElement.node = preNode;
    newElement.edge = -1;
    newElement.cost = 0;
    if (resultPath.empty()) {
        newElement.seq = 1;
        newElement.agg_cost = 0.0;
    } else {
        newElement.seq = resultPath.back().seq + 1;
        newElement.agg_cost =
            resultPath.back().agg_cost + resultPath.back().cost;
    }
    resultPath.push_back(newElement);
}

// perform DFS approach to generate Euler circuit
// TODO(mg) find suitable API in BGL, maybe DfsVisitor will work.
// Implement DFS without BGL for now
bool
PgrDirectedChPPGraph::EulerCircuitDFS(int64_t vertex) {
    for (const auto e : resultGraph[VToVecid[vertex]].second) {
        if (!edgeVisited.has(e)) {
            edgeVisited += e;
            EulerCircuitDFS(resultEdges[e].target);
        }
    }
    pathStack.push(vertex);
    vertexVisited += vertex;
    return true;
}

void
PgrDirectedChPPGraph::BuildResultGraph() {
    pgassert(resultGraph.empty());
    pgassert(VToVecid.empty());
    resultGraph.clear();
    VToVecid.clear();
    size_t e_id(0);
    for (const auto &edge_t : resultEdges) {
        if (VToVecid.find(edge_t.source) == VToVecid.end()) {
            VToVecid[edge_t.source] = resultGraph.size();
            resultGraph.resize(resultGraph.size() + 1);
        }
        size_t vid = VToVecid[edge_t.source];
        resultGraph[vid].second.push_back(e_id);
        resultGraph[vid].first = edge_t.source;
        ++e_id;
    }
}


}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_CHINESE_PGR_CHINESEPOSTMAN_HPP_
