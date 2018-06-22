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

#ifndef INCLUDE_PRIM_PGR_KRUSKAL_HPP_
#define INCLUDE_PRIM_PGR_KRUSKAL_HPP_
#pragma once

#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"

template < class G > class Pgr_kruskal;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_kruskal {
 public:

     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;
     std::vector< V > predecessors;
     std::vector<pgr_kruskal_t> kruskal(
                 G &graph);

     std::vector< pgr_kruskal_t > 
     generateKruskal(
	       const G &graph ) {
       typename std::vector < E >::iterator ei;
     
       std::vector < E > spanning_tree;
       boost::kruskal_minimum_spanning_tree(
                              graph.graph, 
                              std::back_inserter(spanning_tree),
                              boost::weight_map(get(&G::G_T_E::cost, graph.graph))
                              );
     
       std::vector< pgr_kruskal_t > results;
       double totalcost = 0;
 
       E_i ei1, ei_end1;

       for (ei = spanning_tree.begin(); ei != spanning_tree.end(); ++ei) {
          pgr_kruskal_t tmp; 
          for (boost::tie(ei1, ei_end1) = edges(graph.graph); ei1 != ei_end1; ++ei1) {
              if(*ei == *ei1){
                tmp.cost = graph[*ei1].cost;   
                break;
              }          
          }
          auto start_node = graph.graph[source(*ei, graph.graph)].id;
          auto end_node = graph.graph[target(*ei, graph.graph)].id; // node
         
          auto v_sn(graph.get_V(start_node));
          auto v_en(graph.get_V(end_node));

          auto edge_id = 
                 graph.get_edge_id(v_sn, v_en, tmp.cost);
	  totalcost += tmp.cost;    
 
          tmp.edge = edge_id;	     // edge_id 
          tmp.tree_cost = totalcost; 
          results.push_back(tmp);

        }
       return results; 
     }
};

template < class G >
std::vector<pgr_kruskal_t>
Pgr_kruskal< G >::kruskal(
                G &graph) {
          
      return generateKruskal(
                             graph);     
} 


#endif  // INCLUDE_PRIM_PGR_KRUSKAL_HPP_
