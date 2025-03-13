/*PGR-GNU*****************************************************************
File: deadEndContraction.hpp

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

#ifndef INCLUDE_CONTRACTION_DEADENDCONTRACTION_HPP_
#define INCLUDE_CONTRACTION_DEADENDCONTRACTION_HPP_
#pragma once

#include <queue>
#include <functional>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include "cpp_common/identifiers.hpp"
#include "cpp_common/interruption.hpp"

namespace pgrouting {
namespace contraction {

template < class G >
class Pgr_deadend {
 private:
     using V = typename G::V;
     using E = typename G::E;

 public:
    Pgr_deadend() = default;

    void calculateVertices(G &graph) {
        for (const auto v : boost::make_iterator_range(vertices(graph.graph))) {
            if (graph.is_dead_end(v) && !graph.is_forbidden(v)) {
                deadendVertices += v;
            }
        }
    }

    void doContraction(G &graph) {
        calculateVertices(graph);

        while (!deadendVertices.empty()) {
            V v = deadendVertices.front();
            deadendVertices -= v;
            pgassert(graph.is_dead_end(v));

            Identifiers<V> local;
            for (auto u : graph.find_adjacent_vertices(v)) {
                /*
                 * u{v1} --{v2}-> v{v3}
                 *
                 * u{v1 + v + v2 + v3}     v{}
                 */
                const auto& e = graph.get_min_cost_edge(u, v);
                graph[u].contracted_vertices() +=
                    std::get<0>(e).contracted_vertices();
                graph[u].add_contracted_vertex(graph[v]);

                deadendVertices -= v;
                local += u;
            }

            graph[v].contracted_vertices().clear();
            boost::clear_vertex(v, graph.graph);

            /* abort in case of an interruption occurs
            (e.g. the query is being cancelled) */
            CHECK_FOR_INTERRUPTS();

            for (const auto u : local) {
                if (graph.is_dead_end(u)
                    && !graph.is_forbidden(u)) {
                    deadendVertices += u;
                } else {
                    deadendVertices -= u;
                }
            }
        }
    }

 private:
    Identifiers<V> deadendVertices;
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_DEADENDCONTRACTION_HPP_
