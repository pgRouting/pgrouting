/*PGR-GNU*****************************************************************
File: pgr_depthFirstSearch.hpp

Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Ashish Kumar
Mail: ashishkr23438@gmail.com

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

#ifndef INCLUDE_DEPTHFIRSTSEARCH_PGR_DEPTHFIRSTSEARCH_HPP_
#define INCLUDE_DEPTHFIRSTSEARCH_PGR_DEPTHFIRSTSEARCH_HPP_
#pragma once


#include <visitors/dfs_visitor_with_root.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/undirected_dfs.hpp>

#include <vector>
#include <map>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"

namespace pgrouting {
namespace functions {

//*************************************************************

template <class G>
class Pgr_depthFirstSearch : public pgrouting::Pgr_messages {
 public:
     typedef typename G::V V;
     typedef typename G::E E;

     /** @name DepthFirstSearch
      * @{
      * 
      */

     /** @brief depthFirstSearch function
      *
      * It does all the processing and returns the results.
      *
      * @param graph     the graph containing the edges
      * @param roots     the starting vertices
      * @param depth     the maximum depth of traversal
      * @param directed  tells whether the graph is directed or undirected
      *
      * @returns results, when results are found
      *
      * @see depthFirstSearch_single_vertex
      * @see get_results
      */
     std::vector<pgr_mst_rt> depthFirstSearch(
             G &graph,
             std::vector<int64_t> roots,
             int64_t depth,
             bool directed) {
         std::vector<pgr_mst_rt> results;

         for (auto root : roots) {
             std::vector<E> visited_order;

             if (graph.has_vertex(root)) {
                 results.push_back({root, 0, root, -1, 0.0, 0.0});

                 // get the graph root vertex
                 auto v_root(graph.get_V(root));

                 // perform the algorithm
                 depthFirstSearch_single_vertex(graph, v_root, visited_order, directed);

                 // get the results
                 auto result = get_results(visited_order, root, depth, graph);
                 results.insert(results.end(), result.begin(), result.end());
             }
         }

         return results;
     }

     //@}

 private:
     /** @brief Calls the Boost function
      *
      *
      * Calls [boost::depth_first_search](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/depth_first_search.html)
      * and [boost::undirected_dfs](https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/undirected_dfs.html)
      * for directed graphs and undirected graphs, respectively.
      *
      * @param graph          the graph containing the edges
      * @param root           the starting vertex
      * @param visited_order  vector which will contain the edges of the resulting traversal
      * @param directed       tells whether the graph is directed or undirected
      *
      * @returns bool  @b true, when results are found
      *
      * @see depthFirstSearch
      * @see get_results
      */
     bool depthFirstSearch_single_vertex(
                 G &graph,
                 V root,
                 std::vector<E> &visited_order,
                 bool directed) {
         using dfs_visitor = visitors::Dfs_visitor_with_root<V, E>;

         try {
             if (directed) {
                 boost::depth_first_search(
                     graph.graph,
                     visitor(dfs_visitor(root, visited_order))
                     .root_vertex(root));
             } else {
                 std::map<E, boost::default_color_type> edge_color;
                 boost::undirected_dfs(
                     graph.graph,
                     visitor(dfs_visitor(root, visited_order))
                     .edge_color_map(boost::make_assoc_property_map(edge_color))
                     .root_vertex(root));
             }
         } catch(found_goals &) {
             {}
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }
         return true;
     }

     /** @brief to get the results
      *
      * Uses the visited_order of vertices to get the results.
      * Also makes sure to consider only those nodes whose
      * depth is less than the max_depth.
      *
      * @param visited_order  vector which contains the edges of the resulting traversal
      * @param source         the starting vertex
      * @param max_depth      the maximum depth of traversal
      * @param graph          the graph containing the edges
      *
      * @returns bool  @b True, when results are found
      *
      * @see depthFirstSearch
      * @see depthFirstSearch_single_vertex
      */
     template <typename T>
     std::vector<pgr_mst_rt> get_results(
             T visited_order,
             int64_t source,
             int64_t max_depth,
             const G &graph) {
         std::vector<pgr_mst_rt> results;

         std::vector<double> agg_cost(graph.num_vertices(), 0);
         std::vector<int64_t> depth(graph.num_vertices(), 0);

         for (const auto edge : visited_order) {
             auto u = graph.source(edge);
             auto v = graph.target(edge);

             agg_cost[v] = agg_cost[u] + graph[edge].cost;
             depth[v] = depth[u] + 1;

             if (max_depth >= depth[v]) {
                 results.push_back({
                     source,
                     depth[v],
                     graph[v].id,
                     graph[edge].id,
                     graph[edge].cost,
                     agg_cost[v]
                 });
             }
         }
         return results;
     }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_DEPTHFIRSTSEARCH_PGR_DEPTHFIRSTSEARCH_HPP_
