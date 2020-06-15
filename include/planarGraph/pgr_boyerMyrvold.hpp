/*PGR-GNU*****************************************************************
File: pgr_boyerMyrvold.hpp

Copyright (c) 2020 pgRouting developers
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
#include "c_types/pgr_boyer_t.h"
//******************************************
using namespace boost;
namespace pgrouting {
namespace functions {

template < class G >
class Pgr_boyerMyrvold {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;
     std::vector<pgr_boyer_t> boyerMyrvold(
                 G &graph){
                   return generateboyerMyrvold(
                                          graph);
                 }

 private:
     std::vector< pgr_boyer_t >
     generateboyerMyrvold(
        const G &graph ) {
       std::vector< pgr_boyer_t > results(num_edges(graph.graph));
       // results[0].id = 0;
       // results[0].source = 1;
       // results[0].target = 2;
       // results[0].cost = 3;
       // results[0].reverse_cost = 4;
       auto check = boyer_myrvold_planarity_test(graph.graph);
       if(check){
         // pgr_boyer_t him;
      E_i ei, ei_end;
      int i;
      for (boost::tie(ei, ei_end) = edges(graph.graph),i = 0; ei != ei_end; ++ei,++i){
           int64_t id = graph[*ei].id;
           int64_t source = graph[graph.source(*ei)].id;
           int64_t target = graph[graph.target(*ei)].id;
           double cost = graph[*ei].cost;
           // double reverse_cost = graph[*ei].reverse_cost;
           // results[i].id = id;
           results[i].source = source;
           results[i].target = target;
           results[i].cost = cost;
           results[i].reverse_cost = cost;
           // him.id = id;
           // tmp.id = graph.graph[*ei].id;
           // him.source = source;
           // him.target = target;
           // tmp.target = graph[graph.target(*ei)].id;
           // tmp.source = source(*ei,graph.graph);
           // tmp.target = target(*ei,graph.graph);
           // him.cost = cost;
           // tmp.id = 1;
           // tmp.cost = graph.graph[*ei].cost;
           // tmp.reverse_cost = graph.graph[*ei].cost;
           // results.push_back(him);
      }

    }
       return results;
    }
};
}
}

#endif //INCLUDE_PLANARGRAPH_PGR_BOYERMYRVOLD_HPP_
