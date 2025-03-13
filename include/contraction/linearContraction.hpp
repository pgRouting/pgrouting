/*PGR-GNU*****************************************************************
File: linearContraction.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Rohith Reddy
Mail:

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

#ifndef INCLUDE_CONTRACTION_LINEARCONTRACTION_HPP_
#define INCLUDE_CONTRACTION_LINEARCONTRACTION_HPP_
#pragma once

#include <queue>
#include <functional>
#include <vector>
#include <cstdint>

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "cpp_common/ch_edge.hpp"
#include "cpp_common/identifiers.hpp"

namespace pgrouting {
namespace contraction {

template < class G >
class Pgr_linear {
 private:
     typedef typename G::V V;
     typedef typename G::V_i V_i;
     typedef typename G::B_G B_G;
     typedef typename G::E E;

 public:
     void operator()(G &graph, const Identifiers<V>& forbidden_vertices) {
         doContraction(graph, forbidden_vertices);
     }

     Pgr_linear() = default;

     bool is_contractible(G &graph, V v) {
         return graph.is_linear(v) && !graph.is_forbidden(v);
     }

     void calculateVertices(G &graph) {
         m_linearVertices.clear();
         V_i vi;
         BGL_FORALL_VERTICES_T(v, graph.graph, B_G) {
             if (is_contractible(graph, v)) {
                 m_linearVertices += v;
             }
         }
     }

     void doContraction(G &graph, Identifiers<V> forbidden_vertices) {
         graph.set_forbidden_vertices(forbidden_vertices);
         calculateVertices(graph);

         while (!m_linearVertices.empty()) {
             V v = m_linearVertices.front();
             m_linearVertices -= v;
             pgassert(is_contractible(graph, v));
             one_cycle(graph, v);
         }
     }

     void one_cycle(G &graph, V v) {
         pgassert(is_contractible(graph, v));

         Identifiers<V> adjacent_vertices =
             graph.find_adjacent_vertices(v);
         pgassert(adjacent_vertices.size() == 2);

         V u = adjacent_vertices.front();
         adjacent_vertices.pop_front();
         V w = adjacent_vertices.front();
         adjacent_vertices.pop_front();

         pgassert(v != u);
         pgassert(v != w);
         pgassert(u != w);

         E e, f;
         bool found_e, found_f;

         E e, f;
         bool found_e, found_f;

         if (graph.is_directed()) {
             /*
              *  u --> v --> w
              */
            boost::tie(e, found_e) = boost::edge(u, v, graph.graph);
            boost::tie(f, found_f) = boost::edge(v, w, graph.graph);
            if (found_e && found_f) {
                graph.process_shortcut(u, v, w);
            }
            boost::tie(e, found_e) = boost::edge(u, v, graph.graph);
            boost::tie(f, found_f) = boost::edge(v, w, graph.graph);
            if (found_e && found_f) {
                graph.process_shortcut(u, v, w);
            }
             /*
              *  w --> v --> u
              */
             boost::tie(e, found_e) = boost::edge(w, v, graph.graph);
             boost::tie(f, found_f) = boost::edge(v, u, graph.graph);
             if (found_e && found_f) {
                graph.process_shortcut(w, v, u);
             }
             boost::tie(e, found_e) = boost::edge(w, v, graph.graph);
             boost::tie(f, found_f) = boost::edge(v, u, graph.graph);
             if (found_e && found_f) {
                graph.process_shortcut(w, v, u);
             }

         } else {
             pgassert(graph.is_undirected());
             /*
              * u - v - w
              */
             graph.process_shortcut(u, v, w);
             graph.process_shortcut(u, v, w);
         }

         graph[v].contracted_vertices().clear();
         boost::clear_vertex(v, graph.graph);
         m_linearVertices -= v;

         if (is_contractible(graph, u)) {
             one_cycle(graph, u);
         } else {
             m_linearVertices -= u;
         }
         if (is_contractible(graph, w)) {
             one_cycle(graph, w);
         } else {
             m_linearVertices -= w;
         }
     }

 private:
     Identifiers<V> m_linearVertices;
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_LINEARCONTRACTION_HPP_
