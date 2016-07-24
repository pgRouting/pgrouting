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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef unlink
#undef unlink
#endif
#endif


#if 0
#include "./../../common/src/signalhandler.h"
#endif
#include "./../../common/src/pgr_types.h"

#include <cstdint>
#include <map>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/assert.hpp>

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
      return this->id_to_V.find(id)->second;
  }

  int64_t get_vertex_id(V v) {
      return this->V_to_id.find(v)->second;
  }

  int64_t get_edge_id(E e) {
      return this->E_to_id.find(e)->second;
  }

  int64_t push_relabel() {
      return boost::push_relabel_max_flow(this->boost_graph,
                                          this->source_vertex,
                                          this->sink_vertex);
  }

  int64_t edmonds_karp() {
      return boost::edmonds_karp_max_flow(this->boost_graph,
                                          this->source_vertex,
                                          this->sink_vertex);
  }

  int64_t boykov_kolmogorov() {
      size_t num_v = boost::num_vertices(this->boost_graph);
      std::vector<boost::default_color_type> color(num_v);
      std::vector<long> distance(num_v);
      return boost::boykov_kolmogorov_max_flow(this->boost_graph,
                                               this->source_vertex,
                                               this->sink_vertex);
  }

  void create_edge_disjoint_paths_graph(pgr_basic_edge_t *data_edges,
                                        size_t total_tuples,
                                        const std::set<int64_t> &source_vertices,
                                        const std::set<int64_t> &sink_vertices,
                                        bool directed) {

      std::set<int64_t> vertices;
      for (int64_t source: source_vertices) {
          vertices.insert(source);
      }
      for (int64_t sink: sink_vertices) {
          vertices.insert(sink);
      }
      for (size_t i = 0; i < total_tuples; ++i) {
          vertices.insert(data_edges[i].source);
          vertices.insert(data_edges[i].target);
      }
      for (int64_t id : vertices) {
          V v = add_vertex(this->boost_graph);
          this->id_to_V.insert(std::pair<int64_t, V>(id, v));
          this->V_to_id.insert(std::pair<V, int64_t>(v, id));
      }
      bool added;

      V supersource = add_vertex(this->boost_graph);
      for (int64_t source_id: source_vertices) {
          V source = this->get_boost_vertex(source_id);
          E e1, e1_rev;
          boost::tie(e1, added) =
              boost::add_edge(supersource, source, this->boost_graph);
          boost::tie(e1_rev, added) =
              boost::add_edge(source, supersource, this->boost_graph);
          this->capacity[e1] = 999999999;
          this->capacity[e1_rev] = 0;
          this->rev[e1] = e1_rev;
          this->rev[e1_rev] = e1;
      }

      V supersink = add_vertex(this->boost_graph);
      for (int64_t sink_id: sink_vertices) {
          V sink = this->get_boost_vertex(sink_id);
          E e1, e1_rev;
          boost::tie(e1, added) =
              boost::add_edge(sink, supersink, this->boost_graph);
          boost::tie(e1_rev, added) =
              boost::add_edge(supersink, sink, this->boost_graph);
          this->capacity[e1] = 999999999;
          this->capacity[e1_rev] = 0;
          this->rev[e1] = e1_rev;
          this->rev[e1_rev] = e1;
      }

      this->source_vertex = supersource;
      this->sink_vertex = supersink;

      this->capacity = get(boost::edge_capacity, this->boost_graph);
      this->rev = get(boost::edge_reverse, this->boost_graph);
      this->residual_capacity =
          get(boost::edge_residual_capacity, this->boost_graph);

      for (size_t i = 0; i < total_tuples; ++i) {
          V v1 = this->get_boost_vertex(data_edges[i].source);
          V v2 = this->get_boost_vertex(data_edges[i].target);
          if (directed) {
              E e, e_rev;
              boost::tie(e, added) =
                  boost::add_edge(v1, v2, this->boost_graph);
              boost::tie(e_rev, added) =
                  boost::add_edge(v2, v1, this->boost_graph);
              this->E_to_id.insert(std::pair<E, int64_t>(e, data_edges[i].id));
              this->E_to_id.insert(std::pair<E, int64_t>(e_rev,
                                                         data_edges[i].id));
              this->capacity[e] = data_edges[i].going > 0 ? 1 : 0;
              this->capacity[e_rev] = data_edges[i].coming > 0 ? 1 : 0;
              this->rev[e] = e_rev;
              this->rev[e_rev] = e;
          } else {
              if (data_edges[i].going > 0 || data_edges[i].coming > 0) {
                  E e, e_rev;
                  boost::tie(e, added) =
                      boost::add_edge(v1, v2, this->boost_graph);
                  boost::tie(e_rev, added) =
                      boost::add_edge(v2, v1, this->boost_graph);
                  this->E_to_id.insert(std::pair<E, int64_t>(e, data_edges[i].id));
                  this->E_to_id.insert(std::pair<E, int64_t>(e_rev,
                                                             data_edges[i].id));
                  this->capacity[e] = 1;
                  this->capacity[e_rev] = 1;
                  this->rev[e] = e_rev;
                  this->rev[e_rev] = e;
              }
          }
      }
  }

  void
  flow_dfs(V vertex,
           int64_t path_id,
           std::vector<std::vector<int64_t> > &paths) {
      Eout_it ei, e_end;
      if (boost::edge(vertex, this->sink_vertex, this->boost_graph).second) {
          int64_t v_id = get_vertex_id(vertex);
          paths[path_id].push_back(v_id);
      }
      else {
          for (boost::tie(ei, e_end) =
                   boost::out_edges(vertex, this->boost_graph);
               ei != e_end; ++ei) {
              if (this->residual_capacity[*ei] < this->capacity[*ei]) {
                  //exclude this edge from subsequent visits
                  this->capacity[*ei] = -1;
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
               boost::out_edges(this->source_vertex, this->boost_graph);
           ei != e_end; ++ei) {
          if (this->capacity[*ei] - this->residual_capacity[*ei] > 0) {
              for (boost::tie(ei2, e2_end) =
                       boost::out_edges((*ei).m_target, this->boost_graph);
                   ei2 != e2_end; ++ei2) {
                  if (this->capacity[*ei2] - this->residual_capacity[*ei2]
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
          int j;
          for (j = 0; j < size - 1; j++) {
              General_path_element_t edge;
              edge.seq = j + 1;
              edge.start_id = paths[i][0];
              edge.end_id = paths[i][size - 1];
              edge.node = paths[i][j];
              boost::tie(e, exists) = boost::edge(get_boost_vertex(paths[i][j]),
                                                  get_boost_vertex(paths[i][j
                                                      + 1]),
                                                  this->boost_graph);
              edge.edge = this->get_edge_id(e);
              path_elements.push_back(edge);
          }
          General_path_element_t edge;
          edge.seq = j + 1;
          edge.start_id = paths[i][0];
          edge.end_id = paths[i][size - 1];
          edge.node = paths[i][j];
          edge.edge = -1;
          path_elements.push_back(edge);
      }
  }

};