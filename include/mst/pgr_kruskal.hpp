/*PGR-GNU*****************************************************************
File: pgr_kruskal.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Aditya Pratap Singh
adityapratap.singh28@gmail.com

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

#ifndef INCLUDE_MST_PGR_KRUSKAL_HPP_
#define INCLUDE_MST_PGR_KRUSKAL_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_kruskal {
 public:
     typedef typename G::V V;
     typedef typename G::E E;

     std::vector<pgr_kruskal_t> operator() (G &graph);

 private:
     std::vector< pgr_kruskal_t >
     generateKruskal(
        const G &graph);
};


namespace {
/* @brief maps component number with smallest original vertex id
 *
 *
 */
template <class G>
std::vector<int64_t>
component_id(const std::vector<size_t> &components, size_t num_comps, bool get_component, const G &graph) {
    std::vector<int64_t> tree_id(num_comps, 0);

    if (get_component) {
        for (const auto v : boost::make_iterator_range(vertices(graph.graph))) {
            tree_id[components[v]] =
                (tree_id[components[v]] == 0
                 || tree_id[components[v]] >= graph[v].id) ?
                graph[v].id : tree_id[components[v]];
        }
    } else {
        std::iota(tree_id.begin(), tree_id.end(), 0);
    }
    return tree_id;

}

template <class G>
std::vector<pgr_kruskal_t>
order_results(std::vector<pgr_kruskal_t> &results, int order_by, const G &graph) {

    /*
     * order by discovered edge
     * No aggregate costs given back on result
     */
    if (order_by == 0) {
        return results;
    }

    /*
     * order by component
     */
    if (order_by == 1) {
        std::sort(results.begin(), results.end(),
                [](const pgr_kruskal_t &l, const pgr_kruskal_t &r ) {
                    return l.component < r.component;
                });
    }

    return results;
#if 0
    std::vector<pgr_kruskal_t> &results;
    /*
     * order by min_node on edge
     */
    if (order_by = 2) {
        std::stable_sort(results.begin(), results.end(),
                [](const pgr_kruskal_t &l, const pgr_kruskal_t &r ) {
                    return l.min_node < r.min_node;
                });
    }
#endif
}

}  // namespace


/* IMPLEMENTATION */
template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::generateKruskal(const G &graph) {
    bool get_component = true;

    std::vector<E> spanning_tree;
    boost::kruskal_minimum_spanning_tree(
            graph.graph,
            std::back_inserter(spanning_tree),
            boost::weight_map(get(&G::G_T_E::cost, graph.graph)));

    std::vector< pgr_kruskal_t > results;
    size_t totalNodes = num_vertices(graph.graph);

    /*
     * components[v] has the component number of vertex v
     */
    std::vector<size_t> components(totalNodes);
    /*
     * Calculate connected components
     *
     * iNumber of components of graph: num_comps components
     */
    auto num_comps = boost::connected_components(
            graph.graph,
            &components[0]);

    auto tree_id = component_id(components, num_comps, get_component, graph);


    for (const auto edge : spanning_tree) {
        results.push_back({
            tree_id[components[graph.source(edge)]],
            std::min(graph[graph.source(edge)].id, graph[graph.target(edge)].id),
            graph[graph.source(edge)].id,
            graph[graph.target(edge)].id,
            graph[edge].id,
            graph[edge].cost,
            0
        });
    }


    return order_results(results, 1, graph);
}

template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::operator() (G &graph) {
    return generateKruskal(graph);
}


}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_KRUSKAL_HPP_
