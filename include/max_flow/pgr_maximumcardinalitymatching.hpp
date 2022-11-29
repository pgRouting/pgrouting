/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Ignroing directed flag & works only for undirected graph
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky at georepublic.mail

Function's developer:
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

#include <map>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "c_types/edge_bool_t_rt.h"


namespace pgrouting {
namespace flow {

class MaxCardinalityMatch {
  using G = boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS>;
  using V = boost::graph_traits<G>::vertex_descriptor;
  using E = boost::graph_traits<G>::edge_descriptor;
  using V_it = boost::graph_traits<G>::vertex_iterator;

 public:
  MaxCardinalityMatch(Edge_bool_t_rt *, size_t);
  std::vector<int64_t> get_matched_vertices();

 private:
  inline V get_boost_vertex(int64_t id) {
      return id_to_V[id];
  }

  inline int64_t get_edge_id(E e) {
      return E_to_id[e];
  }

  G boost_graph;
  std::map<int64_t, V> id_to_V;
  std::map<V, int64_t> V_to_id;
  std::map<E, int64_t> E_to_id;

};

}  // namespace flow
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_PGR_MAXIMUMCARDINALITYMATCHING_HPP_
