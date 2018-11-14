/*PGR-GNU*****************************************************************
File: pgr_prim.hpp

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

#ifndef INCLUDE_MST_PGR_PRIM_HPP_
#define INCLUDE_MST_PGR_PRIM_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_assert.h"

#include "mst/visitors.hpp"
#include "mst/details.hpp"


//******************************************

namespace pgrouting {
namespace {

template <class V>
double prim_aggegrateCost(
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        V root_vertex,
        V find_node) {
    double agg_cost = 0;

    for (V i = find_node; i != root_vertex;) {
        auto parent =  predecessors[i];
        auto cost = distances[i] - distances[root_vertex];

        agg_cost += cost;
        i = parent;
    }
    return agg_cost;
}

}  // namespace visitors

namespace functions {

template <class G>
class Pgr_prim {
 public:
     typedef typename G::V V;
     typedef typename G::B_G B_G;

     std::vector<pgr_mst_rt> operator() (
                 G &graph,
                 int64_t root_vertex,
                 bool use_root);

     std::vector<pgr_mst_rt> prim(G &graph);

     std::vector<pgr_mst_rt> primBFS(
             G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth);

     std::vector<pgr_mst_rt> primDFS(
             G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth);

     std::vector<pgr_mst_rt> primDD(
             G &graph,
             std::vector<int64_t> roots,
             double distance);


 private:
     // Functions
     void clear() {
         data.clear();
         predecessors.clear();
         distances.clear();
     }

     void resize(const G &graph);

     std::vector<pgr_mst_rt> generatePrim(
             const G &graph,
             int64_t root_vertex );

     std::vector<pgr_mst_rt> generatePrim(
             const G &graph);

     std::vector< pgr_mst_rt > disconnectedPrim(const G &graph);

 private:
     // Member
     std::vector<V> predecessors;
     std::vector<double> distances;
     std::vector<V> data;

     std::string m_suffix;
     bool m_get_component;
     int  m_order_by;
     std::vector<int64_t> m_roots;
     int64_t  m_max_depth;
     double  m_distance;
     std::vector<pgr_mst_rt> m_results;
};


template <class G>
void
Pgr_prim<G>::resize(const G &graph) {
    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::generatePrim(
        const G &graph) {
    for (auto r : m_roots) {
        auto result = generatePrim(graph, r);
        m_results.insert(m_results.end(), result.begin(), result.end());
    }
    return m_results;
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::generatePrim(
        const G &graph,
        int64_t root_vertex ) {
    clear();
    resize(graph);
    auto v_root(graph.get_V(root_vertex));

    using prim_visitor = visitors::Prim_visitor<V>;
    boost::prim_minimum_spanning_tree(
            graph.graph,
            &predecessors[0],
            boost::distance_map(&distances[0]).
            weight_map(get(&G::G_T_E::cost, graph.graph))
            .root_vertex(v_root)
            .visitor(prim_visitor(data)));

    std::vector<pgr_mst_rt> results;
    double totalcost = 0;

    for (const auto v : data) {
        if (v == data[0]) {
            results.push_back({root_vertex, -2, root_vertex, -1, 0, totalcost});
            continue;
        }

        auto node = graph.graph[v].id;
        auto v_sn(graph.get_V(graph.graph[predecessors[v]].id));
        auto cost = distances[v_sn] - distances[v];
        auto edge_id = graph.get_edge_id(v_sn, v, cost);
        totalcost += cost;

        results.push_back({
            root_vertex,
            -2,
            node,
            edge_id,
            cost,
            prim_aggegrateCost(predecessors, distances, v_root, v)});
    }
    return results;
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::disconnectedPrim(const G &graph) {
    size_t totalNodes = num_vertices(graph.graph);


    /*Calculate connected components*/
    std::vector<size_t> components(totalNodes);
    auto num_comps =
        boost::connected_components(graph.graph, &components[0]);

    std::vector<std::vector<int64_t>> component;
    component.resize(num_comps);

    for (auto v : boost::make_iterator_range(vertices(graph.graph))) {
        component[components[v]].push_back(v);
    }

    std::vector<pgr_mst_rt> results;
    for (const auto c : component) {
        /* Implementation */
        auto tmpresults = generatePrim(
                graph,
                graph.graph[c[0]].id);
        results.insert(results.end(), tmpresults.begin(), tmpresults.end());
    }
    return results;
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::prim(
        G &graph) {
    m_suffix = "";
    m_order_by = 0;
    m_get_component = false;
    m_distance = -1;
    m_max_depth = -1;
    m_roots.clear();

    return disconnectedPrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primBFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    m_suffix = "BFS";
    m_order_by = 2;
    m_get_component = true;
    m_distance = -1;
    m_max_depth = max_depth;
    m_roots = details::clean_vids(roots);

    return generatePrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    m_suffix = "DFS";
    m_order_by = 1;
    m_get_component = false;
    m_distance = -1;
    m_max_depth = max_depth;
    m_roots = details::clean_vids(roots);

    return generatePrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDD(
        G &graph,
        std::vector<int64_t> roots,
        double distance) {
    m_suffix = "DD";
    m_order_by = 1;
    m_get_component = false;
    m_distance = distance;
    m_max_depth = -1;
    m_roots = details::clean_vids(roots);

    return generatePrim(graph);
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::operator() (
        G &graph,
        int64_t root_vertex,
        bool use_root) {
    if (!use_root) {
        return disconnectedPrim(graph);
    }

    if (!graph.has_vertex(root_vertex)) {
        return std::vector<pgr_mst_rt>();
    }

    return generatePrim(
            graph,
            root_vertex);
}

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_PRIM_HPP_
