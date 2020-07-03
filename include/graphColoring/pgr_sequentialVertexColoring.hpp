/*PGR-GNU*****************************************************************
File: pgr_sequentialVertexColoring.hpp

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

#ifndef INCLUDE_GRAPHCOLORING_PGR_SEQUENTIALVERTEXCOLORING_HPP_
#define INCLUDE_GRAPHCOLORING_PGR_SEQUENTIALVERTEXCOLORING_HPP_
#pragma once


#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

#include <vector>
#include <map>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"


/** @file pgr_sequentialVertexColoring.hpp
 * @brief The main file which calls the respective boost function.
 *
 * Contains actual implementation of the function and the calling
 * of the respective boost function.
 */


namespace pgrouting {
namespace functions {

//*************************************************************

template <class G>
class Pgr_sequentialVertexColoring : public pgrouting::Pgr_messages {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> Graph;
     typedef boost::graph_traits<Graph>::vertices_size_type vertices_size_type;

     /** @name SequentialVertexColoring
      * @{
      * 
      */

     /** @brief sequentialVertexColoring function
      *
      * It does all the processing and returns the results.
      *
      * @param graph      the graph containing the edges
      *
      * @returns results, when results are found
      *
      * @see [boost::sequential_vertex_coloring]
      * (https://www.boost.org/libs/graph/doc/sequential_vertex_coloring.html)
      */
     std::vector<pgr_vertex_color_rt> sequentialVertexColoring(
             G &graph) {
         std::vector<pgr_vertex_color_rt> results;

         // vector which will store the color of all the vertices in the graph
         std::vector<vertices_size_type> colors(boost::num_vertices(graph.graph));

         // An iterator property map which records the colors of each vertex
         auto color_map = boost::make_iterator_property_map(colors.begin(),
             boost::get(boost::vertex_index, graph.graph));

         try {
             // calling the boost function
             boost::sequential_vertex_coloring(
                 graph.graph, color_map);
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }

         results = get_results(colors, graph);

         return results;
     }

     //@}

 private:
     /** @brief to get the results
      *
      * Uses the `colors` vector to get the results i.e. the color of every vertex.
      *
      * @param colors      vector which contains the color of every vertex
      * @param graph       the graph containing the edges
      *
      * @returns `results` vector
      */
     std::vector<pgr_vertex_color_rt> get_results(
             std::vector<vertices_size_type> &colors,
             const G &graph) {
         std::vector<pgr_vertex_color_rt> results;

         typename boost::graph_traits<Graph>::vertex_iterator v, vend;

         // iterate through every vertex in the graph
         for (boost::tie(v, vend) = vertices(graph.graph); v != vend; ++v) {
             int64_t node = graph[*v].id;
             int64_t color = colors[*v];

             // push the vertex id and the color of the vertex in the `results` vector
             results.push_back({
                 node,
                 color
             });
         }

         // ordering the results in an increasing order of the node id
         std::sort(results.begin(), results.end(),
             [](const pgr_vertex_color_rt row1, const pgr_vertex_color_rt row2) {
                 return row1.node < row2.node; 
             });

         return results;
     }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_GRAPHCOLORING_PGR_SEQUENTIALVERTEXCOLORING_HPP_
