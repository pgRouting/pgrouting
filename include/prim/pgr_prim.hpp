/*PGR-GNU*****************************************************************
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh@gmail.com
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

#include <vector>
#include <utility>
#include <algorithm>

#include "pgr_primGraph.hpp"

template < class G > class Pgr_prim;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_prim {
 public:
     std::vector<pgr_prim_t> prim(
             G &graph);

};

template < class G >
std::vector<pgr_prim_t>
Pgr_prim< G >::prim(
        G &graph) {
         
    size_t totalNodes = num_vertices(graph.graph); //Total no. of nodes in Graph
    double totalcost = 0;			   //Total cost
    std::vector< pgr_prim_t > results;

    // Spanning of Tree using Prim Algorithm
    pgrouting::PredecessorList p(totalNodes);
    boost::prim_minimum_spanning_tree(graph.graph, &p[0]);

    // generate result
    for (size_t i = 0; i < totalNodes; i++) {
        pgr_prim_t tmp;
        tmp.seq = static_cast< int > (i + 1);   //seq
	int64_t var = static_cast< int64_t > (i + 1);	
	tmp.start_node = var;		//start node        
        if(p[i]!=i) {
            tmp.end_node = p[i];
	    pgrouting::E_i ei, ei_end;
 	    for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei) {
                if( (source(*ei, graph.graph) ==p[i] && target(*ei, graph.graph)==var) || 
                      (target(*ei, graph.graph) ==p[i] && source(*ei, graph.graph)==var)){
		tmp.edge = var;
		tmp.cost = boost::get( boost::edge_weight, graph.graph, *ei );    // cost
                totalcost += boost::get( boost::edge_weight, graph.graph, *ei );
                tmp.agg_cost = totalcost;    // agg_cost
                break;
                }
            }
        }
	else { 
            tmp.edge = -1; 
            tmp.end_node = -1;
            tmp.cost = 0;
            tmp.agg_cost = totalcost;
        }             
        results.push_back(tmp);
    }
    return results;
}


#endif  // INCLUDE_PRIM_PGR_PRIM_HPP_
