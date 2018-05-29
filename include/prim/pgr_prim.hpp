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

#ifndef INCLUDE_PRIM_PGR_PRIM_HPP_
#define INCLUDE_PRIM_PGR_PRIM_HPP_
#pragma once

#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"


template < class G > class Pgr_prim;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_prim {
 public:

     typedef typename G::V V;

     std::vector<pgr_prim_t> prim(
                 G &graph);

 private:
     
     // Member
     std::vector< V > predecessors;
     std::vector< double > distances;
     
     //Functions
     void clear() {
         predecessors.clear();
         distances.clear();
     }
     
    std::vector< pgr_prim_t > 
    GenerateResults(
	       const G &graph) {

         size_t totalNodes = num_vertices(graph.graph); // Total Node in graph    

         /*Calculate connected components*/
         std::vector< int > components(totalNodes);
         size_t num_comps = boost::connected_components(graph.graph, &components[0]);
         
         std::vector< std::vector< int64_t > > component;
         component.resize(num_comps);
         for (size_t i = 0; i < totalNodes; i++)
             component[components[i]].push_back(i);
    
         std::vector< pgr_prim_t > results;
         for (size_t i = 0; i < num_comps; i++) {
            
               /*Implementation */
               clear();
               predecessors.resize(graph.num_vertices());
               distances.resize(graph.num_vertices());
               boost::prim_minimum_spanning_tree(
               	           graph.graph,
					       &predecessors[0],
                           boost::distance_map(&distances[0]).
                           weight_map(get(&G::G_T_E::cost, graph.graph)).root_vertex(component[i][0]));
               double totalcost = 0;
               int seqCount = 0;

               /*Generate Result*/
               for (size_t j = 0; j < totalNodes; j++) {
     	         pgr_prim_t tmp;
	             tmp.start_node = graph.graph[j].id;  // Start node         
                 if( static_cast< int >(j) == component[i][0] ){
                               tmp.seq = ++seqCount;
                               tmp.edge = -1; 
                               tmp.end_node = -1;
                               tmp.cost = 0;
                               tmp.agg_cost = totalcost;
                               results.push_back(tmp); 	  
                 }     // for root node 
                 if(predecessors[j]!=j) { 
                     tmp.seq = ++seqCount;
	                 tmp.end_node = graph.graph[predecessors[j]].id;  //end node
                     auto v_sn(graph.get_V(tmp.start_node));
	                 auto v_en(graph.get_V(tmp.end_node));

	                 auto cost = distances[v_sn] - distances[v_en];
                     auto edge_id = 
                       graph.get_edge_id(v_sn, v_en, cost);
	                 totalcost += cost;    
 
	                 tmp.edge = edge_id; 	  // edge_id
	                 tmp.cost = cost; 		  // cost
                     tmp.agg_cost = totalcost;    // agg_cost
                     results.push_back(tmp);
                 } //IF
               }//for j

         }//for i
         return results;
     }     // main generate function
			
};

template < class G >
std::vector<pgr_prim_t>
Pgr_prim< G >::prim(
             G &graph) {

	return GenerateResults(
	            graph);
}


#endif  // INCLUDE_PRIM_PGR_PRIM_HPP_
