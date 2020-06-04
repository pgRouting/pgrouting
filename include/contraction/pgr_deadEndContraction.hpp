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
#include "cpp_common/interruption.h"

namespace pgrouting {
namespace contraction {

template < class G >
class Pgr_deadend {
 private:
     using V = typename G::V;
     using E = typename G::E;

 public:
    Pgr_deadend() = default;

    void setForbiddenVertices(
            Identifiers<V> forbidden_vertices) {
        forbiddenVertices = forbidden_vertices;
    }


    void calculateVertices(G &graph) {
        for (const auto v : boost::make_iterator_range(vertices(graph.graph))) {
            if (is_dead_end(graph, v) && !forbiddenVertices.has(v)) {
                deadendVertices += v;
            }
        }
    }

    bool is_dead_end(G &graph, V v) {
        if (graph.is_undirected()) {
            return graph.find_adjacent_vertices(v).size() == 1;
        }

        pgassert(graph.is_directed());

        return graph.find_adjacent_vertices(v).size() == 1
            || (graph.in_degree(v) > 0 && graph.out_degree(v) == 0);
    }

    void doContraction(G &graph) {
        calculateVertices(graph);

        while (!deadendVertices.empty()) {
            V v = deadendVertices.front();
            deadendVertices -= v;
            pgassert(is_dead_end(graph, v));

            Identifiers<V> local;
            for (auto u : graph.find_adjacent_vertices(v)) {
                /*
                 * u{v1} --{v2}-> v{v3}
                 *
                 * u{v1 + v + v2 + v3}     v{}
                 */
                auto v2(graph.get_min_cost_edge(u, v));
                graph[u].contracted_vertices() += std::get<1>(v2);
                graph[u].contracted_vertices() += graph[v].id;
                graph[u].contracted_vertices() += graph[v].contracted_vertices();

                deadendVertices -= v;
                local += u;
            }

            graph[v].contracted_vertices().clear();
            boost::clear_vertex(v, graph.graph);

            /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
            CHECK_FOR_INTERRUPTS();

            for (const auto u : local) {
                if (is_dead_end(graph, u) && !forbiddenVertices.has(u)) {
                    deadendVertices += u;
                } else {
                    deadendVertices -= u;
                }
            }
        }
    }

 private:
    Identifiers<V> deadendVertices;
    Identifiers<V> forbiddenVertices;
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_PGR_DEADENDCONTRACTION_HPP_
