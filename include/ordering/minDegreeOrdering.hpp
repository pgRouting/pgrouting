/*PGR-GNU*****************************************************************
File: minDegreeOrdering.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Developer:
Copyright (c) 2025 Fan Wu
Mail: wifiblack0131 at gmail.com

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

#ifndef INCLUDE_ORDERING_MINDEGREEORDERING_HPP_
#define INCLUDE_ORDERING_MINDEGREEORDERING_HPP_
#pragma once

#include <vector>
#include <limits>
#include <iterator>
#include <utility>
#include <string>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>


#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include <boost/graph/minimum_degree_ordering.hpp>


namespace pgrouting  {

template <class G>
std::vector<int64_t>
minDegreeOrdering(G &graph) {
    using V = typename G::V;

    size_t n = boost::num_vertices(graph.graph);
    std::vector<int64_t> results(n);

    auto index_map = boost::get(boost::vertex_index, graph.graph);

    std::vector<V> degrees(n);
    auto degree_map = boost::make_iterator_property_map(degrees.begin(), index_map);

    std::vector<V> supernode_sizes(n, 1);
    auto supernode_map = boost::make_iterator_property_map(supernode_sizes.begin(), index_map);

    std::vector<V> permutation(n);
    std::vector<V> inv_permutation(n);

    for (V v = 0; v < n; ++v) {
        degree_map[v] = boost::degree(v, graph.graph);
    }

    CHECK_FOR_INTERRUPTS();

    boost::minimum_degree_ordering(
        graph.graph,
        degree_map,
        inv_permutation.begin(),
        permutation.begin(),
        supernode_map,
        0,
        index_map);

    size_t j = 0;
    for (auto i = inv_permutation.begin(); i != inv_permutation.end(); ++i, ++j) {
        results[j] = static_cast<int64_t>(graph.graph[index_map[*i]].id);
    }

    return results;
}

}  // namespace pgrouting

#endif  // INCLUDE_ORDERING_ORDERING_HPP_
