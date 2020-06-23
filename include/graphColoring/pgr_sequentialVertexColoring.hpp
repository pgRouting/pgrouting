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
using namespace boost;

namespace pgrouting {
namespace functions {

//*************************************************************

template <class G>
class Pgr_sequentialVertexColoring : public pgrouting::Pgr_messages {
public:
    typedef typename G::V V;
    typedef typename G::E E;
    typedef adjacency_list<listS, vecS, undirectedS> Graph;
    typedef graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef graph_traits<Graph>::vertices_size_type vertices_size_type;
    typedef property_map<Graph, vertex_index_t>::const_type vertex_index_map;

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
     * (https://www.boost.org/doc/libs/1_73_0/libs/graph/doc/sequential_vertex_coloring.html)
     */
    std::vector<pgr_vertex_color_rt> sequentialVertexColoring(
            G &graph) {
        std::vector<pgr_vertex_color_rt> results;

        typedef std::pair<int, int> Edge;
          enum nodes {A, B, C, D, E, n};
          Edge edge_array[] = { Edge(A, C), Edge(B, B), Edge(B, D), Edge(B, E), 
                                Edge(C, B), Edge(C, D), Edge(D, E), Edge(E, A), 
                                Edge(E, B) };
          int m = sizeof(edge_array) / sizeof(Edge);
          Graph g(edge_array, edge_array + m, n);

          // Test with the normal order
          std::vector<vertices_size_type> color_vec(num_vertices(g));
          iterator_property_map<vertices_size_type*, vertex_index_map>
            color(&color_vec.front(), get(vertex_index, g));
          vertices_size_type num_colors = sequential_vertex_coloring(g, color);

        return results;
    }

    //@}

};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_GRAPHCOLORING_PGR_SEQUENTIALVERTEXCOLORING_HPP_
