/*PGR-GNU*****************************************************************
File: pgr_linear.c

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

#ifndef INCLUDE_CONTRACTION_PGR_LINEARCONTRACTION_HPP_
#define INCLUDE_CONTRACTION_PGR_LINEARCONTRACTION_HPP_
#pragma once


#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <queue>
#include <functional>
#include <vector>

#include "cpp_common/identifiers.hpp"


namespace pgrouting {
namespace contraction {

template < class G >
class Pgr_linear {
 private:
     typedef typename G::V V;
     typedef typename G::V_i V_i;
     typedef typename G::B_G B_G;


 public:
     void operator()(G &graph, Identifiers<V>& forbidden_vertices) {
         doContraction(graph, forbidden_vertices);
     }

     Pgr_linear():last_edge_id(0) {}

 private:
     int64_t get_next_id() {
         return --last_edge_id;
     }


 public:
     void setForbiddenVertices(
             Identifiers<V> forbidden_vertices) {
         m_forbiddenVertices = forbidden_vertices;
     }

     bool is_contractible(G &graph, V v) {
         return graph.is_linear(v) && !m_forbiddenVertices.has(v);
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
         m_forbiddenVertices = forbidden_vertices;
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

         if (graph.is_directed()) {
             /*
              *  u --> v --> w
              */
             process_shortcut(graph, u, v, w);
             /*
              *  w --> v --> u
              */
             process_shortcut(graph, w, v, u);

         } else {
             pgassert(graph.is_undirected());
             /*
              * u - v - w
              */
             process_shortcut(graph, u, v, w);
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



     /**
      *
      * u ----e1{v1}----> v ----e2{v2}----> w
      *
      * e1: min cost edge from u to v
      * e2: min cost edge from v to w
      *
      * result:
      * u ---{v+v1+v2}---> w
      *
      */
     void process_shortcut(G &graph, V u, V v, V w) {
         auto e1 = graph.get_min_cost_edge(u, v);
         auto e2 = graph.get_min_cost_edge(v, w);

         if (std::get<2>(e1) && std::get<2>(e2)) {
             auto contracted_vertices = std::get<1>(e1) + std::get<1>(e2);
             double cost = std::get<0>(e1) + std::get<0>(e2);
             contracted_vertices += graph[v].id;
             contracted_vertices += graph[v].contracted_vertices();

             // Create shortcut
             CH_edge shortcut(
                     get_next_id(),
                     graph[u].id,
                     graph[w].id,
                     cost);
             shortcut.contracted_vertices() = contracted_vertices;

             graph.add_shortcut(shortcut, u, w);
         }
     }


 private:
     Identifiers<V> m_linearVertices;
     Identifiers<V> m_forbiddenVertices;

     int64_t last_edge_id;
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_LINEARCONTRACTION_HPP_
