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
#ifdef unlink
#undef unlink
#endif
#endif
#endif

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>

#if 0
#include "./../../common/src/signalhandler.h"
#endif
#include "./../../common/src/pgr_types.h"

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <set>


// user's functions
// for development

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    Traits;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
        // Vertex properties
        boost::property<boost::vertex_name_t, std::string,
        boost::property<boost::vertex_index_t, int64_t,
        boost::property<boost::vertex_color_t, boost::default_color_type,
        boost::property<boost::vertex_distance_t, int64_t,
        boost::property<boost::vertex_predecessor_t, Traits::edge_descriptor> > > > >,
        // Edge properties
        boost::property<boost::edge_capacity_t, int64_t,
        boost::property<boost::edge_residual_capacity_t, int64_t,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >
    FlowGraph;

template<class G>
class PgrFlowGraph {
 public:
  G boost_graph;

  typedef typename boost::graph_traits<G>::vertex_descriptor V;
  typedef typename boost::graph_traits<G>::edge_descriptor E;
  typedef typename boost::graph_traits<G>::vertex_iterator V_it;
  typedef typename boost::graph_traits<G>::edge_iterator E_it;

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

  int64_t push_relabel() {
      return boost::push_relabel_max_flow(boost_graph,
                                          source_vertex,
                                          sink_vertex);
  }

  int64_t edmonds_karp() {
      return boost::edmonds_karp_max_flow(boost_graph,
                                          source_vertex,
                                          sink_vertex);
  }

  int64_t boykov_kolmogorov() {
      size_t num_v = boost::num_vertices(boost_graph);
      std::vector<boost::default_color_type> color(num_v);
      std::vector<int64_t> distance(num_v);
      return boost::boykov_kolmogorov_max_flow(boost_graph,
                                               source_vertex,
                                               sink_vertex);
  }

  void create_flow_graph(pgr_edge_t *data_edges,
                         size_t total_tuples,
                         const std::set<int64_t> &source_vertices,
                         const std::set<int64_t> &sink_vertices,
                         char *algorithm) {
      /* In multi source flow graphs, a super source is created connected to all sources with "infinite" capacity
       * The same applies for sinks.
       * To avoid code repetition, a supersource/sink is used even in the one to one signature.
       */
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
          // TODO(vicky) set to std::max
          capacity[e] = 999999999;
          capacity[e_rev] = 0;
          rev[e] = e_rev;
          rev[e_rev] = e;
      }

      V supersink = add_vertex(boost_graph);
      for (int64_t sink_id : sink_vertices) {
          V sink = get_boost_vertex(sink_id);
          E e, e_rev;
          boost::tie(e, added) = boost::add_edge(sink, supersink, boost_graph);
          boost::tie(e_rev, added) =
              boost::add_edge(supersink, sink, boost_graph);
          capacity[e] = 999999999;
          capacity[e_rev] = 0;
          rev[e] = e_rev;
          rev[e_rev] = e;
      }

      source_vertex = supersource;
      sink_vertex = supersink;

      capacity = get(boost::edge_capacity, boost_graph);
      rev = get(boost::edge_reverse, boost_graph);
      residual_capacity = get(boost::edge_residual_capacity, boost_graph);

      /*
       * Push-relabel requires each edge to be mapped to its reverse with capacity 0.
       * The other algorithms have no such requirement. (I can have half as many edges)
       */
      if (strcmp(algorithm, "push_relabel") == 0) {
          for (size_t i = 0; i < total_tuples; ++i) {
              V v1 = get_boost_vertex(data_edges[i].source);
              V v2 = get_boost_vertex(data_edges[i].target);
              E e1, e1_rev, e2, e2_rev;
              if (data_edges[i].cost > 0) {
                  boost::tie(e1, added) = boost::add_edge(v1, v2, boost_graph);
                  boost::tie(e1_rev, added) =
                      boost::add_edge(v2, v1, boost_graph);
                  E_to_id.insert(std::pair<E, int64_t>(e1, data_edges[i].id));
                  E_to_id.insert(std::pair<E, int64_t>(e1_rev,
                                                       data_edges[i].id));
                  capacity[e1] = (int64_t) data_edges[i].cost;
                  capacity[e1_rev] = 0;
                  rev[e1] = e1_rev;
                  rev[e1_rev] = e1;
              }
              if (data_edges[i].reverse_cost > 0) {
                  boost::tie(e2, added) = boost::add_edge(v2, v1, boost_graph);
                  boost::tie(e2_rev, added) =
                      boost::add_edge(v1, v2, boost_graph);
                  E_to_id.insert(std::pair<E, int64_t>(e2, data_edges[i].id));
                  E_to_id.insert(std::pair<E, int64_t>(e2_rev,
                                                       data_edges[i].id));
                  capacity[e2] = (int64_t) data_edges[i].reverse_cost;
                  capacity[e2_rev] = 0;
                  rev[e2] = e2_rev;
                  rev[e2_rev] = e2;
              }
          }
      } else {
          for (size_t i = 0; i < total_tuples; ++i) {
              V v1 = get_boost_vertex(data_edges[i].source);
              V v2 = get_boost_vertex(data_edges[i].target);
              E e, e_rev;
              boost::tie(e, added) = boost::add_edge(v1, v2, boost_graph);
              boost::tie(e_rev, added) =
                  boost::add_edge(v2, v1, boost_graph);
              E_to_id.insert(std::pair<E, int64_t>(e, data_edges[i].id));
              E_to_id.insert(std::pair<E, int64_t>(e_rev, data_edges[i].id));
              capacity[e] =
                  data_edges[i].cost > 0 ? (int64_t) data_edges[i].cost : 0;
              capacity[e_rev] = data_edges[i].reverse_cost > 0
                                ? (int64_t) data_edges[i].reverse_cost : 0;
              rev[e] = e_rev;
              rev[e_rev] = e;
          }
      }
  }

  void get_flow_edges(std::vector<pgr_flow_t> &flow_edges) {
      E_it e, e_end;
      for (boost::tie(e, e_end) = boost::edges(boost_graph); e != e_end;
           ++e) {
          // A supersource/supersink is used internally
          if (((capacity[*e] - residual_capacity[*e]) > 0) &&
              ((*e).m_source != source_vertex) &&
              ((*e).m_target != sink_vertex)) {
              pgr_flow_t edge;
              edge.edge = get_edge_id(*e);
              edge.source = get_vertex_id((*e).m_source);
              edge.target = get_vertex_id((*e).m_target);
              edge.flow = capacity[*e] - residual_capacity[*e];
              edge.residual_capacity = residual_capacity[*e];
              flow_edges.push_back(edge);
          }
      }
  }
};
