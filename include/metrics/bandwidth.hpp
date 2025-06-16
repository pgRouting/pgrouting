/*PGR-GNU*****************************************************************
File: bandwidth.hpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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

#ifndef INCLUDE_METRICS_BANDWIDTH_HPP_
#define INCLUDE_METRICS_BANDWIDTH_HPP_
#pragma once

#include <vector>
#include <limits>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "c_types/iid_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

namespace pgrouting {
namespace metrics {

template <class G>
int bandwidth(const G &graph) {
    CHECK_FOR_INTERRUPTS();

    using GraphType = typename std::remove_reference<decltype(graph.graph)>::type;
    using Vertex = typename boost::graph_traits<GraphType>::vertex_descriptor;

    int bw = std::numeric_limits<int>::max();

    // We need a vertex index map for consistent ordering
    auto index_map = boost::get(boost::vertex_index, graph.graph);

    // Loop over all edges to compute max absolute difference of vertex indices
    typename boost::graph_traits<GraphType>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(graph.graph); ei != ei_end; ++ei) {
        Vertex u = boost::source(*ei, graph.graph);
        Vertex v = boost::target(*ei, graph.graph);

        int diff = std::abs(static_cast<int>(index_map[u]) - static_cast<int>(index_map[v]));
        if (diff < bw) {
            bw = diff;
        }
    }

    // If graph has no edges, bandwidth is zero
    if (bw == std::numeric_limits<int>::max()) {
        bw = 0;
    }
    return bw;
}

}  // namespace metrics
}  // namespace pgrouting

#endif  // INCLUDE_METRICS_BANDWIDTH_HPP_
