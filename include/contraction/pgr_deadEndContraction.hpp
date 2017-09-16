/*PGR-GNU*****************************************************************
File: pgr_deadend.hpp

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

#ifndef INCLUDE_CONTRACTION_PGR_DEADENDCONTRACTION_HPP_
#define INCLUDE_CONTRACTION_PGR_DEADENDCONTRACTION_HPP_
#pragma once


#include <queue>
#include <functional>
#include <vector>
#include "cpp_common/identifiers.hpp"

namespace pgrouting {
namespace contraction {

template < class G >
class Pgr_deadend {
 private:
     typedef typename G::V V;
     typedef typename G::E E;

 public:
     void setForbiddenVertices(
             Identifiers<V> forbidden_vertices);

     void calculateVertices(G &graph);
     /// @brief true when \b v is a dead end
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
Pgr_deadend< G >::setForbiddenVertices(
        Identifiers<V> forbidden_vertices) {
#ifndef NDEBUG
    debug << "Setting forbidden vertices\n";
#endif
    forbiddenVertices = forbidden_vertices;
}


template < class G >
void Pgr_deadend<G>::calculateVertices(G &graph) {
    debug << "Calculating vertices\n";

    for (auto vi = vertices(graph.graph).first;
            vi != vertices(graph.graph).second;
            ++vi) {
#ifndef NDEBUG
        debug << "Checking vertex " << graph[(*vi)].id << '\n';
#endif
        if (is_dead_end(graph, *vi)) {
#ifndef NDEBUG
            debug << "Adding " << graph[(*vi)].id << " to dead end" << '\n';
#endif
            deadendVertices += (*vi);
        }
    }
    deadendVertices -= forbiddenVertices;
}

template < class G >
bool Pgr_deadend<G>::is_dead_end(G &graph, V v) {
#ifndef NDEBUG
    debug << "Is dead end: " << graph.graph[v].id << "?\n";
#endif

    if (forbiddenVertices.has(v)) {
        /**
         * - fobbiden_vertices
         *   - Not considered as dead end
         */

        return false;
    }

    if (graph.is_undirected()) {
        /**
         * undirected:
         * ----------
         *   - There is only one adjacent vertex:
         *   - All adjcent edges are from a single vertex
         *

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
         v -- a0;
         v -- a0;
         }
         @enddot

         */
        Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(v);
        if (adjacent_vertices.size() == 1) {
            return true;
        }
        return false;
    }

    pgassert(graph.is_directed());
    /*
     * directed graph
     *
     * is dead end when:
     *  (2) one incoming edge, no outgoing edge (dead end)
     *  (3) one outgoing edge, one incoming edge
     *       and both are from/to the same vertex
     *  (4) many incoming edges
     *       and no outgoing edges
     *  (5) many outgoing edges TODO but all go to same vertex
     *       and no incoming edges
     *
     * NOT dead end when:
     *  (3) one outgoing edge, one incoming edge
     *       and both from/to different vertex
     *
     * note: when contracting case 4 & 5, the vertex has to be
     *       part of all the adjacent vertices
     */

    if (graph.in_degree(v) == 0 && graph.out_degree(v) == 1) {
        /**
         * directed
         * ----------
         *  case (1):  (dead start)
         *   - one outgoing edge,
         *   - no incoming edge
         *

         @dot
         digraph G {
         graph [rankdir=LR];
         subgraph cluster0 {
         node [shape=point,height=0.2,style=filled,color=black];
         style=filled;
         color=lightgrey;
         a0; a1; a2;
         label = "rest of graph";
         }
         v [color=green];
         v -> a0;
         }
         @enddot

         */
        return true;
    }

    if (graph.in_degree(v) == 1 && graph.out_degree(v) == 0) {
        /**
         * case (2):  (dead end)
         *   - no outgoing edge,
         *   - one incoming edge
         *

         @dot
         digraph G {
         graph [rankdir=LR];
         subgraph cluster0 {
         node [shape=point,height=0.2,style=filled,color=black];
         style=filled;
         color=lightgrey;
         a0; a1; a2;
         label = "rest of graph";
         }
         v [color=green];
         a0 -> v;
         }
         @enddot

         */
        return true;
    }

    if (graph.out_degree(v) == 1 && graph.in_degree(v) == 1) {
        /**
         * case (3):
         *   - one outgoing edge,
         *   - one incoming edge
         *   - one adjacent vertex
         *

         @dot
         digraph G {
         graph [rankdir=LR];
         subgraph cluster0 {
         node [shape=point,height=0.2,style=filled,color=black];
         style=filled;
         color=lightgrey;
         a0; a1; a2;
         label = "rest of graph";
         }
         v [color=green];
         v -> a0;
         a0 -> v;
         }
         @enddot

         */
        auto out_e = *(out_edges(v, graph.graph).first);
        auto in_e = *(in_edges(v, graph.graph).first);

        auto out_v = graph.is_source(v, out_e) ?
            graph.target(out_e) : graph.source(out_e);
        auto in_v = graph.is_source(v, in_e) ?
            graph.target(in_e) : graph.source(in_e);

        if (out_v == in_v) {
            return true;
        }
        return false;
    }

    if (graph.in_degree(v) > 0 && graph.out_degree(v) == 0) {
        /**
         * case (4):
         *   - no outgoing edge,
         *   - many incoming edges
         *
         *

         @dot
         digraph G {
         graph [rankdir=LR];
         subgraph cluster0 {
         node [shape=point,height=0.2,style=filled,color=black];
         style=filled;
         color=lightgrey;
         a0; a1; a2;
         label = "rest of graph";
         }
         v [color=green];
         a0 -> v;
         a1 -> v;
         a0 -> v;
         }
         @enddot

         */
        return true;
    }

    if (graph.in_degree(v) > 0 && graph.out_degree(v) > 0) {
        /**
         * case (5):
         *   - many outgoing edge,
         *   - many incoming edges
         *   - All adjacent edges are from a single vertex
         *
         *

         @dot
         digraph G {
         graph [rankdir=LR];
         subgraph cluster0 {
         node [shape=point,height=0.2,style=filled,color=black];
         style=filled;
         color=lightgrey;
         a0; a1; a2;
         label = "rest of graph";
         }
         v [color=green];
         a0 -> v;
         a0 -> v;
         v -> a0;
         v -> a0;
         }
         @enddot

         */

        auto adjacent_vertices = graph.find_adjacent_vertices(v);
        if (adjacent_vertices.size() == 1) {
            return true;
        }
    }
    debug << "Is Not Dead End\n";
    return false;
}

template < class G >
void
Pgr_deadend<G>::add_if_dead_end(G &graph, V v) {
    if (is_dead_end(graph, v)) {
        deadendVertices += v;
    }
}

template < class G >
void
Pgr_deadend<G>::doContraction(G &graph) {
#ifndef NDEBUG
    debug << "Performing contraction\n";
#endif
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
#ifndef NDEBUG
            debug << "Contracting current vertex "
                << graph[current_vertex].id << std::endl;
#endif
            graph[adjacent_vertex].add_contracted_vertex(
                    graph[current_vertex], current_vertex);

#ifndef NDEBUG
            debug << "Adding contracted vertices of the edge\n";
#endif
            auto o_edges = out_edges(current_vertex, graph.graph);
            for (auto out = o_edges.first;
                    out != o_edges.second;
                    ++out) {
                debug << graph.graph[*out];
                graph.add_contracted_edge_vertices(
                        adjacent_vertex, graph[*out]);
            }
            auto i_edges = in_edges(current_vertex, graph.graph);
            for (auto in = i_edges.first;
                    in != i_edges.second; ++in) {
#ifndef NDEBUG
                debug << graph.graph[*in];
#endif
                graph.add_contracted_edge_vertices(adjacent_vertex, graph[*in]);
            }
#ifndef NDEBUG
            debug << "Current Vertex:\n";
            debug << graph[current_vertex];
            debug << "Adjacent Vertex:\n";
            debug << graph[adjacent_vertex];
#endif
            graph.disconnect_vertex(current_vertex);
            deadendVertices -= current_vertex;
#ifndef NDEBUG
            debug << "Adjacent vertex dead_end?: "
                << is_dead_end(graph, adjacent_vertex)
                << std::endl;
#endif
            if (is_dead_end(graph, adjacent_vertex)
                    && !forbiddenVertices.has(adjacent_vertex)) {
                deadendVertices += adjacent_vertex;
                deadendPriority.push(adjacent_vertex);
            }
        }
    }
}

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_DEADENDCONTRACTION_HPP_
