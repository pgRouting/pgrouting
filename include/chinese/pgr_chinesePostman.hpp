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
     bool EulerCircuitDFS(int64_t p);
     void BuildResultGraph();
     void BuildResultPath();
     bool JudgeCoveredAllEdges();

 private:
     int64_t totalDeg;
     double totalCost;
     int64_t superSource, superTarget;
     int64_t startPoint;

     std::map<std::pair<int64_t, int64_t>,  // source, target
              size_t> edgeToId;  // index in resultEdges

     graph::PgrCostFlowGraph flowGraph;
     std::vector<pgr_edge_t> resultEdges;

     std::vector<std::pair<int64_t, std::vector<size_t> > > resultGraph;
     std::map<int64_t, size_t> VToVecid;
     std::vector<bool> edgeVisited;

     std::stack<int64_t> pathStack;  // node stack
     std::vector<General_path_element_t> resultPath;
};

PgrDirectedChPPGraph::PgrDirectedChPPGraph(
        const pgr_edge_t *dataEdges,
        const size_t totalEdges) :
    totalCost(0) {
    resultEdges.clear();
    for (size_t i = 0; i < totalEdges; i++) {
        pgr_edge_t edge;
        edge.id = dataEdges[i].id;
        edge.source = dataEdges[i].source;
        edge.target = dataEdges[i].target;
        edge.reverse_cost = -1.0;
        if (dataEdges[i].cost > 0) {
            startPoint = edge.source;
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

        if (edgeToId.find(std::make_pair(resultEdges[i].source, resultEdges[i].target)) ==
                edgeToId.end()) {
            edgeToId.insert(std::make_pair(std::make_pair(resultEdges[i].source, resultEdges[i].target), i));
        } else {
            if (resultEdges[edgeToId[
                    std::make_pair(resultEdges[i].source, resultEdges[i].target)]].cost >
                    resultEdges[i].cost) {
                edgeToId[std::make_pair(resultEdges[i].source, resultEdges[i].target)] = i;
            }
        }

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

    // find superSource & superTarget
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

    PgrCostFlowGraph graph(edges, sources, targets);
    flowGraph = graph;
}

bool
PgrDirectedChPPGraph::JudgeCoveredAllEdges() {
    for (const auto b : edgeVisited)
        if (!b)
            return false;
    return true;
}

std::vector<General_path_element_t>
PgrDirectedChPPGraph::GetPathEdges() {
    // catch new edges
    std::vector<pgr_flow_t> addedEdges = flowGraph.GetFlowEdges();
    for (auto &flow_t : addedEdges) {
        if (flow_t.source != superSource && flow_t.source != superTarget) {
            if (flow_t.target != superSource && flow_t.target != superTarget) {
                pgr_edge_t newEdge;
                newEdge.source = flow_t.source;
                newEdge.target = flow_t.target;
                newEdge.id =
                    resultEdges[edgeToId[std::make_pair(
                            newEdge.source,
                            newEdge.target)]].id;
                newEdge.cost =
                    resultEdges[edgeToId[std::make_pair(
                            newEdge.source,
                            newEdge.target)]].cost;
                newEdge.reverse_cost = -1.0;
                while (flow_t.flow--)
                    resultEdges.push_back(newEdge);
            }
        }
    }

    BuildResultGraph();

    EulerCircuitDFS(startPoint);

    BuildResultPath();

    if (!JudgeCoveredAllEdges())
        resultPath.clear();
    return resultPath;
}

void
PgrDirectedChPPGraph::BuildResultPath() {
    if (pathStack.empty())
        return;
    int64_t preNode = pathStack.top();
    pathStack.pop();

    General_path_element_t newElement;
    while (!pathStack.empty()) {
        int64_t nowNode = pathStack.top();
        pathStack.pop();
        pgr_edge_t edge_t =
            resultEdges[edgeToId[std::make_pair(preNode, nowNode)]];
        newElement.node = edge_t.source;
        newElement.edge = edge_t.id;
        newElement.cost = edge_t.cost;
        if (resultPath.empty()) {
            newElement.seq = 1;
            newElement.agg_cost = 0.0;
        } else {
            newElement.seq = resultPath.back().seq + 1;
            newElement.agg_cost =
                resultPath.back().agg_cost + resultPath.back().cost;
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
PgrDirectedChPPGraph::EulerCircuitDFS(int64_t p) {
    for (auto iter = resultGraph[VToVecid[p]].second.begin();
         iter != resultGraph[VToVecid[p]].second.end();
         ++iter) {
        if (!edgeVisited[*iter]) {
            edgeVisited[*iter] = true;
            EulerCircuitDFS(resultEdges[*iter].target);
        }
    }
    pathStack.push(p);
    return true;
}

void
PgrDirectedChPPGraph::BuildResultGraph() {
    resultGraph.clear();
    VToVecid.clear();
    edgeVisited.clear();
    for (size_t i = 0; i < resultEdges.size(); i++) {
        pgr_edge_t edge_t = resultEdges[i];
        edgeVisited.push_back(false);
        if (VToVecid.find(edge_t.source) == VToVecid.end()) {
            VToVecid.insert(
                    std::pair<int64_t, size_t>(
                        edge_t.source,
                        resultGraph.size()));
            resultGraph.resize(resultGraph.size() + 1);
        }
        size_t vid = VToVecid[edge_t.source];
        resultGraph[vid].second.push_back(i);
        resultGraph[vid].first = edge_t.source;
    }
}


}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_CHINESE_PGR_CHINESEPOSTMAN_HPP_
