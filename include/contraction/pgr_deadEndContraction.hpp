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


#include <boost/graph/iteration_macros.hpp>
#include <queue>
#include <functional>
#include <vector>
#include "cpp_common/identifiers.hpp"

// TODO remove the debug messages

namespace pgrouting {
namespace contraction {

template < class G >
class Pgr_deadend {
 private:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::B_G B_G;

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
};

/******* IMPLEMENTATION ************/
template < class G >
void
Pgr_deadend< G >::setForbiddenVertices(
        Identifiers<V> forbidden_vertices) {
    forbiddenVertices = forbidden_vertices;
}


template < class G >
void Pgr_deadend<G>::calculateVertices(G &graph) {
    BGL_FORALL_VERTICES_T(v, graph.graph, B_G) {
        if (is_dead_end(graph, v)) {
            deadendVertices += (v);
        }
    }
    deadendVertices -= forbiddenVertices;
}

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

directed graph

is dead end when:
(1) no incomming edge, one outgoing edge (dead start)
(2) one incoming edge, no outgoing edge (dead end)
(3) one outgoing edge, one incoming edge
and both are from/to the same vertex
(4) many incoming edges
and no outgoing edges
(5) many outgoing edges TODO but all go to same vertex
and no incoming edges

NOT dead end when:
(3) one outgoing edge, one incoming edge
and both from/to different vertex

note: when contracting case 4 & 5, the vertex has to be
part of all the adjacent vertices

directed
----------
case (1):  (dead start)
- one outgoing edge
- no incoming edge

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

case (2):  (dead end)
- no outgoing edge,
- one incoming edge


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

case (3):
    - one outgoing edge,
    - one incoming edge
    - one adjacent vertex


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

case (4):
- no outgoing edge
- many incoming edges

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

case (5):
- many outgoing edge
- many incoming edges
- All adjacent edges are from a single vertex

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

template < class G >
bool Pgr_deadend<G>::is_dead_end(G &graph, V v) {
    if (graph.is_undirected()) {
        return graph.find_adjacent_vertices(v).size() == 1;
    }

    pgassert(graph.is_directed());
    return graph.find_adjacent_vertices(v).size() == 1
        || (graph.in_degree(v) > 0 && graph.out_degree(v) == 0);
}

template < class G >
void
Pgr_deadend<G>::add_if_dead_end(G &graph, V v) {
    if (forbiddenVertices.has(v))  return;
    if (is_dead_end(graph, v)) {
        deadendVertices += v;
    }
}

template < class G >
void
Pgr_deadend<G>::doContraction(G &graph) {

    while (!deadendVertices.empty()) {
        V current_vertex = deadendVertices.front();
        deadendVertices -= current_vertex;
        pgassert(is_dead_end(graph, current_vertex));

        for (auto adjacent_vertex : graph.find_adjacent_vertices(current_vertex)) {

            graph[adjacent_vertex].add_contracted_vertex(
                    graph[current_vertex]);

            graph.disconnect_vertex(current_vertex);
            deadendVertices -= current_vertex;

            if (is_dead_end(graph, adjacent_vertex)
                    && !forbiddenVertices.has(adjacent_vertex)) {
                deadendVertices += adjacent_vertex;
            } else {
                deadendVertices -= adjacent_vertex;
            }
        }
        graph[current_vertex].clear_contracted_vertices();
    }
}

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_DEADENDCONTRACTION_HPP_
