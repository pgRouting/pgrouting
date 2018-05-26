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
     
     std::vector< V > predecessors;
     std::vector< double > distances;
     
     void clear() {
         predecessors.clear();
         distances.clear();
     }
     
     bool primMain(
               G &graph) {
         try {
	     boost::prim_minimum_spanning_tree(graph.graph,
					       &predecessors[0],
                                               boost::distance_map(&distances[0]).
                                               weight_map(get(&G::G_T_E::cost, graph.graph))
					       );
	 } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }
         return true;
     }
     
     std::vector< pgr_prim_t > 
     GenerateResults(
 		const G &graph) {
			
	std::vector< pgr_prim_t > results;
        size_t totalNodes = num_vertices(graph.graph); // total no. of node in graph
	double totalcost = 0;
	  
	for (size_t i = 0; i < totalNodes; i++) {
          
	  pgr_prim_t tmp;
	  tmp.start_node = graph.graph[i].id;  // Start node
               
          if(predecessors[i]!=i) { 
	      tmp.end_node = graph.graph[predecessors[i]].id;  //end node
              auto v_sn(graph.get_V(tmp.start_node));
	      auto v_en(graph.get_V(tmp.end_node));
	      auto cost = distances[v_sn] - distances[v_en];
              auto edge_id = 
                 graph.get_edge_id(v_sn, v_en, cost);
	     totalcost += cost;    
 
	     tmp.edge = edge_id; 	  // edge_id
	     tmp.cost = cost; 		  // cost
             tmp.agg_cost = totalcost;    // agg_cost
                  
          } //IF
	  else { 
              tmp.edge = -1; 
              tmp.end_node = -1;
              tmp.cost = 0;
              tmp.agg_cost = totalcost;
          } //ELSEIF            
          results.push_back(tmp);
        }    // FOR 

            
        return results;
     }  // MAIN			
};

template < class G >
std::vector<pgr_prim_t>
Pgr_prim< G >::prim(
             G &graph) {

	clear(); // clear predecessors and distances vectors

        // adjust predecessors and distances vectors
	predecessors.resize(graph.num_vertices());
        distances.resize(graph.num_vertices());
     
        primMain( graph ); 

        // Now Generate result
	return GenerateResults(
	            graph);
}


#endif  // INCLUDE_PRIM_PGR_PRIM_HPP_
