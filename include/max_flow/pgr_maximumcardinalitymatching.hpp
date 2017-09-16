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

#ifndef INCLUDE_MAX_FLOW_PGR_MAXIMUMCARDINALITYMATCHING_HPP_
#define INCLUDE_MAX_FLOW_PGR_MAXIMUMCARDINALITYMATCHING_HPP_
#pragma once


#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


#include <map>
#include <vector>
#include <utility>
#include <set>




namespace pgrouting {

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS>
    BasicUndirectedGraph;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS>
    BasicDirectedGraph;

namespace flow {

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
  std::map<E, int64_t> E_to_id;

  V get_boost_vertex(int64_t id) {
      return id_to_V[id];
  }

  int64_t get_vertex_id(V v) {
      return V_to_id[v];
  }

  int64_t get_edge_id(E e) {
      return E_to_id[e];
  }

  void create_max_cardinality_graph(pgr_basic_edge_t *data_edges,
                                    size_t total_tuples) {
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
          if (data_edges[i].going) {
              boost::tie(e1, added) = boost::add_edge(v1, v2, boost_graph);
              E_to_id.insert(std::pair<E, int64_t>(e1, data_edges[i].id));
          }
          if (data_edges[i].coming) {
              boost::tie(e2, added) = boost::add_edge(v2, v1, boost_graph);
              E_to_id.insert(std::pair<E, int64_t>(e2, data_edges[i].id));
          }
      }
  }

  void get_matched_vertices(std::vector<pgr_basic_edge_t> &matched_vertices,
                            const std::vector<int64_t> &mate_map) {
      V_it vi, vi_end;
      E e;
      bool exists;
      if (boost::is_directed(boost_graph)) {
          std::vector<bool> already_matched(num_vertices(boost_graph), false);
          for (boost::tie(vi, vi_end) = boost::vertices(boost_graph);
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
              boost::tie(e, exists) =
                  boost::edge(*vi, mate_map[*vi], boost_graph);
              if (((uint64_t)mate_map[*vi]
                          != boost::graph_traits<G>::null_vertex())
                  && exists && !already_matched[*vi]
                  && !already_matched[mate_map[*vi]]) {
                  already_matched[*vi] = true;
                  already_matched[mate_map[*vi]] = true;
                  pgr_basic_edge_t matched_couple;
                  matched_couple.source = get_vertex_id(*vi);
                  matched_couple.target = get_vertex_id(mate_map[*vi]);
                  matched_couple.edge_id = get_edge_id(e);
                  matched_vertices.push_back(matched_couple);
              }
          }
      } else {
          for (boost::tie(vi, vi_end) = boost::vertices(boost_graph);
               vi != vi_end;
               ++vi) {
              boost::tie(e, exists) =
                  boost::edge(*vi, mate_map[*vi], boost_graph);
              if (((uint64_t)mate_map[*vi]
                          != boost::graph_traits<G>::null_vertex())
                  && (*vi < (uint64_t)mate_map[*vi])) {
                  pgr_basic_edge_t matched_couple;
                  matched_couple.source = get_vertex_id(*vi);
                  matched_couple.target = get_vertex_id(mate_map[*vi]);
                  matched_couple.edge_id = get_edge_id(e);
                  matched_vertices.push_back(matched_couple);
              }
          }
      }
  }

  void maximum_cardinality_matching(std::vector<int64_t> &mate_map) {
      edmonds_maximum_cardinality_matching(boost_graph,
                                           &mate_map[0]);
  }
};

}  // namespace flow
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_PGR_MAXIMUMCARDINALITYMATCHING_HPP_
