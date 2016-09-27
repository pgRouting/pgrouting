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

#include <queue>
#include <functional>
#include <vector>
#include "../../common/src/identifiers.hpp"
namespace pgrouting {

template < class G >
class Pgr_deadEndContraction {
 private:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::V_i V_i;
     typedef typename G::E_i E_i;
     typedef typename G::EO_i EO_i;
     typedef typename G::EI_i EI_i;

 public:
     void setForbiddenVertices(
             Identifiers<V> forbidden_vertices);

     void calculateVertices(G &graph);
     /*! @brief true when \b v is a dead end
      *
      * - undirected: the condition in case of undirected graph
      *   - is all incoming edges come from a single vertex
      *   - regardless of the number of edges:
      *   @dot undirected dead end {
      *   b [color=green];
      *   rest - a - b;
      *   rest - a - b;
      *   }
      *
      */
     bool is_dead_end(G &graph, V v);
     void add_if_dead_end(G &graph, V v);
     void doContraction(G &graph);

 private:
     Identifiers<V> deadendVertices;
     Identifiers<V> forbiddenVertices;
     std::ostringstream debug;
};

/******* IMPLEMENTATION ************/
template < class G >
void
Pgr_deadEndContraction< G >::setForbiddenVertices(
        Identifiers<V> forbidden_vertices) {
    debug << "Setting forbidden vertices\n";
    forbiddenVertices = forbidden_vertices;
}


template < class G >
void Pgr_deadEndContraction<G>::calculateVertices(G &graph) {
    debug << "Calculating vertices\n";
    V_i vi;
    for (vi = vertices(graph.graph).first;
            vi != vertices(graph.graph).second;
            ++vi) {
        debug << "Checking vertex " << graph[(*vi)].id << '\n';
        if (is_dead_end(graph, *vi)) {
            debug << "Adding " << graph[(*vi)].id << " to dead end" << '\n';
            deadendVertices += (*vi);
        }
    }
    deadendVertices -= forbiddenVertices;
}

template < class G >
bool Pgr_deadEndContraction<G>::is_dead_end(G &graph, V v) {
    debug << "Is dead end: " << graph.graph[v].id << "?\n";

    if (graph.is_undirected()) {
        Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(v);
#ifndef NDEBUG
        debug << "undirected\nAdjacent Vertices\n";
        debug << adjacent_vertices;
#endif
        //  only one adjacent vertex
        if (adjacent_vertices.size() == 1) {
            debug << "Is Dead End\n";
            return true;
        }
        debug << "Is Not Dead End\n";
        return false;
    }

    /*
     * directed graph
     */
    if (graph.out_degree(v) == 1 && graph.in_degree(v) == 1) {
        int64_t incoming_edge_id = -1;
        int64_t outgoing_edge_id = -2;
        EO_i out, out_end;
        EI_i in, in_end;
        for (boost::tie(out, out_end) = out_edges(v, graph.graph);
                out != out_end; ++out) {
            outgoing_edge_id = graph[*out].id;
        }
        for (boost::tie(in, in_end) = in_edges(v, graph.graph);
                in != in_end; ++in) {
            incoming_edge_id = graph[*in].id;
        }
        if (incoming_edge_id == outgoing_edge_id) {
            debug << "Is Dead End\n";
            return true;
        }
        debug << "Is Not Dead End\n";
        return false;
    }
    //  additional cases
    if (graph.out_degree(v) == 0 && graph.in_degree(v) > 0) {
        debug << "Is Dead End\n";
        return true;
    }
#if 0
    //  dead start
    if (graph.in_degree(v) == 0 && graph.out_degree(v) > 0) {
        debug << "Is Dead End\n";
        return true;
    }
#endif
    debug << "Is Not Dead End\n";
    return false;
}

template < class G >
void
Pgr_deadEndContraction<G>::add_if_dead_end(G &graph, V v) {
    if (is_dead_end(graph, v)) {
        deadendVertices += v;
    }
}

template < class G >
void
Pgr_deadEndContraction<G>::doContraction(G &graph) {
    debug << "Performing contraction\n";
    std::priority_queue<V, std::vector<V>, std::greater<V> > deadendPriority;
    for (V deadendVertex : deadendVertices) {
        deadendPriority.push(deadendVertex);
    }
    while (!deadendPriority.empty()) {
        V current_vertex = deadendPriority.top();
        deadendPriority.pop();
        if (!is_dead_end(graph, current_vertex)) {
            continue;
        }
        Identifiers<V> adjacent_vertices =
            graph.find_adjacent_vertices(current_vertex);
        for (auto adjacent_vertex : adjacent_vertices) {
            debug << "Contracting current vertex "
               << graph[current_vertex].id << std::endl;
            graph[adjacent_vertex].add_contracted_vertex(
                    graph[current_vertex], current_vertex);
            //  Adding contracted vertices of the edge
            EO_i out, out_end;
            EI_i in, in_end;
            debug << "Adding contracted vertices of the edge\n";
            for (boost::tie(out, out_end) =
                    out_edges(current_vertex, graph.graph);
                    out != out_end;
                    ++out) {
                debug << graph.graph[*out];
                graph.add_contracted_edge_vertices(
                        adjacent_vertex, graph[*out]);
            }
            for (boost::tie(in, in_end) = in_edges(current_vertex, graph.graph);
                    in != in_end; ++in) {
                debug << graph.graph[*in];
                graph.add_contracted_edge_vertices(adjacent_vertex, graph[*in]);
            }
            debug << "Current Vertex:\n";
            debug << graph[current_vertex];
            debug << "Adjacent Vertex:\n";
            debug << graph[adjacent_vertex];
            graph.disconnect_vertex(debug, current_vertex);
            deadendVertices -= current_vertex;
            debug << "Adjacent vertex dead_end?: "
                << is_dead_end(graph, adjacent_vertex)
                << std::endl;
            if (is_dead_end(graph, adjacent_vertex)
                    && !forbiddenVertices.has(adjacent_vertex)) {
                deadendVertices += adjacent_vertex;
                deadendPriority.push(adjacent_vertex);
            }
        }
    }
}

}  // namespace pgrouting
