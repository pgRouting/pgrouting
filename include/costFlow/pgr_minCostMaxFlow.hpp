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

#ifndef INCLUDE_COSTFLOW_PGR_MINCOSTMAXFLOW_HPP_
#define INCLUDE_COSTFLOW_PGR_MINCOSTMAXFLOW_HPP_
#pragma once


#include "pgr_costFlowGraph.hpp"
#include "successive_shortest_path_nonnegative_weights.hpp"
#include "find_flow_cost.hpp"

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <limits>

#include "c_types/pgr_flow_t.h"
#include "c_types/pgr_costFlow_t.h"

namespace pgrouting {
namespace graph {

class PgrCostFlowGraph {
     typedef boost::graph_traits<CostFlowGraph>::vertex_descriptor V;
     typedef boost::graph_traits<CostFlowGraph>::edge_descriptor E;
     typedef boost::graph_traits<CostFlowGraph>::vertex_iterator V_it;
     typedef boost::graph_traits<CostFlowGraph>::edge_iterator E_it;
     typedef boost::graph_traits<CostFlowGraph>::out_edge_iterator Eout_it;

     Capacity capacity;
     ResidualCapacity residual_capacity;
     Reversed rev;
     Weight weight;

 public:
     double minCostMaxFlow() {
         boost::successive_shortest_path_nonnegative_weights(
             graph,
             supersource,
             supersink);
         return boost::find_flow_cost(graph);
     }

     PgrCostFlowGraph(
             const std::vector<pgr_costFlow_t> &edges,
             const std::set<int64_t> &source_vertices,
             const std::set<int64_t> &sink_vertices);

     std::vector<pgr_flow_t> get_flow_edges() const;

 private:
     V get_boost_vertex(int64_t id) const {
         return id_to_V.at(id);
     }

     int64_t get_vertex_id(V v) const {
         return V_to_id.at(v);
     }

     int64_t get_edge_id(E e) const {
         return E_to_id.at(e);
     }

     void set_supersource(
             const std::set<int64_t> &source_vertices);
     void set_supersink(
             const std::set<int64_t> &sink_vertices);

     E add_edge(V v, V w, double wei, double cap);

     void insert_edges(
             const std::vector<pgr_costFlow_t> &edges);

     /*
      * vertices = {sources} U {sink} U {edges.source} U {edge.target}
      */
     template <typename T>
     void add_vertices(
             const T &edges,
             const std::set<int64_t> &source_vertices,
             const std::set<int64_t> &sink_vertices) {
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

         set_supersource(source_vertices);
         set_supersink(sink_vertices);
     }

 private:
     CostFlowGraph graph;
     std::map<int64_t, V> id_to_V;
     std::map<V, int64_t> V_to_id;
     std::map<E, int64_t> E_to_id;


    /* In multi source flow graphs, a super source is created connected to all sources with "infinite" capacity
     * The same applies for sinks.
     * To avoid code repetition, a supersource/sink is used even in the one to one signature.
     */
     V supersource;
     V supersink;
};

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
    boost::tie(e, b) = boost::add_edge(vertex(v, graph), vertex(w, graph), graph);
    capacity[e] = cap;
    weight[e] = wei;
    return e;
}

void PgrCostFlowGraph::insert_edges(
        const std::vector<pgr_costFlow_t> &edges) {
    for (const auto edge : edges) {
        PgrCostFlowGraph::E e, e_rev;
        V v1 = get_boost_vertex(edge.source);
        V v2 = get_boost_vertex(edge.target);

        e = add_edge(v1, v2, edge.cost, edge.capacity);
        if (edge.reverse_capacity > 0)
            e_rev = add_edge(v2, v1, edge.reverse_cost, edge.reverse_capacity);
        else
            e_rev = add_edge(v2, v1, -edge.cost, 0);

        E_to_id.insert(std::pair<PgrCostFlowGraph::E, int64_t>(e, edge.edge_id));
        E_to_id.insert(std::pair<PgrCostFlowGraph::E, int64_t>(e_rev, edge.edge_id));
        
        rev[e] = e_rev;
        rev[e_rev] = e;
    }
}

void PgrCostFlowGraph::set_supersource(
        const std::set<int64_t> &source_vertices) {
    bool added;
    supersource = add_vertex(graph);
    for (int64_t source_id : source_vertices) {
        PgrCostFlowGraph::V source = get_boost_vertex(source_id);
        PgrCostFlowGraph::E e, e_rev;
        e = add_edge(supersource, source, 0, (std::numeric_limits<int32_t>::max)());
        e_rev = add_edge(source, supersource, 0, 0);
        rev[e] = e_rev;
        rev[e_rev] = e;
    }
}

void PgrCostFlowGraph::set_supersink(
        const std::set<int64_t> &sink_vertices) {
    bool added;
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
            edge.flow = capacity[*e] - residual_capacity[*e];
            edge.residual_capacity = residual_capacity[*e];
            edge.cost = weight[*e] * edge.flow;
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

#endif  // INCLUDE_COSTFLOW_PGR_MINCOSTMAXFLOW_HPP_
