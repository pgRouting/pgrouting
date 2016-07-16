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
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/assert.hpp>

// user's functions
// for development

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS>
    BasicUndirectedGraph;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS>
    BasicDirectedGraph;



template<class G>
class PgrCardinalityGraph {
 public:
  G boost_graph;

  typedef typename boost::graph_traits<G>::vertex_descriptor V;
  typedef typename boost::graph_traits<G>::edge_descriptor E;
  typedef typename boost::graph_traits<G>::vertex_iterator V_it;
  typedef typename boost::graph_traits<G>::edge_iterator E_it;

  std::map<int64_t, V> id_to_V;
  std::map<V, int64_t> V_to_id;

  V source_vertex;
  V sink_vertex;

  V getV(int64_t id) {
      return this->id_to_V.find(id)->second;
  }

  int64_t getid(V v) {
      return this->V_to_id.find(v)->second;
  }

  void create_max_cardinality_graph(pgr_basic_edge_t *data_edges,
                                    size_t total_tuples) {
      std::set<int64_t> vertices;
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

      for (size_t i = 0; i < total_tuples; ++i) {
          V v1 = this->id_to_V.find(data_edges[i].source)->second;
          V v2 = this->id_to_V.find(data_edges[i].target)->second;
          E e1;
          boost::tie(e1, added) = boost::add_edge(v1, v2, this->boost_graph);
      }
  }


  void get_matched_vertices(std::vector<pgr_basic_edge_t> &matched_vertices,
                            const std::vector<int64_t> &mate_map) {
      V_it vi, vi_end;
      int64_t id = 1;
      if (boost::is_directed(this->boost_graph)){
          std::vector<bool> already_matched (num_vertices(this->boost_graph), false);
          for (boost::tie(vi, vi_end) = boost::vertices(this->boost_graph);
               vi != vi_end;
               ++vi) {
              /*
               * For each vertex I check:
               * 1) It is matched with a non-null vertex
               * 2) An edge exists from this vertex to his mate
               * 3) The vertices have not been matched already
               * (this last point prevents having double output with reversed
               * source and target)
               */
              if ((mate_map[*vi] != boost::graph_traits<G>::null_vertex())
                  && (boost::edge(*vi, mate_map[*vi],this->boost_graph).second)
                  && !already_matched[*vi] && !already_matched[mate_map[*vi]]) {
                  already_matched[*vi] = true;
                  already_matched[mate_map[*vi]] = true;
                  pgr_basic_edge_t matched_couple;
                  matched_couple.id = id++;
                  matched_couple.source = this->getid(*vi);
                  matched_couple.target = this->getid(mate_map[*vi]);
                  matched_vertices.push_back(matched_couple);
              }
          }
      } else {
          for (boost::tie(vi, vi_end) = boost::vertices(this->boost_graph);
               vi != vi_end;
               ++vi) {
              if ((mate_map[*vi] != boost::graph_traits<G>::null_vertex())
                      && (*vi < mate_map[*vi])) {
                  pgr_basic_edge_t matched_couple;
                  matched_couple.id = id++;
                  matched_couple.source = this->getid(*vi);
                  matched_couple.target = this->getid(mate_map[*vi]);
                  matched_vertices.push_back(matched_couple);
              }
          }
      }
  }

  void maximum_cardinality_matching(std::vector<int64_t> &mate_map) {
      checked_edmonds_maximum_cardinality_matching(this->boost_graph,
                                           &mate_map[0]);
  }

};