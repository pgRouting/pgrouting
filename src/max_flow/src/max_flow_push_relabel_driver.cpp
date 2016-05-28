/*PGR-GNU*****************************************************************
File: max_flow_push_relabel_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <vector>
#include "pgr_maxflow.hpp"
#include "max_flow_push_relabel.h"
#include <map>
#include <set>
#include <pgr_types.h>

//#define DEBUG

#include "../../common/src/pgr_alloc.hpp"
extern "C" {
#include "./../../common/src/pgr_types.h"
}

void
do_pgr_max_flow_push_relabel(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        int64_t source,
        int64_t sink,
        pgr_flow_t **return_tuples,
        size_t *return_count,
        char ** err_msg) {

  std::ostringstream log;

  try {
      std::set<int64_t> vertices;
      vertices.insert(source);
      vertices.insert(sink);
      for(size_t i = 0; i < total_tuples; ++i){
          vertices.insert(data_edges[i].source);
          vertices.insert(data_edges[i].target);
      }
      FlowGraph g;
      std::map<int64_t, boost::graph_traits<FlowGraph>::vertex_descriptor> id_to_V;
      std::map<boost::graph_traits<FlowGraph>::vertex_descriptor, int64_t> V_to_id;

      for(int64_t id : vertices){
          boost::graph_traits<FlowGraph>::vertex_descriptor v = add_vertex(g);
          id_to_V.insert(std::pair<int64_t, boost::graph_traits<FlowGraph>::vertex_descriptor>(id, v));
          V_to_id.insert(std::pair<boost::graph_traits<FlowGraph>::vertex_descriptor, int64_t>(v, id));
      }

      boost::property_map<FlowGraph, boost::edge_capacity_t>::type
              capacity = get(boost::edge_capacity, g);
      boost::property_map<FlowGraph, boost::edge_reverse_t>::type
              rev = get(boost::edge_reverse, g);
      boost::property_map<FlowGraph, boost::edge_residual_capacity_t>::type
              residual_capacity = get(boost::edge_residual_capacity, g);

      for(size_t i = 0; i < total_tuples; ++i){
          bool added;
          boost::graph_traits<FlowGraph>::vertex_descriptor v1 = id_to_V.find(data_edges[i].source)->second;
          boost::graph_traits<FlowGraph>::vertex_descriptor v2 = id_to_V.find(data_edges[i].target)->second;
          if(data_edges[i].cost > 0){
              boost::graph_traits<FlowGraph>::edge_descriptor e1, e1_rev;
              boost::tie(e1, added) = boost::add_edge(v1,v2,g);
              boost::tie(e1_rev, added) = boost::add_edge(v2,v1,g);
              capacity[e1] = (long)data_edges[i].cost;
              capacity[e1_rev] = 0;
              rev[e1] = e1_rev;
              rev[e1_rev] = e1;
          }
          if (data_edges[i].reverse_cost>0){
              boost::graph_traits<FlowGraph>::edge_descriptor e2, e2_rev;
              boost::tie(e2, added) = boost::add_edge(v2,v1,g);
              boost::tie(e2_rev, added) = boost::add_edge(v1,v2,g);
              capacity[e2] = (long)data_edges[i].reverse_cost;
              capacity[e2_rev] = 0;
              rev[e2] = e2_rev;
              rev[e2_rev] = e2;
          }
      }

      boost::push_relabel_max_flow(g, id_to_V.find(source)->second, id_to_V.find(sink)->second);

      boost::graph_traits<FlowGraph>::edge_iterator e, e_end;
      std::vector<pgr_flow_t> flow_edges;
      for(boost::tie(e, e_end) = boost::edges(g); e != e_end; ++e){
            if (capacity[*e] - residual_capacity[*e] > 0){
                pgr_flow_t edge;
                edge.tail = V_to_id.find((*e).m_source)->second;
                edge.head = V_to_id.find((*e).m_target)->second;
                edge.flow = capacity[*e] - residual_capacity[*e];
                edge.residual_capacity = residual_capacity[*e];
                flow_edges.push_back(edge);
            }
      }

      (*return_tuples) = pgr_alloc(flow_edges.size(), (*return_tuples));
      for(int i=0; i<flow_edges.size(); ++i){
          (*return_tuples)[i] = flow_edges[i];
      }
      *return_count = flow_edges.size();

#ifndef DEBUG
      *err_msg = strdup("OK");
#else
      *err_msg = strdup(log.str().c_str());
#endif

      return;
  } catch ( ... ) {
      log << "Caught unknown expection!\n";
      *err_msg = strdup(log.str().c_str());
      return;
  }
}





