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

#ifndef INCLUDE_MAX_FLOW_PGR_MINCOSTMAXFLOW_HPP_
#define INCLUDE_MAX_FLOW_PGR_MINCOSTMAXFLOW_HPP_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <limits>

#include "max_flow/pgr_costFlowGraph.hpp"

#if BOOST_VERSION_OK
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#else
#include "boost/successive_shortest_path_nonnegative_weights.hpp"
#include "boost/find_flow_cost.hpp"
#endif


#include "c_types/pgr_flow_t.h"
#include "c_types/pgr_costFlow_t.h"

namespace pgrouting {
namespace graph {

class PgrCostFlowGraph {
     typedef Traits::vertex_descriptor V;
     typedef Traits::edge_descriptor E;
     typedef boost::graph_traits<CostFlowGraph>::vertex_iterator V_it;
     typedef boost::graph_traits<CostFlowGraph>::edge_iterator E_it;

     Capacity capacity;
     ResidualCapacity residual_capacity;
     Reversed rev;
     Weight weight;

 public:
     double MinCostMaxFlow() {
         boost::successive_shortest_path_nonnegative_weights(
             graph,
             supersource,
             supersink);
         return boost::find_flow_cost(graph);
     }

     PgrCostFlowGraph() = default;

     PgrCostFlowGraph(
             const std::vector<pgr_costFlow_t> &edges,
             const std::set<int64_t> &source_vertices,
             const std::set<int64_t> &sink_vertices);

     int64_t GetMaxFlow() const;

     std::vector<pgr_flow_t> GetFlowEdges() const;

 private:
     V GetBoostVertex(int64_t id) const {
         return idToV.at(id);
     }

     int64_t GetVertexId(V v) const {
         return vToId.at(v);
     }

     int64_t GetEdgeId(E e) const {
         if (eToId.find(e) == eToId.end())
             return -1;
         return eToId.at(e);
     }

     void SetSupersource(
             const std::set<int64_t> &source_vertices);
     void SetSupersink(
             const std::set<int64_t> &sink_vertices);

     E AddEdge(V v, V w, double wei, double cap);

     void InsertEdges(
             const std::vector<pgr_costFlow_t> &edges);

     /*
      * vertices = {sources} U {sink} U {edges.source} U {edge.target}
      */
     template <typename T>
     void AddVertices(
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
             idToV.insert(std::pair<int64_t, V>(id, v));
             vToId.insert(std::pair<V, int64_t>(v, id));
         }

         SetSupersource(source_vertices);
         SetSupersink(sink_vertices);
     }

 private:
     CostFlowGraph graph;
     std::map<int64_t, V> idToV;
     std::map<V, int64_t> vToId;
     std::map<E, int64_t> eToId;


    /* In multi source flow graphs, a super source is created connected to all sources with "infinite" capacity
     * The same applies for sinks.
     * To avoid code repetition, a supersource/sink is used even in the one to one signature.
     */
     V supersource;
     V supersink;
};

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_PGR_MINCOSTMAXFLOW_HPP_
