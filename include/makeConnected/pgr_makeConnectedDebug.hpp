/*PGR-GNU*****************************************************************
File: pgr_makeConnected.hpp

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

#ifndef INCLUDE_MAKECONNECTED_PGR_MAKECONNECTED_HPP_
#define INCLUDE_MAKECONNECTED_PGR_MAKECONNECTED_HPP_
#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/ref.hpp>
#include <vector>
#include <set>
#include <map>

#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/make_connected.hpp>
#include "cpp_common/pgr_messages.h"
#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/pgr_makeConnected_t.h"
//******************************************
using namespace boost;
using namespace std;
namespace pgrouting {
namespace functions {

template < class G >
class Pgr_makeConnected : public pgrouting::Pgr_messages {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;
     typedef adjacency_list< vecS, vecS, undirectedS, property<vertex_index_t, int>>adj;
     typedef boost::graph_traits< adj >::edge_iterator  E_it;
     std::vector<pgr_makeConnected_t> makeConnected(
                 G &graph){
                   return generatemakeConnected(
                                          graph);
                 }

 private:
     std::vector< pgr_makeConnected_t >
     generatemakeConnected(
      const G &graph ) {

      adj g;
      // std::vector< graph_traits<Graph>::vertices_size_type >component(num_vertices(graph.graph));
      int64_t i=0;
      // make_connected(graph.graph);
      E_i  ei, ei_end;
      map<int64_t,int64_t>mp;
      for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei){
        int64_t src = graph[graph.source(*ei)].id;
        int64_t tgt = graph[graph.target(*ei)].id;
        if(mp.find(src)==mp.end()){
          mp[src] = i;
          log<<"ye i hai pehla"<<i<<"\n";
          i++;
        }if(mp.find(tgt)==mp.end()) {
          mp[tgt] = i;
          log<<"ye i hai dusra"<<i<<"\n";
          i++;
        }
        int64_t u = mp[src];
        int64_t v = mp[tgt];
        log <<"source "<<src<<" "<<"target "<<tgt<<":::"<<"u "<<u<<" v "<<v<<"\n";
        add_edge(u,v,g);
      }

      make_connected(g);
      E_it e,e_end;
      std::vector< pgr_makeConnected_t > results(num_edges(g));
      i=0;
      for (boost::tie(e, e_end) = edges(g); e != e_end; ++e){
           auto src = source(*e,g);
           auto tgt = target(*e,g);
           log <<"src:"<<src<<" "<<"tgt:"<<tgt<<"\n";
           results[i].node_fro = mp[src];
           results[i].node_to = mp[tgt];
           i++;
      }
      return results;
  // E_1  ei, ei_end;
  // set<pair<int64_t,int64_t>>st,st2;
  // for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei){
  //          int64_t src = source(*ei,graph.graph);
  //          int64_t tgt = target(*ei,graph.graph);
  //          st.insert({src,tgt});
  //          log<<"src:"<<src<<"tgt:"<<tgt<<"\n";
  //     }
  // for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei){
  //     if(st.find({source(*ei,graph.graph),target(*ei,graph.graph)})==st.end()){
  //       st2.insert({source(*ei,graph.graph),target(*ei,graph.graph)});
  //     }
  // }
  // std::vector< pgr_makeConnected_t > results(st2.size());
  // if(st.size()>0){
  //   for(auto it :st2) {
  //     // cout<<it.first<<" "<<it.second<<"\n";
  //     results[i].node_fro = it.first;
  //     results[i].node_to = it.second;
  //     i++;
  //   }
  // } else {
  //
  // }
  //  return results;
    }
};
}
}

#endif //INCLUDE_MAKECONNECTED_PGR_MAKECONNECTED_HPP_
