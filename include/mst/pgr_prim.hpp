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
#include <boost/graph/filtered_graph.hpp>

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
#include "mst/pgr_mst.hpp"


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

}

namespace functions {

template <class G>
class Pgr_prim : public Pgr_mst<G> {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
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
     std::set<V> m_unassigned;
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
    for (auto r : this->m_roots) {
        auto result = generatePrim(graph, r);
        this->m_results.insert(this->m_results.end(), result.begin(), result.end());
    }
    return this->m_results;
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

    for (const auto v : data) {
        /*
         * its not a tree, its a forest
         * - v is not on current tree
         */
        if (isinf(distances[v])) continue;
        m_unassigned.erase(v);


        auto u = predecessors[v];

        /*
         * Not a valid edge
         */
        if (u == v) continue;

        auto cost = distances[u] - distances[v];
        auto edge = graph.get_edge(u, v, cost);
        this->m_added_order.push_back(edge);
    }
    return std::vector<pgr_mst_rt>();
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::disconnectedPrim(const G &graph) {
    size_t totalNodes = num_vertices(graph.graph);

    m_unassigned.clear();
    for (V v = 0; v < totalNodes; ++v) {
            m_unassigned.insert(m_unassigned.end(), v);
    }

    std::vector<pgr_mst_rt> results;
    while (!m_unassigned.empty()) {
        auto root = *m_unassigned.begin();
        m_unassigned.erase(m_unassigned.begin());
        auto tmpresults = generatePrim(
                graph,
                graph.graph[root].id);
        results.insert(results.end(), tmpresults.begin(), tmpresults.end());
    }

    this->m_results = this->order_results(graph);
    return this->m_results;
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::prim(
        G &graph) {
    this->mst();
#if 0
    this->m_suffix = "";
    this->m_order_by = 0;
    this->m_get_component = false;
    this->m_distance = -1;
    this->m_max_depth = -1;
    this->m_roots.clear();
#endif
    return disconnectedPrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primBFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
        this->mstBFS(roots, max_depth);
#if 0
    this->m_suffix = "BFS";
    this->m_order_by = 2;
    this->m_get_component = true;
    this->m_distance = -1;
    this->m_max_depth = max_depth;
    this->m_roots = details::clean_vids(roots);
#endif

    return this->m_roots.empty()? disconnectedPrim(graph)
        : generatePrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    this->mstDFS(roots, max_depth);
#if 0
    this->m_suffix = "DFS";
    this->m_order_by = 1;
    this->m_get_component = false;
    this->m_distance = -1;
    this->m_max_depth = max_depth;
    this->m_roots = details::clean_vids(roots);
#endif

    return this->m_roots.empty()? disconnectedPrim(graph)
        : generatePrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDD(
        G &graph,
        std::vector<int64_t> roots,
        double distance) {
    this->mstDD(roots, distance);
#if 0
    this->m_suffix = "DD";
    this->m_order_by = 1;
    this->m_get_component = false;
    this->m_distance = distance;
    this->m_max_depth = -1;
    this->m_roots = details::clean_vids(roots);
#endif

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
