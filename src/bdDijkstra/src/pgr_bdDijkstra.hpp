/*PGR-GNU*****************************************************************

File: pgr_bdDijkstra.hpp 

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

#ifndef SRC_BDDIJKSTRA_SRC_PGR_BDDIJKSTRA_HPP_
#define SRC_BDDIJKSTRA_SRC_PGR_BDDIJKSTRA_HPP_
#pragma once

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <limits>
#include <functional>


#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/pgr_base_graph.hpp"




template < typename G >
class Pgr_bdDijkstra {
     typedef typename G::V V;
     typedef typename G::E E;

     typedef std::pair<double, V> Cost_Vertex_pair;
     typedef typename std::priority_queue<
         Cost_Vertex_pair,
         std::vector<Cost_Vertex_pair>,
         std::greater<Cost_Vertex_pair> > Priority_queue;


 public:
     explicit Pgr_bdDijkstra(G &pgraph):
         graph(pgraph),
         INF(std::numeric_limits<double>::max()) {
             m_log << "constructor\n";
         };

     ~Pgr_bdDijkstra() = default;


     Path pgr_bdDijkstra(V start_vertex, V end_vertex, bool only_cost) {
         m_log << "pgr_bdDijkstra\n";
         v_source = start_vertex;
         v_target = end_vertex;

         if (v_source == v_target) {
            return Path(v_source, v_target);
         }
         return bidir_dijkstra(only_cost);
     }

     std::string log() const {return m_log.str();}
     void clean_log() {log.clear();}
     void clear() {
         while (!forward_queue.empty()) forward_queue.pop();
         while (!backward_queue.empty()) backward_queue.pop();

         backward_finished.clear();
         backward_edge.clear();
         backward_predecessor.clear();
         backward_cost.clear();

         forward_finished.clear();
         forward_edge.clear();
         forward_predecessor.clear();
         forward_cost.clear();
     }


 private:
     void initialize() {
         m_log << "initializing\n";
         clear();
         forward_predecessor.resize(graph.num_vertices());
         forward_finished.resize(graph.num_vertices(), false);
         forward_edge.resize(graph.num_vertices(), -1);
         forward_cost.resize(graph.num_vertices(), INF);
         std::iota(forward_predecessor.begin(), forward_predecessor.end(), 0);

         backward_predecessor.resize(graph.num_vertices());
         backward_finished.resize(graph.num_vertices(), false);
         backward_edge.resize(graph.num_vertices(), -1);
         backward_cost.resize(graph.num_vertices(), INF);
         std::iota(backward_predecessor.begin(), backward_predecessor.end(), 0);

         v_min_node = -1;
         best_cost = INF;
     }

     Path bidir_dijkstra(bool only_cost) {
         m_log << "bidir_dijkstra\n";

         Pgr_bdDijkstra< G >::initialize();

         forward_cost[v_source] = 0;
         forward_queue.push(std::make_pair(0.0, v_source));

         backward_cost[v_target] = 0;
         backward_queue.push(std::make_pair(0.0, v_target));

         while (!forward_queue.empty() &&  !backward_queue.empty()) {
             auto forward_node = forward_queue.top();
             auto backward_node = backward_queue.top();
             /*
              *  done: there is no path with lower cost
              */
             if (forward_node.first == INF || backward_node.first == INF) {
                 break;
             }

             /*
              * Explore from the cheapest side
              */
             if (backward_node.first < forward_node.first) {
                 backward_queue.pop();
                 if (!backward_finished[backward_node.second]) {
                     explore_backward(backward_node);
                 }
                 if (found(backward_node.second)) {
                     break;
                 }
             } else {
                 forward_queue.pop();
                 if (!forward_finished[forward_node.second]) {
                     explore_forward(forward_node);
                 }
                 if (found(forward_node.second)) {
                     break;
                 }
             }
         }

         if (best_cost == INF) return Path();

         Path forward_path(
                 graph,
                 v_source,
                 v_min_node,
                 forward_predecessor,
                 forward_cost,
                 only_cost,
                 true);
         Path backward_path(
                 graph,
                 v_target,
                 v_min_node,
                 backward_predecessor,
                 backward_cost,
                 only_cost,
                 false);
         m_log << forward_path;
         backward_path.reverse();
         m_log << backward_path;
         forward_path.append(backward_path);
         m_log << forward_path;
         return forward_path;
     }



     bool found(const V &node) {
         /*
          * Update common node
          */
         if (forward_finished[node] && backward_finished[node]) {
             if (best_cost >= forward_cost[node] + backward_cost[node]) {
                 v_min_node = node;
                 best_cost =  forward_cost[node] + backward_cost[node];
                 return false;
             } else {
                 return true;
             }
         }
         return false;
     }

     void explore_forward(const Cost_Vertex_pair &node) {
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
             }
         }
         forward_finished[current_node] = true;
     }

     void explore_backward(const Cost_Vertex_pair &node) {
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
             }
         }
         backward_finished[current_node] = true;
     }


 private:
     G &graph;
     V v_source;  //!< source descriptor
     V v_target;  //!< target descriptor
     V v_min_node;  //!< target descriptor

     double INF;  //!< infinity

     mutable std::ostringstream m_log;
     Priority_queue forward_queue;
     Priority_queue backward_queue;

     double best_cost;
     bool cost_only;

     std::vector<bool> backward_finished;
     std::vector<int64_t> backward_edge;
     std::vector<V> backward_predecessor;
     std::vector<double> backward_cost;

     std::vector<bool> forward_finished;
     std::vector<int64_t> forward_edge;
     std::vector<V> forward_predecessor;
     std::vector<double> forward_cost;
};

#endif  // SRC_BDDIJKSTRA_SRC_PGR_BDDIJKSTRA_HPP_
