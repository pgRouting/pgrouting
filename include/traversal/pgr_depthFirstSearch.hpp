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

#ifndef INCLUDE_TRAVERSAL_PGR_DEPTHFIRSTSEARCH_HPP_
#define INCLUDE_TRAVERSAL_PGR_DEPTHFIRSTSEARCH_HPP_
#pragma once

#include <vector>
#include <map>

#include <visitors/dfs_visitor.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/undirected_dfs.hpp>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
#include "c_types/mst_rt.h"


/** @file pgr_depthFirstSearch.hpp
 * @brief The main file which calls the respective boost function.
 *
 * Contains actual implementation of the function and the calling
 * of the respective boost function.
 */


namespace pgrouting {
namespace functions {

//*************************************************************

template < class G >
class Pgr_depthFirstSearch {
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
      * @param graph      the graph containing the edges
      * @param roots      the root vertices
      * @param max_depth  the maximum depth of traversal
      * @param directed   whether the graph is directed or undirected
      *
      * @returns results, when results are found
      *
      * @see [boost::depth_first_search]
      * (https://www.boost.org/libs/graph/doc/depth_first_search.html)
      * @see [boost::undirected_dfs]
      * (https://www.boost.org/libs/graph/doc/undirected_dfs.html)
      */
     std::vector < MST_rt > depthFirstSearch(
             G &graph,
             std::vector < int64_t > roots,
             bool directed,
             int64_t max_depth) {
         std::vector < MST_rt > results;

         for (auto root : roots) {
             std::vector < E > visited_order;
             results.push_back({root, 0, root, -1, 0.0, 0.0});

             if (graph.has_vertex(root)) {
                 auto v_root(graph.get_V(root));

                 depthFirstSearch_single_vertex(graph, v_root, visited_order, directed, max_depth);

                 auto result = get_results(visited_order, root, max_depth, graph);
                 results.insert(results.end(), result.begin(), result.end());
             }
         }

         return results;
     }

     //@}

 private:
     /** @brief Calls the Boost function
      *
      * Calls [boost::depth_first_search]
      * (https://www.boost.org/libs/graph/doc/depth_first_search.html)
      * and [boost::undirected_dfs]
      * (https://www.boost.org/libs/graph/doc/undirected_dfs.html)
      * for directed graphs and undirected graphs, respectively.
      *
      * @param graph          the graph containing the edges
      * @param root           the root vertex
      * @param visited_order  vector which will contain the edges of the resulting traversal
      * @param directed       whether the graph is directed or undirected
      * @param max_depth      Upper limit for depth to be returned
      *
      * @returns bool  @b true, when results are found
      */
     bool depthFirstSearch_single_vertex(
                 G &graph,
                 V root,
                 std::vector < E > &visited_order,
                 bool directed,
                 int64_t max_depth) {
         using dfs_visitor = visitors::Dfs_visitor < V, E, G >;

         // Exterior property storage containers
         std::vector < boost::default_color_type > colors(boost::num_vertices(graph.graph));
         std::map < E, boost::default_color_type > edge_color;

         auto i_map = boost::get(boost::vertex_index, graph.graph);

         // Iterator property maps which record the color of each vertex and edge, respectively
         auto vertex_color_map = boost::make_iterator_property_map(colors.begin(), i_map);
         auto edge_color_map = boost::make_assoc_property_map(edge_color);

         auto vis =  dfs_visitor(root, visited_order, max_depth, colors, graph);

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         try {
             if (directed) {
                 boost::depth_first_search(graph.graph, vis, vertex_color_map, root);
             } else {
                 boost::undirected_dfs(graph.graph, vis, vertex_color_map, edge_color_map, root);
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
      * Uses the `visited_order` of vertices to get the results.
      * Selects only those nodes in the final result whose
      * depth is less than the `max_depth`.
      *
      * @param visited_order  vector which contains the edges of the resulting traversal
      * @param root           the root vertex
      * @param max_depth      the maximum depth of traversal
      * @param graph          the graph containing the edges
      *
      * @returns `results` vector
      */
     template < typename T >
     std::vector < MST_rt > get_results(
             T visited_order,
             int64_t root,
             int64_t max_depth,
             const G &graph) {
         std::vector < MST_rt > results;

         std::vector < double > agg_cost(graph.num_vertices(), 0);
         std::vector < int64_t > depth(graph.num_vertices(), 0);

         for (const auto edge : visited_order) {
             auto u = graph.source(edge);
             auto v = graph.target(edge);

             agg_cost[v] = agg_cost[u] + graph[edge].cost;
             depth[v] = depth[u] + 1;

             if (max_depth >= depth[v]) {
                 results.push_back({
                     root,
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

#endif  // INCLUDE_TRAVERSAL_PGR_DEPTHFIRSTSEARCH_HPP_
