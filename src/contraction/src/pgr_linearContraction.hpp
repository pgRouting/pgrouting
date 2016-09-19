/*PGR-GNU*****************************************************************
File: pgr_contractionGraph.c

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
#pragma once
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif
#include <functional>
#include <queue>
#include <vector>
#include "../../common/src/identifiers.hpp"

namespace pgrouting {

template < class G >
class Pgr_linearContraction {
 private:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::V_i V_i;
     typedef typename G::E_i E_i;
     typedef typename G::EO_i EO_i;
     typedef typename G::EI_i EI_i;
     typedef typename G::degree_size_type degree_size_type;


 public:
     Pgr_linearContraction():last_edge_id(0) {}
     void setForbiddenVertices(
             Identifiers<V> forbidden_vertices);
     void calculateVertices(G &graph);
     void doContraction(G &graph);

 private:
     int64_t& get_next_id() {
         return --last_edge_id;
     }

     bool is_linear(G &graph, V v);
     void add_if_linear(G &graph, V v);
     void add_edge_pair(V vertex, int64_t &incoming_eid,
             int64_t &outgoing_eid);
     void add_shortcut(G &graph, V vertex,
             E incoming_edge,
             E outgoing_edge);
     void add_shortcut(G &graph,
             pgrouting::contraction::Edge &shortcut);

 private:
     Identifiers<V> linearVertices;
     Identifiers<V> forbiddenVertices;

     int64_t last_edge_id;
     std::ostringstream debug;
};

/*************** IMPLEMENTTION **************/

template < class G >
void
Pgr_linearContraction< G >::setForbiddenVertices(
        Identifiers<V> forbidden_vertices) {
    debug << "Setting forbidden vertices\n";
    forbiddenVertices = forbidden_vertices;
}


template < class G >
bool Pgr_linearContraction<G>::is_linear(G &graph, V v) {
    degree_size_type in_degree, out_degree;
    in_degree = graph.in_degree(v);
    out_degree = graph.out_degree(v);
    Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(v);
    if (adjacent_vertices.size() == 2) {
        if (in_degree > 0 && out_degree > 0) {
            debug << graph.graph[v].id << " is linear " << std::endl;
            return true;
        }
    }
    debug << graph.graph[v].id << " is not linear " << std::endl;
    return false;
}

template < class G >
void Pgr_linearContraction<G>::calculateVertices(G &graph) {
    debug << "Calculating vertices\n";
    V_i vi;
    for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
        debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
        if (is_linear(graph, *vi)) {
            // debug << "Adding " << graph.graph[(*vi)].id << " to linear" << '\n';
            linearVertices += (*vi);
        }
    }
    linearVertices -= forbiddenVertices;
}



template < class G >
void Pgr_linearContraction<G>::doContraction(G &graph) {
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
    while (!linearPriority.empty()) {
        V current_vertex = linearPriority.top();
        linearPriority.pop();
        if (!is_linear(graph, current_vertex)) {
            linearVertices -= current_vertex;
            continue;
        }
        Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(current_vertex);
        pgassert(adjacent_vertices.size() == 2);
        V vertex_1 = adjacent_vertices[0];
        V vertex_2 = adjacent_vertices[1];
        contraction_debug << "Adjacent vertices\n";
        contraction_debug << graph[vertex_1].id << ", " << graph[vertex_2].id << std::endl;

        if (graph.m_gType == DIRECTED) {
            if (graph.out_degree_to_vertex(vertex_1, current_vertex) > 0
                    && graph.in_degree_from_vertex(vertex_2, current_vertex) > 0) {
                E e1 = graph.get_min_cost_edge(vertex_1,
                        current_vertex);
                E e2 = graph.get_min_cost_edge(current_vertex,
                        vertex_2);
                add_shortcut(graph, current_vertex, e1, e2);
            }

            if (graph.out_degree_to_vertex(vertex_2, current_vertex) > 0
                    && graph.in_degree_from_vertex(vertex_1, current_vertex) > 0) {
                E e1 = graph.get_min_cost_edge(vertex_2,
                        current_vertex);
                E e2 = graph.get_min_cost_edge(current_vertex,
                        vertex_1);
                add_shortcut(graph, current_vertex, e1, e2);
            }
        } else if (graph.m_gType == UNDIRECTED) {
            if (graph.out_degree_to_vertex(vertex_1, current_vertex) > 0
                    && graph.in_degree_from_vertex(vertex_2, current_vertex) > 0) {
                contraction_debug << "UNDIRECTED graph before contraction\n";
                graph.print_graph(contraction_debug);
                E e1 = graph.get_min_cost_edge(vertex_1,
                        current_vertex);
                E e2 = graph.get_min_cost_edge(current_vertex,
                        vertex_2);
                add_shortcut(graph, current_vertex, e1, e2);
            }
        }

        graph.disconnect_vertex(contraction_debug, current_vertex);
        linearVertices -= current_vertex;
        if (is_linear(graph, vertex_1)
                && !forbiddenVertices.has(vertex_1)) {
            linearPriority.push(vertex_1);
            linearVertices += vertex_1;
        }
        if (is_linear(graph, vertex_2)
                && !forbiddenVertices.has(vertex_2)) {
            linearPriority.push(vertex_2);
            linearVertices += vertex_2;
        }
    }
    debug << contraction_debug.str().c_str() << "\n";
}



template < class G >
void Pgr_linearContraction<G>::add_shortcut(G &graph, V vertex,
        E incoming_edge,
        E outgoing_edge) {
    if (graph.m_gType == UNDIRECTED) {
        Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(vertex);
        V vertex_1 = adjacent_vertices[0];
        V vertex_2 = adjacent_vertices[1];
        E shortcut_E;
        contraction::Edge shortcut(get_next_id(), graph[vertex_1].id,
                graph[vertex_2].id,
                graph[incoming_edge].cost + graph[outgoing_edge].cost);
        shortcut.add_contracted_vertex(graph[vertex], vertex);
        shortcut.add_contracted_edge_vertices(graph[incoming_edge]);
        shortcut.add_contracted_edge_vertices(graph[outgoing_edge]);
        debug << "Adding shortcut\n";
        debug << shortcut;
        graph.add_shortcut(shortcut);
        debug << "Added shortcut\n";
    } else if (graph.m_gType == DIRECTED) {
        contraction::Edge shortcut(get_next_id(), graph[incoming_edge].source,
                graph[outgoing_edge].target,
                graph[incoming_edge].cost + graph[outgoing_edge].cost);
        shortcut.add_contracted_vertex(graph[vertex], vertex);
        shortcut.add_contracted_edge_vertices(graph[incoming_edge]);
        shortcut.add_contracted_edge_vertices(graph[outgoing_edge]);
        debug << "Adding shortcut\n";
        debug << shortcut;
        graph.add_shortcut(shortcut);
        debug << "Added shortcut\n";
    }
}
template < class G >
void Pgr_linearContraction<G>::add_shortcut(G &graph,
        pgrouting::contraction::Edge &shortcut) {
    graph.add_shortcut(shortcut);
}

}  // namespace pgrouting
