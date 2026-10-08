/*PGR-GNU*****************************************************************
File: yen.hpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2026 Celia Virginia Vergara Castillo
Mail: vicky AT erosion.dev

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more yen.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#ifndef INCLUDE_YEN_YEN_HPP_
#define INCLUDE_YEN_YEN_HPP_
#pragma once

#include <algorithm>
#include <cstdint>
#include <deque>
#include <map>
#include <memory>
#include <set>

#include "cpp_common/assert.hpp"
#include "cpp_common/compPaths.hpp"
#include "cpp_common/filteredGraph.hpp"
#include "cpp_common/messages.hpp"
#include "cpp_common/path.hpp"

#include "dijkstra/dijkstra.hpp"

namespace pgrouting {
namespace yen {

/** @brief Yen's K shortest paths
 *
 * The algorithm runs over a filtered view of the graphi
 *
 * Used by pgr_ksp and pgr_withPointsKSP (via algorithms::Yen) and as
 * the base of Pgr_turnRestrictedPath. The public entry point is run() rather
 * than Yen(), because a member function cannot share its class's name.
 *
 * @tparam G the Pgr_base_graph being searched
 */
template <class G>
class Yen : public Pgr_messages {
    using V = typename G::V;
    using pSet = std::set<Path, compPathsLess>;
    using FilteredGraph_t = pgrouting::graph::FilteredGraph<G>;

 public:
    Yen() :
         m_vis(std::make_unique<Visitor>()) {
         }

     ~Yen() = default;

     Yen(const Yen&) = delete;
     Yen& operator=(const Yen&) = delete;
     Yen(Yen&&) = delete;
     Yen& operator=(Yen&&) = delete;

     /** @brief Calculates the K shortest paths between two vertices
      *
      * @param [in] graph       the graph to search
      * @param [in] start_vertex source vertex identifier
      * @param [in] end_vertex   destination vertex identifier
      * @param [in] K            how many paths
      * @param [in] heap_paths   when true, also returns the candidates that did
      *                          not make it into the result set
      * @returns the paths found
      */
     std::deque<Path> run(
             G &graph,
             int64_t  start_vertex,
             int64_t end_vertex,
             size_t K,
             bool heap_paths) {
         /*
          * No path: already in destination
          */
         if ((start_vertex == end_vertex) || (K == 0)) {
             return std::deque<Path>();
         }

         /*
          * no path: disconnected vertices
          */
         if (!graph.has_vertex(start_vertex)
                 || !graph.has_vertex(end_vertex)) {
             return std::deque<Path>();
         }

         /*
          * clean up containers
          */
         clear();


         v_source = graph.get_V(start_vertex);
         v_target = graph.get_V(end_vertex);
         m_start = start_vertex;
         m_end = end_vertex;
         m_K = K;
         m_heap_paths = heap_paths;


         executeYen(graph);

         auto paths = get_results();
         if (!m_heap_paths && paths.size() > m_K) paths.resize(m_K);

         return paths;
     }

     void clear() {
         m_Heap.clear();
         m_ResultSet.clear();
     }



     class Visitor {
      public:
          virtual ~Visitor() = default;

          Visitor() = default;
          Visitor(const Visitor&) = delete;
          Visitor& operator=(const Visitor&) = delete;
          Visitor(Visitor&&) = delete;
          Visitor& operator=(Visitor&&) = delete;

         virtual void on_insert_first_solution(const Path) const {
             /* noop */
         }

         virtual void on_insert_to_heap(const Path) const {
             /* noop */
         }
     };

 protected:
     //! the actual algorithm
     void executeYen(G &graph) {
         clear();
         /* One view for the whole run. The filter starts empty, is filled and
          * emptied once per spur node, and the underlying graph is never
          * touched. */
         FilteredGraph_t filtered(graph);

         curr_result_path = getFirstSolution(filtered);
         m_vis->on_insert_first_solution(curr_result_path);

         if (m_ResultSet.size() == 0) return;  // no path found

         while (m_ResultSet.size() <  m_K) {
             doNextCycle(filtered);
             if (m_Heap.empty()) break;
             curr_result_path = *m_Heap.begin();
             curr_result_path.recalculate_agg_cost();
             m_ResultSet.insert(curr_result_path);
             m_Heap.erase(m_Heap.begin());
         }
     }

     //! Performs the first Dijkstra of the algorithm
      Path getFirstSolution(FilteredGraph_t &graph) {
         Path path;

         path = algorithms::dijkstra(graph, m_start, m_end);
         path.recalculate_agg_cost();

         if (path.empty()) return path;
         m_ResultSet.insert(path);
         return path;
     }

     //! Performs the next cycle of the algorithm
     void doNextCycle(FilteredGraph_t &graph) {
         for (unsigned int i = 0; i < curr_result_path.size(); ++i) {
             int64_t spurNodeId = curr_result_path[i].node;

             auto rootPath = curr_result_path.getSubpath(i);

             for (const auto &path : m_ResultSet) {
                 if (path.isEqual(rootPath) && spurNodeId == path[i].node) {
                     if (path.size() > i + 1 && path[i].edge != -1) {
                         /* Suppress only this path's edge, by edge id, so the
                          * parallel edges between the same pair stay available
                          * to the spur search. */
                         graph.remove_edge_id(path[i].edge);
                     }
                 }
             }

             removeVertices(graph, rootPath);

             auto spurPath = algorithms::dijkstra(graph, spurNodeId, m_end);

             if (spurPath.size() > 0) {
                 rootPath.appendPath(spurPath);
                 m_Heap.insert(rootPath);
                 m_vis->on_insert_to_heap(rootPath);
             }

             graph.clear_filter();
         }
     }

     //! stores in subPath the first i elements of path
     void removeVertices(FilteredGraph_t &graph, const Path &subpath) {
         for (const auto &e : subpath) graph.remove_vertex(e.node);
     }

     std::deque<Path> get_results() {
         if (this->m_ResultSet.empty()) {
             return std::deque<Path>();
         }

         std::deque<Path> paths(m_ResultSet.begin(), m_ResultSet.end());

         if (m_heap_paths && !m_Heap.empty()) {
            paths.insert(paths.end(), m_Heap.begin(), m_Heap.end());
         }
         pgassert(!paths.empty());

         std::sort(paths.begin(), paths.end(), compPathsLess());

         return paths;
     }

     V v_source;  //!< source descriptor
     V v_target;  //!< target descriptor
     int64_t m_start{0};  //!< source id
     int64_t m_end{0};   //!< target id
     size_t m_K{0};
     bool m_heap_paths{false};

     Path curr_result_path;  //!< storage for the current result

     pSet m_ResultSet;  //!< ordered set of shortest paths
     pSet m_Heap;  //!< the heap

     std::unique_ptr<Visitor> m_vis;
};


}  // namespace yen

namespace algorithms {

/** @brief Yen's algorithm over a filtered graph, for every (source, target) pair
 *
 * @param [in] graph        the graph to search
 * @param [in] combinations the (source, target) pairs
 * @param [in] k            how many paths per pair
 * @param [in] heap_paths   also return the candidates not selected
 * @returns the paths found
 */
template <class G>
std::deque<Path> Yen(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        size_t k,
        bool heap_paths) {
    std::deque<Path> paths;
    pgrouting::yen::Yen<G> fn_yen;

    for (const auto &c : combinations) {
        if (!graph.has_vertex(c.first)) continue;

        for (const auto &destination : c.second) {
            if (!graph.has_vertex(destination)) continue;

            fn_yen.clear();
            auto result_path = fn_yen.run(graph, c.first, destination, k, heap_paths);
            paths.insert(paths.end(), result_path.begin(), result_path.end());
        }
    }

    return paths;
}

}  // namespace algorithms

}  // namespace pgrouting

#endif  // INCLUDE_YEN_YEN_HPP_
