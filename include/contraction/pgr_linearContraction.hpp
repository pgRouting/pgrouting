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
     Pgr_linear():last_edge_id(0) {}
     void setForbiddenVertices(
             Identifiers<V> forbidden_vertices);
     void calculateVertices(G &graph);
     void doContraction(G &graph);

 private:
     int64_t get_next_id() {
         return --last_edge_id;
     }

     bool is_linear(G &graph, V v);
     void add_if_linear(G &graph, V v);
     void add_edge_pair(V vertex, int64_t &incoming_eid,
             int64_t &outgoing_eid);
     bool is_shortcut_possible(G &graph, V v, V left, V right);
     void add_shortcut(G &graph, V vertex,
             E incoming_edge,
             E outgoing_edge);
     void add_shortcut(G &graph,
             pgrouting::CH_edge &shortcut);

 private:
     Identifiers<V> linearVertices;
     Identifiers<V> forbiddenVertices;

     int64_t last_edge_id;
};

/*************** IMPLEMENTTION **************/

template < class G >
void
Pgr_linear< G >::setForbiddenVertices(
        Identifiers<V> forbidden_vertices) {
    log << "Setting forbidden vertices\n";
    forbiddenVertices = forbidden_vertices;
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
template < class G >
bool
Pgr_linear< G >::is_shortcut_possible(G &graph, V v, V u, V w) {
    if (graph.is_undirected()) {
        /*
         * u - v - w
         */
        log << "\tundirected";
        return boost::edge(u, v, graph.graph).second &&  boost::edge(v, w,  graph.graph).second;
    }

    pgassert(graph.is_directed());
    log << "\tdirected";
    return
        /*
         * u -> v -> w
         */
        (boost::edge(u, v, graph.graph).second &&  boost::edge(v, w,  graph.graph).second)
        /*
         * u <- v <- w
         */
        ||  (boost::edge(w, v, graph.graph).second &&  boost::edge(v, u,  graph.graph).second);

#if 0
    auto in_degree_from_left_vertex = graph.in_degree_from_vertex(v, u);
    auto out_degree_to_right_vertex = graph.out_degree_to_vertex(v, w);
    auto in_degree_from_right_vertex = graph.in_degree_from_vertex(v, w);
    auto out_degree_to_left_vertex = graph.out_degree_to_vertex(v, u);
    return (in_degree_from_left_vertex > 0 && out_degree_to_right_vertex > 0
           && in_degree_from_right_vertex == 0 && out_degree_to_left_vertex == 0)
            || (in_degree_from_left_vertex == 0 && out_degree_to_right_vertex == 0
           && in_degree_from_right_vertex > 0 && out_degree_to_left_vertex > 0)
            || (in_degree_from_left_vertex > 0 && out_degree_to_right_vertex > 0
           && in_degree_from_right_vertex > 0 && out_degree_to_left_vertex > 0);
#endif
}

template < class G >
bool Pgr_linear<G>::is_linear(G &graph, V v) {
    log << "\tChecking vertex " << graph[v].id << '\n';


#if 0
    if (forbiddenVertices.has(v)) {
        /**
         * - fobbiden_vertices
         *   - Not considered as linear
         */
        log << graph.graph[v].id << " is forbidden !!" << std::endl;
        return false;
    }
#endif


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

template < class G >
void Pgr_linear<G>::calculateVertices(G &graph) {
    log << "\n" << __PRETTY_FUNCTION__ << "\n";
    V_i vi;
    BGL_FORALL_VERTICES_T(v, graph.graph, B_G) {
        if (is_linear(graph, v)) {
            linearVertices += v;
        }
    }
    linearVertices -= forbiddenVertices;
}



template < class G >
void Pgr_linear<G>::doContraction(G &graph) {
    log << "Performing contraction\n";

    std::priority_queue<V, std::vector<V>, std::greater<V> > linearPriority;
    for (const auto linearVertex : linearVertices) {
        linearPriority.push(linearVertex);
    }
    log << "Linear vertices" << std::endl;
    for (const auto v : linearVertices) {
        log << graph[v].id << ", ";
    }
    log << std::endl;
    EI_i in, in_end;
    EO_i out, out_end;
    while (!linearPriority.empty()) {
        V current_vertex = linearPriority.top();
        linearPriority.pop();
        if (!is_linear(graph, current_vertex)) {
            linearVertices -= current_vertex;
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

            auto e1_1 = graph.get_min_cost_edge(u, v);
            auto e1_2 = graph.get_min_cost_edge(v, w);
            auto e2_1 = graph.get_min_cost_edge(w, v);
            auto e2_2 = graph.get_min_cost_edge(v, u);

            if (e1_1.second && e1_2.second) {
                log << e1_1.first << e1_2.first;
                add_shortcut(graph, v, e1_1.first, e1_2.first);
            }

            if (e2_1.second && e2_2.second) {
                add_shortcut(graph, v, e2_1.first, e2_2.first);
            }

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

            E e1;
            E e2;
            double min_e1 = (std::numeric_limits<double>::max)();
            double min_e2 = (std::numeric_limits<double>::max)();
            BGL_FORALL_OUTEDGES_T(v, e, graph.graph, B_G) {
                log << "\n*****cycling" << e;
                if (graph.adjacent(v, e) == u && graph[e].cost < min_e1) {
                    min_e1 = graph[e].cost;
                    e1 = e;
                }

                if (graph.adjacent(v, e) == w && graph[e].cost < min_e2) {
                    min_e2 = graph[e].cost;
                    e2 = e;
                }
            }
            log << "\tshortest path cost" << min_e1 + min_e2;
            add_shortcut(graph, v, e1, e2);
            log << "\nafter shorcut\n" << graph;
            graph.disconnect_vertex(v);
            log << "\nafter disconnect\n" << graph;
            graph[current_vertex].clear_contracted_vertices();
            linearVertices -= current_vertex;
            continue;
        }


        //Disconnecting the linear vertex after contraction
        graph.disconnect_vertex(current_vertex);
        graph[current_vertex].clear_contracted_vertices();

        //log << "After removing vertex the graph is: " << graph << std::endl;
        //add_shortcuts(graph, shortcuts);


        linearVertices -= current_vertex;

        // Check if the neighbors of the linear vertex are also linear
        if (is_linear(graph, v_1)
                && !forbiddenVertices.has(v_1) && !linearVertices.has(v_1)) {

            log << "Adding linear vertex: " << graph[v_1].id << std::endl;
            linearPriority.push(v_1);
            linearVertices += v_1;
        }
        if (is_linear(graph, v_2)
                && !forbiddenVertices.has(v_2) && !linearVertices.has(v_2)) {
            linearPriority.push(v_2);
            log << "Adding linear vertex: " << graph[v_2].id << std::endl;
            linearVertices += v_2;
        }

    }
    //log << contraction_debug.str().c_str() << "\n";
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

template < class G >
void Pgr_linear<G>::add_shortcut(
        G &graph, V v,
        E incoming_edge,
        E outgoing_edge) {

    auto u = graph.adjacent(v, incoming_edge);
    auto w = graph.adjacent(v, outgoing_edge);

    log << incoming_edge << outgoing_edge;
    log << "(" << graph[u].id << ", "
        << graph[w].id << ")";
    // Create shortcut
    CH_edge shortcut(
            get_next_id(),
            graph[u].id,
            graph[w].id,
            graph[incoming_edge].cost + graph[outgoing_edge].cost);

    log << "shorcut to be added" << shortcut;

    // Add contracted vertices of the current linear vertex
    shortcut.add_contracted_vertex(graph[v]);

    // Add contracted vertices of the incoming edge
    shortcut.add_contracted_edge_vertices(graph[incoming_edge]);

    // Add contracted vertices of the outgoing edge
    shortcut.add_contracted_edge_vertices(graph[outgoing_edge]);

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


template < class G >
void Pgr_linear<G>::add_shortcut(G &graph,
        pgrouting::CH_edge &shortcut) {
    graph.add_shortcut(shortcut);
}

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_LINEARCONTRACTION_HPP_
