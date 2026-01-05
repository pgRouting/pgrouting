/*PGR-GNU*****************************************************************
File: cuthillMckeeOrdering.cpp

Generated with Template by:
Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Shobhit Chaurasia
Mail: 000shobhitchaurasia at gmail.com

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

#include <vector>
#include <cstdint>

#include <boost/config.hpp>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/cuthill_mckee_ordering.hpp>


namespace pgrouting {
namespace functions {


/** @brief cuthillMckeeOrdering function
 *
 * It does all the processing and returns the results.
 *
 * @param graph      the graph containing the edges
 *
 * @returns results, when results are found
 *
 * @see [boost::cuthill_mckee_ordering]
 * (https://www.boost.org/libs/graph/doc/cuthill_mckee_ordering.html)
 */
std::vector<pgrouting::UndirectedGraph::V>
cuthillMckeeOrdering(pgrouting::UndirectedGraph &graph) {
    using V = typename pgrouting::UndirectedGraph::V;

    /* number of vertices */
    size_t n = boost::num_vertices(graph.graph);

    /* map which store the indices with their nodes. */
    auto index_map = boost::get(boost::vertex_index, graph.graph);

    /* vector which will store the order of the indices. */
    std::vector<V> inv_permutation(n);

    /* vector which will store the color of all the vertices in the graph */
    std::vector<boost::default_color_type> colors(n);

    /* An iterator property map which records the color of each vertex */
    auto color_map = boost::make_iterator_property_map(colors.begin(), index_map, colors[0]);

    /* map which store the degree of each vertex. */
    auto degree_map = boost::make_out_degree_map(graph.graph);


    CHECK_FOR_INTERRUPTS();

    try {
        boost::cuthill_mckee_ordering(graph.graph, inv_permutation.rbegin(), color_map, degree_map);
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
        throw;
    }

    return inv_permutation;
}

}  // namespace functions
}  // namespace pgrouting
