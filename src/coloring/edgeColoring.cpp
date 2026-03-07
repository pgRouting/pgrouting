/*PGR-GNU*****************************************************************
File: edgeColoring.cpp

Generated with Template by:
Copyright (c) 2021-2026 pgRouting developers
Mail: project@pgrouting.org

Rewrite:
Copyright (c) 2026 Vicky Vergara
Mail: vicky at erosion.dev

Function's developer:
Copyright (c) 2021 Veenit Kumar
Mail: 123sveenit@gmail.com
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

#include "coloring/edgeColoring.hpp"

#include <vector>
#include <string>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edge_coloring.hpp>
#include <boost/property_map/property_map.hpp>

#include "cpp_common/interruption.hpp"


namespace pgrouting {
namespace functions {


std::vector<II_t_rt>
edgeColoring(pgrouting::UndirectedGraph g) {
    std::vector<II_t_rt> results(boost::num_edges(g.graph));
    using B_G = pgrouting::UndirectedGraph::B_G;
    using E  = pgrouting::UndirectedGraph::E;

    // Create a std::map to store edge colors
    std::map<E, int64_t> edge_colors_map;

    // Create an associated property map adaptor
    boost::associative_property_map<std::map<E, int64_t>> color_map(edge_colors_map);

    int64_t colors = 0;
    // Run the edge coloring algorithm
    CHECK_FOR_INTERRUPTS();
    try {
        colors = boost::edge_coloring(g.graph, color_map);
    } catch (...) {
        throw std::make_pair(
            std::string("INTERNAL: something went wrong while calling boost::edge_coloring"),
            std::string(__PGR_PRETTY_FUNCTION__));
    }

    size_t i = 0;
    boost::graph_traits<B_G>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(g.graph); ei != ei_end; ++ei, ++i) {
        results[i].d1.id = static_cast<int64_t>(g[*ei].id);

        /**
         * There is a problem with boost:
         * Sometimes it returns a color with outsatnding large value
         * When that happens changing color to: colors + 1
         */
        results[i].d2.value = get(color_map, *ei) < colors? get(color_map, *ei) + 1 : colors + 1 ;
    }

    return results;
}

}  // namespace functions
}  // namespace pgrouting
