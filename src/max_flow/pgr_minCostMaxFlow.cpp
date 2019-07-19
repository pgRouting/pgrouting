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

#include "max_flow/pgr_minCostMaxFlow.hpp"

#include <set>
#include <vector>
#include <utility>
#include <limits>

namespace pgrouting {
namespace graph {

PgrCostFlowGraph::PgrCostFlowGraph(
        const std::vector<pgr_costFlow_t> &edges,
        const std::set<int64_t> &sourceVertices,
        const std::set<int64_t> &sinkVertices) {
    AddVertices(edges, sourceVertices, sinkVertices);

    capacity = get(boost::edge_capacity, graph);
    weight = get(boost::edge_weight, graph);
    rev = get(boost::edge_reverse, graph);
    residual_capacity = get(boost::edge_residual_capacity, graph);

    InsertEdges(edges);
}

PgrCostFlowGraph::E PgrCostFlowGraph::AddEdge(
        PgrCostFlowGraph::V v,
        PgrCostFlowGraph::V w, double wei, double cap) {
    bool b;
    PgrCostFlowGraph::E e;
    boost::tie(e, b) = boost::add_edge(vertex(v, graph),
                                       vertex(w, graph), graph);
    capacity[e] = cap;
    weight[e] = wei;
    return e;
}

void PgrCostFlowGraph::InsertEdges(
        const std::vector<pgr_costFlow_t> &edges) {
    for (const auto edge : edges) {
        PgrCostFlowGraph::E e1, e1Rev, e2, e2Rev;
        V v1 = GetBoostVertex(edge.source);
        V v2 = GetBoostVertex(edge.target);

        if (edge.capacity > 0) {
            e1 = AddEdge(v1, v2, edge.cost,
                                  static_cast<double>(edge.capacity));
            e1Rev = AddEdge(v2, v1, -edge.cost, 0);

            eToId.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e1, edge.edge_id));
            eToId.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e1Rev, edge.edge_id));

            rev[e1] = e1Rev;
            rev[e1Rev] = e1;
        }

        if (edge.reverse_capacity > 0) {
            e2 = AddEdge(v2, v1, edge.reverse_cost,
                                  static_cast<double>(edge.reverse_capacity));
            e2Rev = AddEdge(v1, v2, -edge.reverse_cost, 0);

            eToId.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e2, edge.edge_id));
            eToId.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e2Rev, edge.edge_id));

            rev[e2] = e2Rev;
            rev[e2Rev] = e2;
        }
    }
}

void PgrCostFlowGraph::SetSupersource(
        const std::set<int64_t> &sourceVertices) {
    supersource = add_vertex(graph);
    for (int64_t source_id : sourceVertices) {
        PgrCostFlowGraph::V source = GetBoostVertex(source_id);
        PgrCostFlowGraph::E e, eRev;
        e = AddEdge(supersource, source,
                     0, (std::numeric_limits<int32_t>::max)());
        eRev = AddEdge(source, supersource, 0, 0);
        rev[e] = eRev;
        rev[eRev] = e;
    }
}

void PgrCostFlowGraph::SetSupersink(
        const std::set<int64_t> &sinkVertices) {
    supersink = add_vertex(graph);
    for (int64_t sink_id : sinkVertices) {
        PgrCostFlowGraph::V sink = GetBoostVertex(sink_id);
        PgrCostFlowGraph::E e, eRev;
        e = AddEdge(sink, supersink, 0, (std::numeric_limits<int32_t>::max)());
        eRev = AddEdge(supersink, sink, 0, 0);
        rev[e] = eRev;
        rev[eRev] = e;
    }
}

int64_t
PgrCostFlowGraph::GetMaxFlow() const {
    int64_t maxFlow = 0;
    E_it e, eEnd;
    for (boost::tie(e, eEnd) = boost::edges(graph); e != eEnd; ++e) {
        if (((capacity[*e] - residual_capacity[*e]) > 0) &&
                ((*e).m_source == supersource))
            maxFlow +=
                static_cast<int64_t>(capacity[*e] - residual_capacity[*e]);
    }
    return maxFlow;
}

std::vector<pgr_flow_t>
PgrCostFlowGraph::GetFlowEdges() const {
    std::vector<pgr_flow_t> flowEdges;
    E_it e, eEnd;
    for (boost::tie(e, eEnd) = boost::edges(graph); e != eEnd; ++e) {
        if (((capacity[*e] - residual_capacity[*e]) > 0) &&
                ((*e).m_source != supersource) &&
                ((*e).m_target != supersink)) {
            pgr_flow_t edge;
            edge.edge = GetEdgeId(*e);
            edge.source = GetVertexId((*e).m_source);
            edge.target = GetVertexId((*e).m_target);
            edge.flow =
                static_cast<int64_t>(capacity[*e] - residual_capacity[*e]);
            edge.residual_capacity =
                static_cast<int64_t>(residual_capacity[*e]);
            edge.cost = weight[*e] * static_cast<double>(edge.flow);
            if (flowEdges.size() == 0)
                edge.agg_cost = edge.cost;
            else
                edge.agg_cost = (flowEdges.back()).agg_cost + edge.cost;
            flowEdges.push_back(edge);
        }
    }
    return flowEdges;
}

}  // namespace graph
}  // namespace pgrouting
