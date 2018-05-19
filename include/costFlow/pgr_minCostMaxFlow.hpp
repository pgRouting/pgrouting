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
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

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

     E add_edge(V v, V w, double weight, double capacity);

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

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COSTFLOW_PGR_MINCOSTMAXFLOW_HPP_
