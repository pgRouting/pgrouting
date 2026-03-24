/*PGR-GNU*****************************************************************
File: sequentialVertexColoring.hpp

Copyright (c) 2013-2026 pgRouting developers
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

#include "coloring/sequentialVertexColoring.hpp"

#include <vector>
#include <string>
#include <cstdint>

#include <boost/graph/sequential_vertex_coloring.hpp>

#include "cpp_common/interruption.hpp"

namespace {

using vertices_size_type = pgrouting::UndirectedGraph::vertices_size_type;

/** @brief to get the results
 *
 * Uses the `colors` vector to get the results i.e. the color of every vertex.
 *
 * @param colors      vector which contains the color of every vertex
 * @param graph       the graph containing the edges
 *
 * @returns `results` vector
 */
std::vector<II_t_rt> get_results(
        const std::vector<vertices_size_type> &colors,
        const pgrouting::UndirectedGraph &graph) {
    std::vector<II_t_rt> results;

    pgrouting::UndirectedGraph::V_i v, vend;

    for (boost::tie(v, vend) = vertices(graph.graph); v != vend; ++v) {
        int64_t node = graph[*v].id;
        auto color = colors[*v];
        results.push_back({{node}, {static_cast<int64_t>(color + 1)}});
    }

    // ordering the results in an increasing order of the node id
    std::sort(results.begin(), results.end(),
            [](const II_t_rt row1, const II_t_rt row2) {
            return row1.d1.id < row2.d1.id;
            });

    return results;
}

}  //  namespace

namespace pgrouting {
namespace functions {

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
std::vector<II_t_rt> sequentialVertexColoring(const pgrouting::UndirectedGraph &graph) {
    using vertices_size_type = pgrouting::UndirectedGraph::vertices_size_type;

    std::vector <II_t_rt> results;

    auto i_map = boost::get(boost::vertex_index, graph.graph);

    // vector which will store the color of all the vertices in the graph
    std::vector<vertices_size_type> colors(boost::num_vertices(graph.graph));

    // An iterator property map which records the color of each vertex
    auto color_map = boost::make_iterator_property_map(colors.begin(), i_map);

    CHECK_FOR_INTERRUPTS();

    try {
        boost::sequential_vertex_coloring(graph.graph, color_map);
    } catch (boost::exception const& ex) {
        throw;
    } catch (std::exception &e) {
        throw;
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong while calling boost::edge_coloring"),
                std::string(__PGR_PRETTY_FUNCTION__));;
    }

    return get_results(colors, graph);
}

}  // namespace functions
}  // namespace pgrouting

