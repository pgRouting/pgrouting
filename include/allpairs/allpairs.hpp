/*PGR-GNU*****************************************************************
File: allpairs.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#ifndef INCLUDE_ALLPAIRS_ALLPAIRS_HPP_
#define INCLUDE_ALLPAIRS_ALLPAIRS_HPP_
#pragma once

#include <vector>
#include <limits>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"


namespace pgrouting  {
namespace detail {

template <typename T>
struct inf_plus {
    T operator()(const T& a, const T& b) const {
        T inf = (std::numeric_limits<T>::max)();
        if (a == inf || b == inf) return inf;
        return a + b;
    }
};

}  // namespace detail


template <class G>
std::vector<std::vector<double>>
floydWarshall(G &graph) {
    std::vector<std::vector<double>> matrix(graph.num_vertices(), std::vector<double>(graph.num_vertices(), 0));
    detail::inf_plus<double> combine;

    CHECK_FOR_INTERRUPTS();

    boost::floyd_warshall_all_pairs_shortest_paths(
            graph.graph,
            matrix,
            weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
            distance_combine(combine).
            distance_inf((std::numeric_limits<double>::max)()).
            distance_zero(0));

    return matrix;
}


template <class G>
std::vector<std::vector<double>>
johnson(G &graph) {
    std::vector<std::vector<double>> matrix(graph.num_vertices(), std::vector<double>(graph.num_vertices(), 0));
    detail::inf_plus<double> combine;

    CHECK_FOR_INTERRUPTS();

    boost::johnson_all_pairs_shortest_paths(
            graph.graph,
            matrix,
            weight_map(get(&pgrouting::Basic_edge::cost, graph.graph)).
            distance_combine(combine).
            distance_inf((std::numeric_limits<double>::max)()).
            distance_zero(0));

    return matrix;
}

}  // namespace pgrouting

#endif  // INCLUDE_ALLPAIRS_ALLPAIRS_HPP_
