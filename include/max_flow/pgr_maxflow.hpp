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

#ifndef INCLUDE_MAX_FLOW_PGR_MAXFLOW_HPP_
#define INCLUDE_MAX_FLOW_PGR_MAXFLOW_HPP_
#pragma once


#include "pgr_flowgraph.hpp"
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>


#include <map>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <limits>

#include "c_types/pgr_flow_t.h"
#include "c_types/pgr_edge_t.h"
#include "c_types/general_path_element_t.h"
#include "cpp_common/interruption.h"


namespace pgrouting {
namespace graph {


class PgrFlowGraph {
     typedef boost::graph_traits<FlowGraph>::vertex_descriptor V;
     typedef boost::graph_traits<FlowGraph>::edge_descriptor E;
     typedef boost::graph_traits<FlowGraph>::vertex_iterator V_it;
     typedef boost::graph_traits<FlowGraph>::edge_iterator E_it;
     typedef boost::graph_traits<FlowGraph>::out_edge_iterator Eout_it;


     boost::property_map
         <FlowGraph, boost::edge_capacity_t>::type capacity;
     boost::property_map
         <FlowGraph, boost::edge_reverse_t>::type rev;
     boost::property_map
         <FlowGraph, boost::edge_residual_capacity_t>::type residual_capacity;


 public:
     int64_t push_relabel() {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         return boost::push_relabel_max_flow(
                 graph,
                 supersource,
                 supersink);
     }

     int64_t edmonds_karp() {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         return boost::edmonds_karp_max_flow(
                 graph,
                 supersource,
                 supersink);
     }

     int64_t boykov_kolmogorov() {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         return boost::boykov_kolmogorov_max_flow(
                 graph,
                 supersource,
                 supersink);
     }

     std::vector<General_path_element_t> edge_disjoint_paths() {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         auto flow = boost::boykov_kolmogorov_max_flow(
                 graph,
                 supersource,
                 supersink);
         return get_edge_disjoint_paths(flow);
     }

     PgrFlowGraph(
             const std::vector<pgr_edge_t> &edges,
             const std::set<int64_t> &source_vertices,
             const std::set<int64_t> &sink_vertices,
             int algorithm);

     PgrFlowGraph(
             const std::vector<pgr_edge_t> &edges,
             const std::set<int64_t> &source_vertices,
             const std::set<int64_t> &sink_vertices,
             bool directed);


     std::vector<pgr_flow_t> get_flow_edges() const;

     std::vector<General_path_element_t> get_edge_disjoint_paths(
             int64_t flow);

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

     void insert_edges_push_relabel(
             const std::vector<pgr_edge_t> &edges);
     void insert_edges(
             const std::vector<pgr_edge_t> &edges);
     void insert_edges_edge_disjoint(
             const std::vector<pgr_edge_t> &edges,
             bool directed);

     void flow_dfs(
             V vertex,
             int64_t path_id,
             std::vector<std::vector<int64_t> > &paths);

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
     FlowGraph graph;
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

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_PGR_MAXFLOW_HPP_
