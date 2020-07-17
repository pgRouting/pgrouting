/*PGR-GNU*****************************************************************
File: pgr_contractionGraph.hpp

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

#ifndef INCLUDE_CONTRACTION_PGR_CONTRACTIONGRAPH_HPP_
#define INCLUDE_CONTRACTION_PGR_CONTRACTIONGRAPH_HPP_
#pragma once

#include <boost/graph/iteration_macros.hpp>

#include <limits>
#include <algorithm>
#include <vector>
#include <iostream>
#include <tuple>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/ch_vertex.h"
#include "cpp_common/ch_edge.h"


namespace pgrouting {
namespace graph {

template <class G>
class Pgr_contractionGraph : public Pgr_base_graph<G, CH_vertex, CH_edge> {
 public:
     typedef typename boost::graph_traits < G >::vertex_descriptor V;
     typedef typename boost::graph_traits < G >::edge_descriptor E;
     typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
     typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;

     /*!
       Prepares the _graph_ to be of type *gtype*
       */
     explicit Pgr_contractionGraph<G>(graphType gtype)
         : Pgr_base_graph<G, CH_vertex, CH_edge >(gtype) {
         }

     /*! @brief get the vertex descriptors of adjacent vertices of *v*
       @param [in] v vertex_descriptor
       @return Identifiers<V>: The set of vertex descriptors adjacent to the given vertex *v*
       */
     Identifiers<V> find_adjacent_vertices(V v) const {
         EO_i out, out_end;
         EI_i in, in_end;
         Identifiers<V> adjacent_vertices;

         for (boost::tie(out, out_end) = out_edges(v, this->graph);
                 out != out_end; ++out) {
             adjacent_vertices += this->adjacent(v, *out);
         }
         for (boost::tie(in, in_end) = in_edges(v, this->graph);
                 in != in_end; ++in) {
             adjacent_vertices += this->adjacent(v, *in);
         }
         return adjacent_vertices;
     }


     /*! @brief get the edge with minimum cost between two vertices
       @param [in] u vertex_descriptor of source vertex
       @param [in] v vertex_descriptor of target vertex
       @return E: The edge descriptor of the edge with minimum cost
       */
     std::tuple<double, Identifiers<int64_t>, bool>
     get_min_cost_edge(V u, V v) {
         E min_edge;
         Identifiers<int64_t> contracted_vertices;
         double min_cost = (std::numeric_limits<double>::max)();
         bool found = false;

         if (this->is_directed()) {
             BGL_FORALL_OUTEDGES_T(u, e, this->graph, G) {
                 if (this->target(e) == v) {
                     contracted_vertices += this->graph[e].contracted_vertices();
                     if (this->graph[e].cost < min_cost) {
                         min_cost = this->graph[e].cost;
                         min_edge = e;
                         found = true;
                     }
                 }
             }
             return std::make_tuple(min_cost, contracted_vertices, found);
         }

         pgassert(this->is_undirected());
         BGL_FORALL_OUTEDGES_T(u, e, this->graph, G) {
             if (this->adjacent(u, e) == v) {
                 contracted_vertices += this->graph[e].contracted_vertices();
                 if (this->graph[e].cost < min_cost) {
                     min_cost = this->graph[e].cost;
                     min_edge = e;
                     found = true;
                 }
             }
         }
         return std::make_tuple(min_cost, contracted_vertices, found);
     }


     /*! @brief print the graph with contracted vertices of
       all vertices and edges
       */
     friend
     std::ostream& operator <<(
             std::ostream &os,
             const Pgr_contractionGraph &g) {
         EO_i out, out_end;
         for (auto vi = vertices(g.graph).first;
                 vi != vertices(g.graph).second;
                 ++vi) {
             if ((*vi) >= g.num_vertices()) break;
             os << g.graph[*vi].id << "(" << (*vi) << ")"
                 << g.graph[*vi].contracted_vertices() << std::endl;
             os << " out_edges_of(" << g.graph[*vi].id << "):";
             for (boost::tie(out, out_end) = out_edges(*vi, g.graph);
                     out != out_end; ++out) {
                 os << ' ' << g.graph[*out].id
                     << "=(" << g.graph[g.source(*out)].id
                     << ", " << g.graph[g.target(*out)].id << ") = "
                     <<  g.graph[*out].cost <<"\t";
             }
             os << std::endl;
         }
         return os;
     }


     /*! @brief add_shortuct to the graph during contraction

       u -> v -> w

       u -> w

       edge (u, w) is a new edge e
       e.contracted_vertices = v + v.contracted vertices

       removed from graph edges: u -> v  and v -> w


       @param [in] edge of type *CH_edge* is to be added
       */

     void add_shortcut(const CH_edge &edge, V u, V v) {
         bool inserted;
         E e;
         if (edge.cost < 0) return;

         boost::tie(e, inserted) = boost::add_edge(u, v, this->graph);

         this->graph[e]= edge;
     }


     bool has_u_v_w(V u, V v, V w) const {
         return boost::edge(u, v, this->graph).second &&  boost::edge(v, w, this->graph).second;
     }

     /**
       Possibility of a shortcut from left vertex to right vertex
      *v* should be a linear vertex
      u <-> v -> w: v not considered linear

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
      u -- a0;
      w -- a1;
      }
      @enddot
      */
     bool is_shortcut_possible(
             V v,
             V u,
             V w) {
         pgassert(u != v);
         pgassert(v != w);
         pgassert(u != w);
         if (this->is_undirected()) {
             /*
              * u - v - w
              */
             return has_u_v_w(u, v, w);
         }

         pgassert(this->is_directed());
         return
             /*
              * u <-> v <-> w
              */
             (has_u_v_w(u, v, w) && has_u_v_w(w, v, u))
             /*
              * u -> v -> w
              */
             ||
             (has_u_v_w(u, v, w) && !(boost::edge(v, u, this->graph).second || boost::edge(w, v, this->graph).second))
             /*
              * u <- v <- w
              */
             ||
             (has_u_v_w(w, v, u) && !(boost::edge(v, w, this->graph).second || boost::edge(u, v, this->graph).second));
     }

     bool is_linear(V v) {
         // Checking adjacent vertices constraint
         auto adjacent_vertices = find_adjacent_vertices(v);

         if (adjacent_vertices.size() == 2) {
             // Checking u - v - w
             V u = adjacent_vertices.front();
             adjacent_vertices.pop_front();
             V w = adjacent_vertices.front();
             adjacent_vertices.pop_front();
             if (is_shortcut_possible(v, u, w)) {
                 return true;
             }
             return false;
         }
         return false;
     }
};

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_CONTRACTIONGRAPH_HPP_
