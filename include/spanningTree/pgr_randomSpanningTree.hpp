/*PGR-GNU*****************************************************************
File: pgr_randomSpanningTree.hpp

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

#ifndef INCLUDE_SPANNINGTREE_PGR_RANDOMSPANNINGTREE_HPP_
#define INCLUDE_SPANNINGTREE_PGR_RANDOMSPANNINGTREE_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random_spanning_tree.hpp>
#include <boost/random/random_number_generator.hpp>
#include <boost/graph/graph_traits.hpp>

#include <vector>
#include <random>
#include <iostream>
#include <exception>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"

template < class G > class Pgr_randomSpanningTree;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_randomSpanningTree {
 public:
     typedef typename G::V V;
     typedef typename G::E E;

     std::vector<pgr_randomSpanningTree_t> randomSpanningTree(
                 G &graph,
                 int64_t root_vertex);

 private:
      // Member
      std::vector< V > predecessors;

      // Function
      std::vector< pgr_randomSpanningTree_t >
      undirectedGraph(
           const G &graph,
               int64_t root_vertex) {
         std::ostringstream log;
         auto v_root(graph.get_V(root_vertex));

         std::minstd_rand rng;

          /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
          CHECK_FOR_INTERRUPTS();

         // TODO(aps)
         // This function is running in infinite loop
         try {
             boost::random_spanning_tree(
                     graph.graph,
                     rng,
                     boost::root_vertex(v_root)
                     .predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph)));
         } catch (std::exception const &ex) {
             log << ex.what();
         } catch (...) {
             log << "Unknown exception caught";
         }

         std::vector< pgr_randomSpanningTree_t > resul;
         return resul;
         std::vector< pgr_randomSpanningTree_t > results;
         double totalcost = 0;
         pgr_randomSpanningTree_t tmp;

         tmp.root_vertex = root_vertex;
         tmp.edge = -1;
         tmp.cost = 0;
         tmp.tree_cost = totalcost;

         results.push_back(tmp);

         for (size_t j = 0; j < predecessors.size(); j++) {
           if (j != v_root) {
             pgr_randomSpanningTree_t tmp;

             auto start_node = graph.graph[predecessors[j]].id;
             auto end_node = graph.graph[j].id;  // node

             auto v_sn(graph.get_V(start_node));
             auto v_en(graph.get_V(end_node));

             auto cost = graph[boost::edge(
                     predecessors[j], j, graph.graph).first].cost;
             auto edge_id =
                 graph.get_edge_id(v_sn, v_en, cost);
         totalcost += cost;

             tmp.root_vertex = root_vertex;
             tmp.edge = edge_id;         // edge_id
             tmp.cost = cost;            // cost
             tmp.tree_cost = totalcost;      // tree_cost
             results.push_back(tmp);
           }
         }
         return results;
    }
};

template < class G >
std::vector<pgr_randomSpanningTree_t>
Pgr_randomSpanningTree< G >::randomSpanningTree(
                G &graph,
                int64_t root_vertex ) {
      predecessors.clear();
      // TODO(aps)
      // Currently only running for undirected graph
      return undirectedGraph(
              graph,
              root_vertex);
}


#endif  // INCLUDE_SPANNINGTREE_PGR_RANDOMSPANNINGTREE_HPP_
