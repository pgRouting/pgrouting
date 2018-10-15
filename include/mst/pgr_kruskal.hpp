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

/* IMPLEMENTATION */
template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::generateKruskal(const G &graph) {
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
    size_t num_comps = boost::connected_components(
            graph.graph,
            &components[0]);

    /*
     * component[i] is a vector of original vertex id
     */
    std::vector<std::vector<int64_t>> component;
    component.resize(num_comps);

    for (auto v : boost::make_iterator_range(vertices(graph.graph))) {
        component[components[v]].push_back(graph[v].id);
    }

    for (size_t i = 0; i < num_comps; i++) {
        std::sort(component[i].begin(), component[i].end());
    }
    sort(component.begin(), component.end());


    size_t size = component.size();
    for (size_t i = 0; i < size; i++) {
        double totalcost = 0;
        for (auto ei = spanning_tree.begin();
                ei != spanning_tree.end();
                ++ei) {
            if (components[source(*ei, graph.graph)] == i) {
                pgr_kruskal_t tmp;

                tmp.component = component[i][0];;
                tmp.cost = graph[*ei].cost;

                auto start_node = graph.graph[source(*ei, graph.graph)].id;
                auto end_node = graph.graph[target(*ei, graph.graph)].id;
                auto v_sn(graph.get_V(start_node));
                auto v_en(graph.get_V(end_node));

                auto edge_id =
                    graph.get_edge_id(v_sn, v_en, tmp.cost);
                totalcost += tmp.cost;

                tmp.edge = edge_id;       // edge_id
                tmp.tree_cost = totalcost;    // tree_cost
                results.push_back(tmp);
            }
        }
    }
    return results;
}

template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::operator() (G &graph) {
    return generateKruskal(graph);
}


}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_KRUSKAL_HPP_
