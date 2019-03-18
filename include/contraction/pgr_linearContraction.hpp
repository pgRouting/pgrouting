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
class Pgr_linear : public Pgr_messages {
 private:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::V_i V_i;
     typedef typename G::E_i E_i;
     typedef typename G::EO_i EO_i;
     typedef typename G::EI_i EI_i;
     typedef typename G::degree_size_type degree_size_type;
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
         log << "Setting forbidden vertices\n";
         m_forbiddenVertices = forbidden_vertices;
     }

     /**
       Possibility of a shortcut from left vertex to right vertex
      *v* should be a linear vertex

      @dot
      graph G {
      graph [rankdir=LR];
      subgraph cluster0 {
      node [shape=point,height=0.2,style=filled,color=black];
      style=filled;
      color=lightgrey;
      a0; a1; a2;
      label = "rest of graph";
      }
      v [color=green];
      v -- left;
      v -- right;
      u - a0;
      w - a1;
      }
      @enddot
      */
 private:
     bool is_shortcut_possible(
             G &graph,
             V v,
             V u,
             V w) {
         pgassert(u != v);
         pgassert(v != w);
         pgassert(u != w);
         if (graph.is_undirected()) {
             /*
              * u - v - w
              */
             log << "\tundirected";
             return graph.has_u_v_w(u, v, w);
         }

         pgassert(graph.is_directed());
         log << "\tdirected";
         return
             /*
              * u -> v -> w
              */
             graph.has_u_v_w(u, v, w)
             /*
              * u <- v <- w
              */
             || graph.has_u_v_w(w, v, u);
     }

     bool is_linear(G &graph, V v) {
         log << "\tChecking vertex " << graph[v].id << '\n';

         // Checking adjacent vertices constraint
         auto adjacent_vertices = graph.find_adjacent_vertices(v);

         if (adjacent_vertices.size() == 2) {
             log << "\t2 adjacent vertices";

             // Checking u - v - w
             V u = adjacent_vertices.front();
             adjacent_vertices.pop_front();
             V w = adjacent_vertices.front();
             adjacent_vertices.pop_front();
             if (is_shortcut_possible(graph, v, u, w)) {
                 log << graph.graph[v].id << "\t ******* is linear !!" << std::endl;
                 return true;
             }
             log << graph.graph[v].id << "\t ******* is not linear !!" << std::endl;
             return false;
         }
         log << graph.graph[v].id << "\t ******* is not linear !!" << std::endl;
         return false;
     }

     void calculateVertices(G &graph) {
         m_linearVertices.clear();
         V_i vi;
         BGL_FORALL_VERTICES_T(v, graph.graph, B_G) {
             if (is_linear(graph, v)) {
                 m_linearVertices += v;
             }
         }
         m_linearVertices -= m_forbiddenVertices;
     }



     void doContraction(G &graph, Identifiers<V> forbidden_vertices) {
         m_forbiddenVertices = forbidden_vertices;
         calculateVertices(graph);
         log << "Performing contraction\n";
         log << "Linear vertices" << std::endl;
         for (const auto v : m_linearVertices) {
             log << graph[v].id << ", ";
         }
         log << std::endl;
#if 0
         EI_i in, in_end;
         EO_i out, out_end;
#endif
         while (!m_linearVertices.empty()) {
             V current_vertex = m_linearVertices.front();
             m_linearVertices -= current_vertex;
             pgassert(is_linear(graph, current_vertex));
             if (!is_linear(graph, current_vertex)) {
                 m_linearVertices -= current_vertex;
                 continue;
             }
             Identifiers<V> adjacent_vertices =
                 graph.find_adjacent_vertices(current_vertex);
             pgassert(adjacent_vertices.size() == 2);

             V v_1 = adjacent_vertices.front();
             adjacent_vertices.pop_front();
             V v_2 = adjacent_vertices.front();
             adjacent_vertices.pop_front();

             // Adjacent vertices of the current linear vertex
             log << "Adjacent vertices:\t" << graph[v_1].id
                 << ", " << graph[v_2].id
                 << std::endl;

             if (graph.m_gType == DIRECTED) {
                 /*
                    Adding edge for every in edge and every out edge

                    u -e1_1-> v -e1_2-> w
                    w -e2_1-> v -e2_2-> u
                    */
                 auto v = current_vertex;
                 V u = v_1;
                 V w = v_2;
                 pgassert(v != u);
                 pgassert(v != w);
                 pgassert(u != w);

#if 1
                 process_shortcut(graph, u, v, w);
                 process_shortcut(graph, w, v, u);
#else
                 auto e1_1 = graph.get_min_cost_edge(u, v);
                 auto e1_2 = graph.get_min_cost_edge(v, w);
                 auto contracted_vertices = std::get<1>(e1_1) + std::get<1>(e1_2);

                 if (std::get<2>(e1_1) && std::get<2>(e1_2)) {
                     log <<std::get<0>(e1_1)<< std::get<0>(e1_2);
                     add_shortcut(graph, v, std::get<0>(e1_1), std::get<0>(e1_2), contracted_vertices);
                 }

                 auto e2_1 = graph.get_min_cost_edge(w, v);
                 auto e2_2 = graph.get_min_cost_edge(v, u);
                 auto contracted_vertices = std::get<1>(e2_1) + std::get<1>(e2_2);


                 if (std::get<2>(e2_1) && std::get<2>(e2_2)) {
                     log <<std::get<0>(e2_1)<< std::get<0>(e2_2);
                     add_shortcut(graph, v, std::get<0>(e2_1), std::get<0>(e2_2), contracted_vertices);
                 }
#endif

                 m_linearVertices -= current_vertex;
             } else if (graph.m_gType == UNDIRECTED) {
                 /*
                  * u - v - w
                  * e1 - e2
                  * with smallest cost
                  *
                  */
                 auto v = current_vertex;
                 V u = v_1;
                 V w = v_2;
                 pgassert(v != u);
                 pgassert(v != w);
                 pgassert(u != w);

#if 1
                 process_shortcut(graph, u, v, w);
#else
                 auto e1_1 = graph.get_min_cost_edge(u, v);
                 auto e1_2 = graph.get_min_cost_edge(v, w);
                 auto contracted_vertices = std::get<1>(e1_1) + std::get<1>(e1_2);

                 if (std::get<2>(e1_1) && std::get<2>(e1_2)) {
                     log <<std::get<0>(e1_1)<< std::get<0>(e1_2);
                     add_shortcut(graph, v, std::get<0>(e1_1), std::get<0>(e1_2), contracted_vertices);
                 }
#endif
                 m_linearVertices -= current_vertex;
             }

             log << "checking neighbor vertices";


             if (is_linear(graph, v_1) && !m_forbiddenVertices.has(v_1)) {
                 log << "Adding linear vertex: " << graph[v_1].id << std::endl;
                 m_linearVertices += v_1;
             } else {
                 m_linearVertices -= v_1;
             }
             if (is_linear(graph, v_2) && !m_forbiddenVertices.has(v_2)) {
                 log << "Adding linear vertex: " << graph[v_2].id << std::endl;
                 m_linearVertices += v_2;
             } else {
                 m_linearVertices -= v_2;
             }

         }
         //log << contraction_debug.str().c_str() << "\n";
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
         auto contracted_vertices = std::get<1>(e1) + std::get<1>(e2);
         contracted_vertices += graph[v].id;

         if (std::get<2>(e1) && std::get<2>(e2)) {
             log <<std::get<0>(e1)<< std::get<0>(e2);
             add_shortcut(graph, v, std::get<0>(e1), std::get<0>(e2), contracted_vertices);
         }
     }


     /*! \brief add edges(shortuct) to the graph during contraction

       a --incomming--> b ---outgoing--> c

       a -> c

       edge (a, c) is a new edge: @b shortcut
       e.contracted_vertices = b + b.contracted vertices
       e.contracted_vertices += (a->b).contracted vertices
       e.contracted_vertices += (b->c).contracted vertices
       b is "removed" disconnected from the graph
       - by removing all edges to/from b
       */

     void add_shortcut(
             G &graph, V v,
             E incoming_edge,
             E outgoing_edge,
             Identifiers<int64_t> contracted_vertices) {

         auto u = graph.adjacent(v, incoming_edge);
         auto w = graph.adjacent(v, outgoing_edge);

         log << incoming_edge << outgoing_edge;
         log << "(" << graph[u].id << ", " << graph[w].id << ")";
         // Create shortcut
         CH_edge shortcut(
                 get_next_id(),
                 graph[u].id,
                 graph[w].id,
                 graph[incoming_edge].cost + graph[outgoing_edge].cost);

         // Add contracted vertices of the current linear vertex
         shortcut.add_contracted_vertex(graph[v]);
         shortcut.contracted_vertices() += contracted_vertices;

         // Add shortcut to the graph
         log << "\nAdding shortcut\t" << shortcut << std::endl;
         log << "\ngraph\t" << graph << std::endl;
         graph.add_shortcut(shortcut);
         log << "\ngraph\t" << graph << std::endl;

         boost::remove_edge(u, v, graph.graph);
         boost::remove_edge(v, w, graph.graph);
         graph.shortcuts -= incoming_edge;
         graph.shortcuts -= outgoing_edge;
         log << "\ngraph\t" << graph << std::endl;
     }


 private:
     Identifiers<V> m_linearVertices;
     Identifiers<V> m_forbiddenVertices;

     int64_t last_edge_id;
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_LINEARCONTRACTION_HPP_
