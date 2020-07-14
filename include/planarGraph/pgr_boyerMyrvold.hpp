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
#include <map>

#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>


#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/make_biconnected_planar.hpp>

#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/pgr_boyer_t.h"
//******************************************
using namespace boost;
using namespace std;
namespace pgrouting {
namespace functions {

template < class G >
class Pgr_boyerMyrvold {
 public:
 typedef typename G::V V;
 typedef typename G::E E;
 typedef typename G::E_i E_i;
 typedef adjacency_list< vecS, vecS, undirectedS, property<vertex_index_t, int>, property<edge_index_t, int>>biConnected;
 typedef std::vector< graph_traits<biConnected>::edge_descriptor > vec_t;
     std::vector<pgr_boyer_t> boyerMyrvold(
                 G &graph){
                   return generateboyerMyrvold(
                                          graph);
                 }

 private:
     std::vector< pgr_boyer_t >
     generateboyerMyrvold(
         G &graph ) {

     int i=0;
     biConnected g;
     E_i  ei, ei_end;
     map<int64_t,int64_t>mp,mp2;
     for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei){
       int64_t src = graph[graph.source(*ei)].id;
       int64_t tgt = graph[graph.target(*ei)].id;
       if(mp.find(src)==mp.end()){
         mp[src] = i;
         mp2[i] = src;
         // log<<"ye i hai pehla"<<i<<"\n";
         i++;
       }if(mp.find(tgt)==mp.end()) {
         mp[tgt] = i;
         mp2[i] = tgt;
         // log<<"ye i hai dusra"<<i<<"\n";
         i++;
       }
       int64_t u = mp[src];
       int64_t v = mp[tgt];
       // log <<"source "<<src<<" "<<"target "<<tgt<<":::"<<"u "<<u<<" v "<<v<<"\n";
       add_edge(u,v,g);
     }


     property_map<biConnected, edge_index_t>::type e_index = get(edge_index, g);
     graph_traits<biConnected>::edges_size_type edge_count = 0;
     graph_traits<biConnected>::edge_iterator e, e_end;
     for(boost::tie(e, e_end) = edges(g); e != e_end; ++e) {
         put(e_index, *e, edge_count++);
     }



     std::vector< pgr_boyer_t > results;
     auto check = boyer_myrvold_planarity_test(g);
     std::vector < vec_t > embedding (num_vertices(g));
     boyer_myrvold_planarity_test(boyer_myrvold_params::graph = g,
       boyer_myrvold_params::embedding = &embedding[0]);
     make_biconnected_planar(g, &embedding[0]);
     if(check){
     // E_i ei, ei_end;

     for (boost::tie(e, e_end) = edges(g),i = 0; e != e_end; ++e,++i){
         int64_t u = source(*e,g);
         int64_t v = target(*e,g);
         // double cost = graph[*ei].cost;
         pgr_boyer_t tmp;
         tmp.source = mp2[u];
         tmp.target = mp2[v];
         // tmp.cost = cost;
         results.push_back(tmp);
      }

    }
       return results;
    }
};
}
}

#endif //INCLUDE_PLANARGRAPH_PGR_BOYERMYRVOLD_HPP_
