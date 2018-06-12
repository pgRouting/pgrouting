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
     typedef typename G::E E;
     typedef typename G::V_i V_i;
     typedef typename G::E_i E_i;
     typedef typename G::EO_i EO_i;
     typedef typename G::EI_i EI_i;
     typedef typename G::degree_size_type degree_size_type;


 public:
     Pgr_linear():last_edge_id(-1) {}
     void setForbiddenVertices(
             Identifiers<V> forbidden_vertices);
     void calculateVertices(G &graph);
     void doContraction(G &graph);

 private:
     int64_t get_next_id() {
         return --last_edge_id;
     }

     bool is_linear(G &graph, V v, std::ostringstream& debug);
     void add_if_linear(G &graph, V v);
     void add_edge_pair(V vertex, int64_t &incoming_eid,
             int64_t &outgoing_eid);
     bool is_shortcut_possible(G &graph, V v, V left, V right);
     void add_shortcut(G &graph, V vertex,
             E incoming_edge,
             E outgoing_edge, std::ostringstream& debug);
     void add_shortcut(G &graph,
             pgrouting::CH_edge &shortcut);

 private:
     Identifiers<V> linearVertices;
     Identifiers<V> forbiddenVertices;

     int64_t last_edge_id;
     std::ostringstream debug;
};

/*************** IMPLEMENTTION **************/

template < class G >
void
Pgr_linear< G >::setForbiddenVertices(
        Identifiers<V> forbidden_vertices) {
    debug << "Setting forbidden vertices\n";
    forbiddenVertices = forbidden_vertices;
}

/*
Possibility of a shortcut from left vertex to right vertex
*v* should be a linear vertex
*/
template < class G >
bool
Pgr_linear< G >::is_shortcut_possible(G &graph, V v, V left_vertex, V right_vertex) {
    auto in_degree_from_left_vertex = graph.in_degree_from_vertex(v, left_vertex);
    auto out_degree_to_right_vertex = graph.out_degree_to_vertex(v, right_vertex);
    auto in_degree_from_right_vertex = graph.in_degree_from_vertex(v, right_vertex);
    auto out_degree_to_left_vertex = graph.out_degree_to_vertex(v, left_vertex);
    return (in_degree_from_left_vertex > 0 && out_degree_to_right_vertex > 0 
           && in_degree_from_right_vertex == 0 && out_degree_to_left_vertex == 0)
            || (in_degree_from_left_vertex == 0 && out_degree_to_right_vertex == 0 
           && in_degree_from_right_vertex > 0 && out_degree_to_left_vertex > 0)
            || (in_degree_from_left_vertex > 0 && out_degree_to_right_vertex > 0 
           && in_degree_from_right_vertex > 0 && out_degree_to_left_vertex > 0);
}

template < class G >
bool Pgr_linear<G>::is_linear(G &graph, V v, std::ostringstream& debug) {


    if (forbiddenVertices.has(v)) {
        /**
         * - fobbiden_vertices
         *   - Not considered as linear
         */
        return false;
    }



    bool adjacent_vertices_constraint = false;
    bool degree_constraint = false;
    bool contracted_vertices_constraint = false;
    
    // Checking adjacent vertices constraint
    auto adjacent_vertices = graph.find_adjacent_vertices(v);

    if (adjacent_vertices.size() == 2) {
        adjacent_vertices_constraint = true;

        // Checking degree constraint
        V left_vertex = adjacent_vertices.front();
        adjacent_vertices.pop_front();
        V right_vertex = adjacent_vertices.front();
        adjacent_vertices.pop_front();
        if (is_shortcut_possible(graph, v, left_vertex, right_vertex)) {
            degree_constraint = true;
            return true;
        }
        return false;
    }

    return false;


}

template < class G >
void Pgr_linear<G>::calculateVertices(G &graph) {
    debug << "Calculating vertices\n";
    V_i vi;
    for (vi = vertices(graph.graph).first;
            vi != vertices(graph.graph).second;
            ++vi) {
        debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
        if (is_linear(graph, *vi, debug)) {
            linearVertices += (*vi);
        }
    }
    linearVertices -= forbiddenVertices;
}



template < class G >
void Pgr_linear<G>::doContraction(G &graph) {
    std::ostringstream contraction_debug;
    contraction_debug << "Performing contraction\n";

    std::priority_queue<V, std::vector<V>, std::greater<V> > linearPriority;
    for (const auto linearVertex : linearVertices) {
        linearPriority.push(linearVertex);
    }
    contraction_debug << "Linear vertices" << std::endl;
    for (const auto v : linearVertices) {
        contraction_debug << graph[v].id << ", ";
    }
    contraction_debug << std::endl;
    EI_i in, in_end;
    EO_i out, out_end;
    while (!linearPriority.empty()) {
        V current_vertex = linearPriority.top();
        linearPriority.pop();
        if (!is_linear(graph, current_vertex, contraction_debug)) {
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
        contraction_debug << "Adjacent vertices\n";
        contraction_debug << graph[v_1].id
            << ", " << graph[v_2].id
            << std::endl;

        if (graph.m_gType == DIRECTED) {

            /*
            Adding edge for every in edge and every out edge
            */
            for (boost::tie(in, in_end) = boost::in_edges(current_vertex, graph.graph);
                    in != in_end; ++in) {
                for (boost::tie(out, out_end) = boost::out_edges(current_vertex, graph.graph);
                        out != out_end; ++out) {
                    //append_shortcut(graph, current_vertex, *in, *out, shortcuts, debug);
                    if (graph.source(*in) == graph.target(*out)) {
                        continue;
                    }
                    add_shortcut(graph, current_vertex, *in, *out, debug);
                }       
            }


        } else if (graph.m_gType == UNDIRECTED) {
            // TODO: write logic afterwards
            #if 0
            if (graph.out_degree_to_vertex(vertex_1, current_vertex) > 0 &&
                    graph.in_degree_from_vertex(vertex_2, current_vertex) > 0) {
                contraction_debug << "UNDIRECTED graph before contraction\n";
                graph.print_graph(contraction_debug);
                E e1 = graph.get_min_cost_edge(vertex_1,
                        current_vertex);
                E e2 = graph.get_min_cost_edge(current_vertex,
                        vertex_2);
                add_shortcut(graph, current_vertex, e1, e2);
            }
            #endif
        }

        //Disconnecting the linear vertex after contraction
        graph.disconnect_vertex(current_vertex);
        graph[current_vertex].clear_contracted_vertices();

        //debug << "After removing vertex the graph is: " << graph << std::endl;
        //add_shortcuts(graph, shortcuts);


        linearVertices -= current_vertex;

        // Check if the neighbors of the linear vertex are also linear
        if (is_linear(graph, v_1, debug)
                && !forbiddenVertices.has(v_1) && !linearVertices.has(v_1)) {

            debug << "Adding linear vertex: " << graph[v_1].id << std::endl;
            linearPriority.push(v_1);
            linearVertices += v_1;
        }
        if (is_linear(graph, v_2, debug)
                && !forbiddenVertices.has(v_2) && !linearVertices.has(v_2)) {
            linearPriority.push(v_2);
            debug << "Adding linear vertex: " << graph[v_2].id << std::endl;
            linearVertices += v_2;
        }

    }
    debug << contraction_debug.str().c_str() << "\n";
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
        G &graph, V vertex,
        E incoming_edge,
        E outgoing_edge,
        std::ostringstream& debug) {

    // Create shortcut
    CH_edge shortcut(
            get_next_id(),
            graph[graph.source(incoming_edge)].id,
            graph[graph.target(outgoing_edge)].id,
            graph[incoming_edge].cost + graph[outgoing_edge].cost);

    // Add contracted vertices of the current linear vertex 
    shortcut.add_contracted_vertex(graph[vertex]);

    // Add contracted vertices of the incoming edge
    shortcut.add_contracted_edge_vertices(graph[incoming_edge]);

    // Add contracted vertices of the outgoing edge
    shortcut.add_contracted_edge_vertices(graph[outgoing_edge]);
    
    // Add shortcut to the graph
    debug << "Adding shortcut\n" << shortcut << std::endl;
    graph.add_shortcut(shortcut);
}


template < class G >
void Pgr_linear<G>::add_shortcut(G &graph,
        pgrouting::CH_edge &shortcut) {
    graph.add_shortcut(shortcut);
}

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_LINEARCONTRACTION_HPP_
