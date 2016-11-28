/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#pragma once

#if 0
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif
#endif

#ifdef unlink
#undef unlink
#endif
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/assert.hpp>

#include "./../../common/src/pgr_types.h"

#include <map>
#include <utility>
#include <vector>
#include <set>

#include "pgr_maxflow.hpp"

// user's functions
// for development

template<class G>
class PgrEdgeDisjointPathsGraph {
 public:
  G boost_graph;

  typedef typename boost::graph_traits<G>::vertex_descriptor V;
  typedef typename boost::graph_traits<G>::edge_descriptor E;
  typedef typename boost::graph_traits<G>::vertex_iterator V_it;
  typedef typename boost::graph_traits<G>::edge_iterator E_it;
  typedef typename boost::graph_traits<G>::out_edge_iterator Eout_it;

  typename boost::property_map<G, boost::edge_capacity_t>::type capacity;
  typename boost::property_map<G, boost::edge_reverse_t>::type rev;
  typename boost::property_map<G, boost::edge_residual_capacity_t>::type
      residual_capacity;

  std::map<int64_t, V> id_to_V;
  std::map<V, int64_t> V_to_id;
  std::map<E, int64_t> E_to_id;

  V source_vertex;
  V sink_vertex;

  V get_boost_vertex(int64_t id) {
      return id_to_V[id];
  }

  int64_t get_vertex_id(V v) {
      return V_to_id[v];
  }

  int64_t get_edge_id(E e) {
      return E_to_id[e];
  }

  int64_t boykov_kolmogorov() {
      size_t num_v = boost::num_vertices(boost_graph);
      std::vector<boost::default_color_type> color(num_v);
      std::vector<int64_t> distance(num_v);
      return boost::boykov_kolmogorov_max_flow(boost_graph,
                                               source_vertex,
                                               sink_vertex);
  }

  void create_edge_disjoint_paths_graph(pgr_basic_edge_t *data_edges,
                                        size_t total_tuples,
                                        const std::set<int64_t> &source_vertices,
                                        const std::set<int64_t> &sink_vertices,
                                        bool directed) {
      std::set<int64_t> vertices;
      for (int64_t source : source_vertices) {
          vertices.insert(source);
      }
      for (int64_t sink : sink_vertices) {
          vertices.insert(sink);
      }
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

      V supersource = add_vertex(boost_graph);
      for (int64_t source_id : source_vertices) {
          V source = get_boost_vertex(source_id);
          E e, e_rev;
          boost::tie(e, added) =
              boost::add_edge(supersource, source, boost_graph);
          boost::tie(e_rev, added) =
              boost::add_edge(source, supersource, boost_graph);
          //TODO(vicky) set to std::max
          capacity[e] = 999999999;
          capacity[e_rev] = 0;
          rev[e] = e_rev;
          rev[e_rev] = e;
      }

      V supersink = add_vertex(boost_graph);
      for (int64_t sink_id : sink_vertices) {
          V sink = get_boost_vertex(sink_id);
          E e1, e1_rev;
          boost::tie(e1, added) =
              boost::add_edge(sink, supersink, boost_graph);
          boost::tie(e1_rev, added) =
              boost::add_edge(supersink, sink, boost_graph);
          //TODO(vicky) set to std::max
          capacity[e1] = 999999999;
          capacity[e1_rev] = 0;
          rev[e1] = e1_rev;
          rev[e1_rev] = e1;
      }

      source_vertex = supersource;
      sink_vertex = supersink;

      capacity = get(boost::edge_capacity, boost_graph);
      rev = get(boost::edge_reverse, boost_graph);
      residual_capacity =
          get(boost::edge_residual_capacity, boost_graph);

      for (size_t i = 0; i < total_tuples; ++i) {
          V v1 = get_boost_vertex(data_edges[i].source);
          V v2 = get_boost_vertex(data_edges[i].target);
          if (directed) {
              E e, e_rev;
              boost::tie(e, added) =
                  boost::add_edge(v1, v2, boost_graph);
              boost::tie(e_rev, added) =
                  boost::add_edge(v2, v1, boost_graph);
              E_to_id.insert(std::pair<E, int64_t>(e, data_edges[i].id));
              E_to_id.insert(std::pair<E, int64_t>(e_rev,
                                                   data_edges[i].id));
              capacity[e] = data_edges[i].going ? 1 : 0;
              capacity[e_rev] = data_edges[i].coming ? 1 : 0;
              rev[e] = e_rev;
              rev[e_rev] = e;
          } else {
              if (data_edges[i].going || data_edges[i].coming) {
                  E e, e_rev;
                  boost::tie(e, added) =
                      boost::add_edge(v1, v2, boost_graph);
                  boost::tie(e_rev, added) =
                      boost::add_edge(v2, v1, boost_graph);
                  E_to_id.insert(std::pair<E, int64_t>(e, data_edges[i].id));
                  E_to_id.insert(std::pair<E, int64_t>(e_rev,
                                                       data_edges[i].id));
                  capacity[e] = 1;
                  capacity[e_rev] = 1;
                  rev[e] = e_rev;
                  rev[e_rev] = e;
              }
          }
      }
  }

  void
  flow_dfs(V vertex,
           int64_t path_id,
           std::vector<std::vector<int64_t> > &paths) {
      Eout_it ei, e_end;
      if (boost::edge(vertex, sink_vertex, boost_graph).second) {
          int64_t v_id = get_vertex_id(vertex);
          paths[path_id].push_back(v_id);
      } else {
          for (boost::tie(ei, e_end) =
                   boost::out_edges(vertex, boost_graph);
               ei != e_end; ++ei) {
              if (residual_capacity[*ei] < capacity[*ei]) {
                  // exclude this edge from subsequent visits
                  capacity[*ei] = -1;
                  int64_t v_id = get_vertex_id(vertex);
                  paths[path_id].push_back(v_id);
                  flow_dfs((*ei).m_target,
                           path_id,
                           paths);
                  break;
              }
          }
      }
  }

  void
  get_edge_disjoint_paths(std::vector<General_path_element_t> &path_elements,
                          int64_t flow) {
      std::vector<std::vector<int64_t> > paths(flow, std::vector<int64_t>());
      int64_t path_id = 0;
      Eout_it ei, e_end, ei2, e2_end;
      for (boost::tie(ei, e_end) =
               boost::out_edges(source_vertex, boost_graph);
           ei != e_end; ++ei) {
          if (capacity[*ei] - residual_capacity[*ei] > 0) {
              for (boost::tie(ei2, e2_end) =
                       boost::out_edges((*ei).m_target, boost_graph);
                   ei2 != e2_end; ++ei2) {
                  if (capacity[*ei2] - residual_capacity[*ei2]
                      > 0) {
                      paths[path_id].push_back(get_vertex_id((*ei2).m_source));
                      flow_dfs((*ei2).m_target, path_id, paths);
                      path_id++;
                  }
              }
          }
      }
      for (int i = 0; i < flow; i++) {
          size_t size = paths[i].size();
          E e;
          bool exists;
          size_t j;
          for (j = 0; j < size - 1; j++) {
              General_path_element_t edge;
              edge.seq = (int) (j + 1);
              edge.start_id = paths[i][0];
              edge.end_id = paths[i][size - 1];
              edge.node = paths[i][j];
              boost::tie(e, exists) = boost::edge(get_boost_vertex(paths[i][j]),
                                                  get_boost_vertex(paths[i][j
                                                      + 1]),
                                                  boost_graph);
              edge.edge = get_edge_id(e);
              path_elements.push_back(edge);
          }
          General_path_element_t edge;
          edge.seq = (int) (j + 1);
          edge.start_id = paths[i][0];
          edge.end_id = paths[i][size - 1];
          edge.node = paths[i][j];
          edge.edge = -1;
          path_elements.push_back(edge);
      }
  }
};
