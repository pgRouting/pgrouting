/*PGR-GNU*****************************************************************
File: pgr_boyerMyrvold.hpp

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com

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

#ifndef INCLUDE_PLANARGRAPH_PGR_BOYERMYRVOLD_HPP_
#define INCLUDE_PLANARGRAPH_PGR_BOYERMYRVOLD_HPP_
#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/ref.hpp>
#include <vector>

#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>

#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/pgr_edge_t.h"
//******************************************

template < class G >
class Pgr_boyerMyrvold {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;

     std::vector<pgr_edge_t> boyerMyrvold(
                 G &graph);

 private:
     std::vector< pgr_edge_t >
     generateboyerMyrvold(
        const G &graph ) {
       std::vector< pgr_edge_t > results;

       if(boost::boyer_myrvold_planarity_test(graph.graph)){

      E_i ei, ei_end;
      for (tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei){
           pgr_edge_t tmp;
           tmp.id = graph[*ei].id;
           tmp.source = graph[index[source(*ei,graph)]];
           tmp.target = graph[index[target(*ei,graph)]];
           tmp.cost = graph[*ei].cost;
           tmp.reverse_cost = graph[*ei].reverse_cost;
           results.push_back(tmp);
      }
       //     std::cout << "(" << index[source(*ei, g)]
       //               << "," << index[target(*ei, g)] << ") ";
       // std::cout << std::endl;


       // typename std::vector< V >::reverse_iterator ii;
       // for (ii = c.rbegin(); ii != c.rend(); ++ii) {
       //     auto t = *ii;
       //     pgr_edge_t tmp;
       //     tmp.sorted_v = graph.graph[t].id;
       //     results.push_back(tmp);
       // }

       return results;
};

template < class G >
std::vector<pgr_edge_t>
Pgr_boyerMyrvold< G >::boyerMyrvold(
                G &graph) {
      pgassert(num_vertices(graph.graph) >= 1);
      return generateboyerMyrvold(
                             graph);
}


#endif INCLUDE_PLANARGRAPH_PGR_BOYERMYRVOLD_HPP_
