/*PGR-GNU*****************************************************************
File: pgr_breadthFirstSearch.hpp

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2019 Gudesa Venkata Sai AKhil
Mail: gvs.akhil1997@gmail.com

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

#ifndef INCLUDE_BREADTHFIRSTSEARCH_PGR_BREADTHFIRSTSEARCH_HPP_
#define INCLUDE_BREADTHFIRSTSEARCH_PGR_BREADTHFIRSTSEARCH_HPP_
#pragma once


#include <visitors/edges_order_bfs_visitor.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <vector>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
//******************************************

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_breadthFirstSearch {
 public:
    typedef typename G::V V;
    typedef typename G::E E;
    typedef typename G::B_G B_G;


    std::vector<pgr_mst_rt> breadthFirstSearch(
        G &graph,
        std::vector<int64_t> start_vertex,
        int64_t depth) {
        std::vector<pgr_mst_rt> results;
        using bfs_visitor = visitors::Edges_order_bfs_visitor<E>;

        for (auto source : start_vertex) {
            std::vector<E> visited_order;

            if (graph.has_vertex(source)) {
                results.push_back({source, 0, source, -1, 0.0, 0.0});
                boost::breadth_first_search(graph.graph,
                                            graph.get_V(source),
                                            visitor(bfs_visitor(visited_order)));

                auto single_source_results = get_results(visited_order, source, depth, graph);
                results.insert(results.end(), single_source_results.begin(), single_source_results.end());

                /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
                CHECK_FOR_INTERRUPTS();
            }
        }
        return results;
        }

 private:
     template <typename T>
     std::vector<pgr_mst_rt> get_results(
             T order,
             int64_t source,
             int64_t max_depth,
             const G &graph) {
         std::vector<pgr_mst_rt> results;

         std::vector<double> agg_cost(graph.num_vertices(), 0);
         std::vector<int64_t> depth(graph.num_vertices(), 0);

         for (const auto edge : order) {
             auto u = graph.source(edge);
             auto v = graph.target(edge);

             agg_cost[v] = agg_cost[u] + graph[edge].cost;
             depth[v] = depth[u] + 1;

             if (max_depth >= depth[v]) {
                 results.push_back({
                     source,
                         depth[v],
                         graph[v].id,
                         graph[edge].id,
                         graph[edge].cost,
                         agg_cost[v]
                 });
             }
         }
         return results;
     }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_BREADTHFIRSTSEARCH_PGR_BREADTHFIRSTSEARCH_HPP_
