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
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "mst/pgr_mst.hpp"
#include "mst/visitors.hpp"
#include "mst/details.hpp"

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_kruskal : public Pgr_mst<G> {
 public:
     std::vector<pgr_mst_rt> kruskal(G &graph);

     std::vector<pgr_mst_rt> kruskalBFS(
             G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth);

     std::vector<pgr_mst_rt> kruskalDFS(
             G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth);

     std::vector<pgr_mst_rt> kruskalDD(
             G &graph,
             std::vector<int64_t> roots,
             double distance);

 private:
     typedef typename G::B_G B_G;
     typedef typename G::V V;
     typedef typename G::E E;

     /* Does all the work */
     void generateKruskal(G &graph);

};


template <class G>
void
Pgr_kruskal<G>::generateKruskal(G &graph) {
    this->m_spanning_tree.clear();
    this->m_components.clear();
    this->m_results.clear();
    this->m_tree_id.clear();

    boost::kruskal_minimum_spanning_tree(
            graph.graph,
            std::inserter(this->m_spanning_tree.edges, this->m_spanning_tree.edges.begin()),
            boost::weight_map(get(&G::G_T_E::cost, graph.graph)));
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_kruskal<G>::kruskal(
        G &graph) {
    this->mst();
    generateKruskal(graph);
    return this->no_order(graph);
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_kruskal<G>::kruskalBFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    this->mstBFS(roots, max_depth);
    generateKruskal(graph);
    return this->bfs_order(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_kruskal<G>::kruskalDFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    this->mstDFS(roots, max_depth);
    generateKruskal(graph);
    return this->dfs_order(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_kruskal<G>::kruskalDD(
        G &graph,
        std::vector<int64_t> roots,
        double distance) {
    this->mstDD(roots, distance);
    generateKruskal(graph);
    return this->dfs_order(graph);
}


}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_KRUSKAL_HPP_
