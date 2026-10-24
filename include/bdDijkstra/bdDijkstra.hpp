/*PGR-GNU*****************************************************************
File: bdDijkstra.hpp

Copyright (c) 2016-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef INCLUDE_BDDIJKSTRA_BDDIJKSTRA_HPP_
#define INCLUDE_BDDIJKSTRA_BDDIJKSTRA_HPP_
#pragma once

#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <limits>
#include <functional>
#include <set>
#include <map>
#include <deque>


#include "cpp_common/bidirectional.hpp"
#include "cpp_common/path.hpp"


namespace pgrouting {
namespace bidirectional {


template < typename G >
class Pgr_bdDijkstra : public Pgr_bidirectional<G> {
    typedef typename Pgr_bidirectional<G>::V V;
    typedef typename Pgr_bidirectional<G>::E E;
    typedef typename Pgr_bidirectional<G>::Cost_Vertex_pair Cost_Vertex_pair;

    using Pgr_bidirectional<G>::graph;
    using Pgr_bidirectional<G>::m_log;
    using Pgr_bidirectional<G>::v_source;
    using Pgr_bidirectional<G>::v_target;
    using Pgr_bidirectional<G>::best_cost;
    using Pgr_bidirectional<G>::INF;

    using Pgr_bidirectional<G>::backward_predecessor;
    using Pgr_bidirectional<G>::backward_queue;
    using Pgr_bidirectional<G>::backward_finished;
    using Pgr_bidirectional<G>::backward_cost;
    using Pgr_bidirectional<G>::backward_edge;

    using Pgr_bidirectional<G>::forward_predecessor;
    using Pgr_bidirectional<G>::forward_queue;
    using Pgr_bidirectional<G>::forward_finished;
    using Pgr_bidirectional<G>::forward_cost;
    using Pgr_bidirectional<G>::forward_edge;


    using Pgr_bidirectional<G>::bidirectional;
    using Pgr_bidirectional<G>::record_meeting;

 public:
     explicit Pgr_bdDijkstra(G &pgraph):
         Pgr_bidirectional<G>(pgraph) {
             m_log << "pgr_bdDijkstra constructor\n";
         }

     ~Pgr_bdDijkstra() override = default;

     Path pgr_bdDijkstra(V start_vertex, V end_vertex, bool only_cost) {
         m_log << "pgr_bdDijkstra\n";
         v_source = start_vertex;
         v_target = end_vertex;

         return bidirectional(only_cost);
     }


    using Pgr_bidirectional<G>::log;
    using Pgr_bidirectional<G>::clean_log;


 private:
     /*
      * Decides only when to stop. best_cost and v_min_node are kept up to
      * date by record_meeting as the costs are written, so there is
      * nothing to record here.
      *
      * A cheaper route than the one already recorded would have to leave
      * the forward side through a node still waiting in the forward queue
      * and reach the backward side through a node still waiting in the
      * backward queue. Such a route costs at least the cheapest cost
      * waiting on each side added together. Once that sum reaches
      * best_cost, nothing cheaper is left to find.
      */
     bool found(const V &) override {
         if (forward_queue.empty() || backward_queue.empty()) return false;
         if (best_cost >= INF) return false;

         auto forward_top = forward_queue.top().first;
         auto backward_top = backward_queue.top().first;

         if (forward_top >= INF || backward_top >= INF) return false;

         return forward_top + backward_top >= best_cost;
     }

     void explore_forward(const Cost_Vertex_pair &node) override {
         typename G::EO_i out, out_end;

         auto current_cost = node.first;
         auto current_node = node.second;

         for (boost::tie(out, out_end) = out_edges(current_node, graph.graph);
                 out != out_end; ++out) {
             auto edge_cost = graph[*out].cost;
             auto next_node = graph.adjacent(current_node, *out);

             if (forward_finished[next_node]) continue;

             if (edge_cost + current_cost < forward_cost[next_node]) {
                 forward_cost[next_node] = edge_cost + current_cost;
                 forward_predecessor[next_node] = current_node;
                 forward_edge[next_node] = graph[*out].id;
                 forward_queue.push({forward_cost[next_node], next_node});
                 record_meeting(next_node);
             }
         }
         forward_finished[current_node] = true;
     }

     void explore_backward(const Cost_Vertex_pair &node) override {
         typename G::EI_i in, in_end;

         auto current_cost = node.first;
         auto current_node = node.second;

         for (boost::tie(in, in_end) = in_edges(current_node, graph.graph);
                 in != in_end; ++in) {
             auto edge_cost = graph[*in].cost;
             auto next_node = graph.adjacent(current_node, *in);

             if (backward_finished[next_node]) continue;

             if (edge_cost + current_cost < backward_cost[next_node]) {
                 backward_cost[next_node] = edge_cost + current_cost;
                 backward_predecessor[next_node] = current_node;
                 backward_edge[next_node] = graph[*in].id;
                 backward_queue.push({backward_cost[next_node], next_node});
                 record_meeting(next_node);
             }
         }
         backward_finished[current_node] = true;
     }
};

}  // namespace bidirectional

namespace algorithms {

template <class G>
std::deque<pgrouting::Path>
bdDijkstra(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        bool only_cost) {
    using pgrouting::Path;

    pgrouting::bidirectional::Pgr_bdDijkstra<G> fn_bdDijkstra(graph);
    std::deque<Path> paths;

    for (const auto &comb : combinations) {
        auto source = comb.first;
        if (!graph.has_vertex(source)) continue;

        for (const auto &target : comb.second) {
            if (!graph.has_vertex(target)) continue;
            fn_bdDijkstra.clear();

            paths.push_back(fn_bdDijkstra.pgr_bdDijkstra(graph.get_V(source), graph.get_V(target), only_cost));
        }
    }
    return paths;
}

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_BDDIJKSTRA_BDDIJKSTRA_HPP_
