/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "pgr_maxflow.hpp"

namespace pgrouting {
namespace graph {


PgrFlowGraph::PgrFlowGraph(const std::vector<pgr_edge_t> &edges,
        const std::set<int64_t> &source_vertices,
        const std::set<int64_t> &sink_vertices,
        int algorithm) {
    /* In multi source flow graphs, a super source is created connected to all sources with "infinite" capacity
     * The same applies for sinks.
     * To avoid code repetition, a supersource/sink is used even in the one to one signature.
     */

    /*
     * vertices = {sources} U {sink} U {edges.source} U {edge.target}
     */
    std::set<int64_t> vertices(source_vertices);
    vertices.insert(sink_vertices.begin(), sink_vertices.end());

    for (const auto e : edges) {
        vertices.insert(e.source);
        vertices.insert(e.target);
    }

    for (const auto id : vertices) {
        V v = add_vertex(graph);
        id_to_V.insert(std::pair<int64_t, V>(id, v));
        V_to_id.insert(std::pair<V, int64_t>(v, id));
    }

    supersource = add_vertex(graph);
    supersink = add_vertex(graph);


    capacity = get(boost::edge_capacity, graph);
    rev = get(boost::edge_reverse, graph);
    residual_capacity = get(boost::edge_residual_capacity, graph);

    /* Inserting edges
     * Push-relabel requires each edge to be mapped to its reverse with capacity 0.
     * The other algorithms have no such requirement. (can have have as many edges)
     */
    if (algorithm == 1) {
        insert_edges_push_relabel(edges);
    } else {
        insert_edges(edges);
    }

    set_supersource(source_vertices);
    set_supersink(sink_vertices);
}

void PgrFlowGraph::insert_edges_push_relabel(
        const std::vector<pgr_edge_t> &edges) {
    bool added;
    for (const auto edge : edges) {
        V v1 = get_boost_vertex(edge.source);
        V v2 = get_boost_vertex(edge.target);
        E e1, e1_rev, e2, e2_rev;
        if (edge.cost > 0) {
            boost::tie(e1, added) = boost::add_edge(v1, v2, graph);
            boost::tie(e1_rev, added) =
                boost::add_edge(v2, v1, graph);
            E_to_id.insert(std::pair<E, int64_t>(e1, edge.id));
            E_to_id.insert(std::pair<E, int64_t>(e1_rev, edge.id));
            capacity[e1] = (int64_t) edge.cost;
            capacity[e1_rev] = 0;
            rev[e1] = e1_rev;
            rev[e1_rev] = e1;
        }
        if (edge.reverse_cost > 0) {
            boost::tie(e2, added) = boost::add_edge(v2, v1, graph);
            boost::tie(e2_rev, added) =
                boost::add_edge(v1, v2, graph);
            E_to_id.insert(std::pair<E, int64_t>(e2, edge.id));
            E_to_id.insert(std::pair<E, int64_t>(e2_rev, edge.id));
            capacity[e2] = (int64_t) edge.reverse_cost;
            capacity[e2_rev] = 0;
            rev[e2] = e2_rev;
            rev[e2_rev] = e2;
        }
    }
}

void PgrFlowGraph::insert_edges(
        const std::vector<pgr_edge_t> &edges) {
    bool added;
    for (const auto edge : edges) {
        V v1 = get_boost_vertex(edge.source);
        V v2 = get_boost_vertex(edge.target);
        E e, e_rev;
        boost::tie(e, added) = boost::add_edge(v1, v2, graph);
        boost::tie(e_rev, added) =
            boost::add_edge(v2, v1, graph);
        E_to_id.insert(std::pair<E, int64_t>(e, edge.id));
        E_to_id.insert(std::pair<E, int64_t>(e_rev, edge.id));
        capacity[e] = edge.cost > 0 ? (int64_t) edge.cost : 0;
        capacity[e_rev] = edge.reverse_cost > 0
            ? (int64_t) edge.reverse_cost : 0;
        rev[e] = e_rev;
        rev[e_rev] = e;
    }
}

void PgrFlowGraph::set_supersource(
        const std::set<int64_t> &source_vertices) {
    bool added;
    for (int64_t source_id : source_vertices) {
        V source = get_boost_vertex(source_id);
        int64_t total = 0;
        for (auto edge = out_edges(source, graph).first;
                edge != out_edges(source, graph).second;
                ++edge) {
            total += capacity[*edge];
        }
        E e, e_rev;
        boost::tie(e, added) =
            boost::add_edge(supersource, source, graph);
        boost::tie(e_rev, added) =
            boost::add_edge(source, supersource, graph);

        capacity[e] = total;
        /* From sources to supersource has 0 capacity*/
        capacity[e_rev] = 0;
        rev[e] = e_rev;
        rev[e_rev] = e;
    }
}

void PgrFlowGraph::set_supersink(
        const std::set<int64_t> &sink_vertices) {
    bool added;
    for (int64_t sink_id : sink_vertices) {
        V sink = get_boost_vertex(sink_id);
        E e, e_rev;
        boost::tie(e, added) = boost::add_edge(sink, supersink, graph);
        boost::tie(e_rev, added) =
            boost::add_edge(supersink, sink, graph);
        /*
         * NOTE: int64_t crashes the server
         */
        /* From sinks to supersink has maximum capacity*/
        capacity[e] = (std::numeric_limits<int32_t>::max)();
        /* From supersink to sinks has 0 capacity*/
        capacity[e_rev] = 0;
        rev[e] = e_rev;
        rev[e_rev] = e;
    }

}

std::vector<pgr_flow_t>
PgrFlowGraph::get_flow_edges() const {
    std::vector<pgr_flow_t> flow_edges;
    E_it e, e_end;
    for (boost::tie(e, e_end) = boost::edges(graph); e != e_end;
            ++e) {
        // A supersource/supersink is used internally
        if (((capacity[*e] - residual_capacity[*e]) > 0) &&
                ((*e).m_source != supersource) &&
                ((*e).m_target != supersink)) {
            pgr_flow_t edge;
            edge.edge = get_edge_id(*e);
            edge.source = get_vertex_id((*e).m_source);
            edge.target = get_vertex_id((*e).m_target);
            edge.flow = capacity[*e] - residual_capacity[*e];
            edge.residual_capacity = residual_capacity[*e];
            flow_edges.push_back(edge);
        }
    }
    return flow_edges;
}

}  // namespace graph
}  // namespace pgrouting

