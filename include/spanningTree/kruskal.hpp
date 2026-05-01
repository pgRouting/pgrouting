/*PGR-GNU*****************************************************************
File: kruskal.hpp

Copyright (c) 2007-2026 pgRouting developers
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

#ifndef INCLUDE_SPANNINGTREE_KRUSKAL_HPP_
#define INCLUDE_SPANNINGTREE_KRUSKAL_HPP_
#pragma once

#include <vector>
#include <set>
#include <cstdint>

#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include "spanningTree/mst.hpp"
#include "cpp_common/interruption.hpp"

namespace pgrouting {
namespace algorithms {

template <class G>
class Pgr_kruskal : public Pgr_mst<G> {
 public:
     ~Pgr_kruskal() override = default;
     std::vector<MST_rt> kruskal(G &graph) {
         return this->mst(graph);
     }

     std::vector<MST_rt> kruskalBFS(
             G &graph,
             const std::set<int64_t>& roots,
             int64_t max_depth) {
         return this->mstBFS(graph, roots, max_depth);
     }

     std::vector<MST_rt> kruskalDFS(
             G &graph,
             const std::set<int64_t>& roots,
             int64_t max_depth) {
         return this->mstDFS(graph, roots, max_depth);
     }

     std::vector<MST_rt> kruskalDD(
             G &graph,
             const std::set<int64_t>& roots,
             double distance) {
         return this->mstDD(graph, roots, distance);
     }

 private:
     typedef typename G::B_G B_G;
     typedef typename G::V V;
     typedef typename G::E E;

     /* Does all the work */
     void generate_mst(const G &graph) override {
         this->clear();
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         boost::kruskal_minimum_spanning_tree(
                 graph.graph,
                 std::inserter(this->m_spanning_tree.edges, this->m_spanning_tree.edges.begin()),
                 boost::weight_map(get(&G::G_T_E::cost, graph.graph)));
     }
};

}  // namespace algorithms

namespace functions {

std::vector<MST_rt>
kruskal(pgrouting::UndirectedGraph&);

std::vector<MST_rt>
kruskalBFS(pgrouting::UndirectedGraph&, const std::set<int64_t>&, int64_t);

std::vector<MST_rt>
kruskalDFS(pgrouting::UndirectedGraph&, const std::set<int64_t>&, int64_t);

std::vector<MST_rt>
kruskalDD(pgrouting::UndirectedGraph&, const std::set<int64_t>&, double);

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_SPANNINGTREE_KRUSKAL_HPP_
