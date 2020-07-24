/*PGR-GNU*****************************************************************
File: pgr_LTDTree.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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


#ifndef INCLUDE_LTDTREE_PGR_LTDTREE_HPP
#define INCLUDE_LTDTREE_PGR_LTDTREE_HPP
#pragma once
#include <vector>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
#include <iostream>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
using namespace boost;
using namespace  std;

namespace pgrouting {
    namespace functions {

        template<class G>
        class Pgr_LTDTree : public pgrouting::Pgr_messages {
        public:
            typedef typename G::V Vertex;
            typedef typename G::E_i E_i;
         
            typedef pair<int64_t , int64_t> edge; //For making edge list to be used in extract vertices
            vector<edge> edgeList;
            std::vector<pgr_ltdtree_rt> results;
            


/************************************* To check validity of root vertex****************************************/
           

/******************** Method to calculate dominator tree and returns result vector ***************************/
            std::vector <pgr_ltdtree_rt> pgr_ltdtree(
                    G &graph,
                    int64_t root
                    ){
                   const int64_t numOfVertices=graph.num_vertices();
                  
               
               
                   // Lengauer-Tarjan dominator tree algorithm
                   auto v_root(graph.get_V(root));
                  vector<Vertex> domTreePredVector =  vector<Vertex>(num_vertices(graph.graph));
                  auto domTreePredMap =
                           make_iterator_property_map(domTreePredVector.begin(), boost::get(boost::vertex_index, graph.graph));
                   
                   lengauer_tarjan_dominator_tree(graph.graph, v_root, domTreePredMap);



                return results;
            }
        };
    }
}
#endif  // INCLUDE_LTDTREE_PGR_LTDTREE_HPP
