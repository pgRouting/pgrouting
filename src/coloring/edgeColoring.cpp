/*PGR-GNU*****************************************************************
File: edgeColoring.cpp

Generated with Template by:
Copyright (c) 2021-2026 pgRouting developers
Mail: project@pgrouting.org

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
#include <utility>
#include <string>

#include <boost/graph/edge_coloring.hpp>
#include <boost/graph/graph_utility.hpp>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/interruption.hpp"


namespace pgrouting {
namespace functions {

std::vector<II_t_rt>
Pgr_edgeColoring::edgeColoring() {
    std::vector<II_t_rt> results;

    CHECK_FOR_INTERRUPTS();

    try {
        using E = pgrouting::UndirectedGraph::E;

        std::map<E, int64_t> color_storage;
        auto color_map = boost::make_assoc_property_map(color_storage);

        boost::edge_coloring(graph.graph, color_map);

        for (auto e_i : boost::make_iterator_range(boost::edges(graph.graph))) {
            int64_t edge_id = graph.graph[e_i].id;
            int64_t color = color_map[e_i];

            results.push_back({{edge_id}, {(color + 1)}});
        }
    } catch (...) {
        throw std::make_pair
        (
            std::string("INTERNAL: something went wrong while calling boost::edge_coloring"),
            std::string(__PGR_PRETTY_FUNCTION__));
    }

    return results;
}

Pgr_edgeColoring::Pgr_edgeColoring(const std::vector<Edge_t> &edges) {
    graph.insert_edges(edges);
}

}  // namespace functions
}  // namespace pgrouting
