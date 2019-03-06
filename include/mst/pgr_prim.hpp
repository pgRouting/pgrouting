/*PGR-GNU*****************************************************************
File: pgr_prim.hpp

Copyright (c) 2018 pgRouting developers
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

#include <visitors/prim_dijkstra_visitor.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <vector>
#include <set>

#include "mst/pgr_mst.hpp"

//******************************************

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_prim : public Pgr_mst<G> {
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::B_G B_G;

 public:
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

     void primTree(
             const G &graph,
             int64_t root_vertex);

     void generate_mst(const G &graph);

 private:
     // Member
     std::vector<V> predecessors;
     std::vector<double> distances;
     std::vector<V> data;
     std::set<V> m_unassigned;
};


template <class G>
void
Pgr_prim<G>::primTree(
        const G &graph,
        int64_t root_vertex) {
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

    auto v_root(graph.get_V(root_vertex));

    using prim_visitor = visitors::Prim_dijkstra_visitor<V>;
    boost::prim_minimum_spanning_tree(
            graph.graph,
            &predecessors[0],
            boost::distance_map(&distances[0]).
            weight_map(get(&G::G_T_E::cost, graph.graph))
            .root_vertex(v_root)
            .visitor(prim_visitor(data)));

    for (const auto v : data) {
        /*
         * its not a tree, its a forest
         * - v is not on current tree
         */
        if (std::isinf(distances[v])) continue;
        m_unassigned.erase(v);


        auto u = predecessors[v];

        /*
         * Not a valid edge
         */
        if (u == v) continue;

        auto cost = distances[u] - distances[v];
        auto edge = graph.get_edge(u, v, cost);
        this->m_spanning_tree.edges.insert(edge);
    }
}


template <class G>
void
Pgr_prim<G>::generate_mst(const G &graph) {
    this->clear();

    size_t totalNodes = num_vertices(graph.graph);

    m_unassigned.clear();
    for (V v = 0; v < totalNodes; ++v) {
            m_unassigned.insert(m_unassigned.end(), v);
    }

    while (!m_unassigned.empty()) {
        auto root = *m_unassigned.begin();
        m_unassigned.erase(m_unassigned.begin());
        primTree(
                graph,
                graph.graph[root].id);
    }
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::prim(
        G &graph) {
    return this->mst(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primBFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    return this->mstBFS(graph, roots, max_depth);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    return this->mstDFS(graph, roots, max_depth);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDD(
        G &graph,
        std::vector<int64_t> roots,
        double distance) {
    return this->mstDD(graph, roots, distance);
}



}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_PRIM_HPP_
