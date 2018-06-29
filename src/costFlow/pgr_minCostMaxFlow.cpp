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

#include "costFlow/pgr_minCostMaxFlow.hpp"

namespace pgrouting {
namespace graph {

PgrCostFlowGraph::PgrCostFlowGraph(
        const std::vector<pgr_costFlow_t> &edges,
        const std::set<int64_t> &source_vertices,
        const std::set<int64_t> &sink_vertices) {
    add_vertices(edges, source_vertices, sink_vertices);

    capacity = get(boost::edge_capacity, graph);
    weight = get(boost::edge_weight, graph);
    rev = get(boost::edge_reverse, graph);
    residual_capacity = get(boost::edge_residual_capacity, graph);

    insert_edges(edges);
}

PgrCostFlowGraph::E PgrCostFlowGraph::add_edge(
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

void PgrCostFlowGraph::insert_edges(
        const std::vector<pgr_costFlow_t> &edges) {
    for (const auto edge : edges) {
        PgrCostFlowGraph::E e1, e1_rev, e2, e2_rev;
        V v1 = get_boost_vertex(edge.source);
        V v2 = get_boost_vertex(edge.target);

        if (edge.capacity > 0) {
            e1 = add_edge(v1, v2, edge.cost,
                                  static_cast<double>(edge.capacity));
            e1_rev = add_edge(v2, v1, -edge.cost, 0);

            E_to_id.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e1, edge.edge_id));
            E_to_id.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e1_rev, edge.edge_id));

            rev[e1] = e1_rev;
            rev[e1_rev] = e1;
        }

        if (edge.reverse_capacity > 0) {
            e2 = add_edge(v2, v1, edge.reverse_cost,
                                  static_cast<double>(edge.reverse_capacity));
            e2_rev = add_edge(v1, v2, -edge.reverse_cost, 0);

            E_to_id.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e2, edge.edge_id));
            E_to_id.insert(std::pair<PgrCostFlowGraph::E,
                                     int64_t>(e2_rev, edge.edge_id));

            rev[e2] = e2_rev;
            rev[e2_rev] = e2;
        }
    }
}

void PgrCostFlowGraph::set_supersource(
        const std::set<int64_t> &source_vertices) {
    supersource = add_vertex(graph);
    for (int64_t source_id : source_vertices) {
        PgrCostFlowGraph::V source = get_boost_vertex(source_id);
        PgrCostFlowGraph::E e, e_rev;
        e = add_edge(supersource, source,
                     0, (std::numeric_limits<int32_t>::max)());
        e_rev = add_edge(source, supersource, 0, 0);
        rev[e] = e_rev;
        rev[e_rev] = e;
    }
}

void PgrCostFlowGraph::set_supersink(
        const std::set<int64_t> &sink_vertices) {
    supersink = add_vertex(graph);
    for (int64_t sink_id : sink_vertices) {
        PgrCostFlowGraph::V sink = get_boost_vertex(sink_id);
        PgrCostFlowGraph::E e, e_rev;
        e = add_edge(sink, supersink, 0, (std::numeric_limits<int32_t>::max)());
        e_rev = add_edge(supersink, sink, 0, 0);
        rev[e] = e_rev;
        rev[e_rev] = e;
    }
}

int64_t
PgrCostFlowGraph::get_max_flow() const {
    int64_t max_flow = 0;
    E_it e, e_end;
    for (boost::tie(e, e_end) = boost::edges(graph); e != e_end; ++e) {
        if (((capacity[*e] - residual_capacity[*e]) > 0) &&
                ((*e).m_source == supersource))
            max_flow += 
                static_cast<int64_t>(capacity[*e] - residual_capacity[*e]);
    }
    return max_flow;
}

std::vector<pgr_flow_t>
PgrCostFlowGraph::get_flow_edges() const {
    std::vector<pgr_flow_t> flow_edges;
    E_it e, e_end;
    for (boost::tie(e, e_end) = boost::edges(graph); e != e_end; ++e) {
        if (((capacity[*e] - residual_capacity[*e]) > 0) &&
                ((*e).m_source != supersource) &&
                ((*e).m_target != supersink)) {
            pgr_flow_t edge;
            edge.edge = get_edge_id(*e);
            edge.source = get_vertex_id((*e).m_source);
            edge.target = get_vertex_id((*e).m_target);
            edge.flow =
                static_cast<int64_t>(capacity[*e] - residual_capacity[*e]);
            edge.residual_capacity =
                static_cast<int64_t>(residual_capacity[*e]);
            edge.cost = weight[*e] * static_cast<double>(edge.flow);
            if (flow_edges.size() == 0)
                edge.agg_cost = edge.cost;
            else
                edge.agg_cost = (flow_edges.back()).agg_cost + edge.cost;
            flow_edges.push_back(edge);
        }
    }
    return flow_edges;
}

}  // namespace graph
}  // namespace pgrouting
