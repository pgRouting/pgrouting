/*PGR-GNU*****************************************************************
File: pgr_bandwidth.hpp

Copyright (c) 2023 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Sanskar Bhushan
Mail: sbdtu5498@gmail.com

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

#ifndef INCLUDE_METRICS_PGR_BANDWIDTH_HPP_
#define INCLUDE_METRICS_PGR_BANDWIDTH_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/bandwidth.hpp>

#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <set>

#include "c_types/edge_t.h"
#include "c_types/i_rt.h"

namespace pgrouting {

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
    boost::no_property, boost::property<boost::edge_weight_t, double>> UndirectedGraph;

namespace bandwidth {

template<class G>
class PgrBandwidthGraph {
 public:
  G boost_graph;

  typedef typename boost::graph_traits<G>::vertex_descriptor V;
  typedef typename boost::graph_traits<G>::edge_descriptor E;
  typedef typename boost::graph_traits<G>::vertex_iterator V_it;
  typedef typename boost::graph_traits<G>::edge_iterator E_it;
  typedef typename std::pair<E_it, E_it> E_p;

  std::map<int64_t, V> id_to_V;
  std::map<V, int64_t> V_to_id;

  V get_boost_vertex(int64_t id) {
    return id_to_V[id];
  }

  int64_t get_vertex_id(V v) {
    return V_to_id[v];
  }

  PgrBandwidthGraph(Edge_t *data_edges, size_t total_tuples) {
      std::set<int64_t> vertices;
      for (size_t i = 0; i < total_tuples; ++i) {
          vertices.insert(data_edges[i].source);
          vertices.insert(data_edges[i].target);
      }
      for (int64_t id : vertices) {
          V v = add_vertex(boost_graph);
          id_to_V.insert(std::pair<int64_t, V>(id, v));
          V_to_id.insert(std::pair<V, int64_t>(v, id));
      }
      bool added;

      for (size_t i = 0; i < total_tuples; ++i) {
          V v1 = get_boost_vertex(data_edges[i].source);
          V v2 = get_boost_vertex(data_edges[i].target);
          E e1;
          E e2;
          if (data_edges[i].cost > 0) {
              boost::tie(e1, added) = boost::add_edge(v1, v2, data_edges[i].cost, boost_graph);
          }
          if (data_edges[i].reverse_cost > 0) {
              boost::tie(e2, added) = boost::add_edge(v2, v1, data_edges[i].reverse_cost, boost_graph);
          }
      }
  }

  I_rt
  get_bandwidth() {
      I_rt bandwidth;
      
      bandwidth.id = static_cast<int64_t>(boost::bandwidth(boost_graph));

      return bandwidth;
  }
};
}  // namespace bandwidth
}  // namespace pgrouting

#endif  // INCLUDE_METRICS_PGR_BANDWIDTH_HPP
