/*PGR-GNU*****************************************************************
File: coreNumbers.cpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2026 Sakir Ahmed
Mail: sakirahmed75531 at gmail.com

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

#include "metrics/coreNumbers.hpp"

#include <vector>
#include <algorithm>
#include <cstddef>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

namespace {

using V = pgrouting::UndirectedGraph::V;

/**
 * Sort vertices by degree using bin-sort.
 *
 * bin[d] = starting position of vertices with degree d
 * vert[i] = vertex at position i in the sorted order
 * pos[v]  = position of vertex v in the vert array
 */
void bin_sort_vertices(
        const pgrouting::UndirectedGraph &graph,
        const std::vector<size_t> &degree,
        size_t max_degree,
        std::vector<V> &vert,
        std::vector<size_t> &pos,
        std::vector<size_t> &bin) {
    pgrouting::UndirectedGraph::V_i vi, vi_end;

    bin.assign(max_degree + 1, 0);
    for (boost::tie(vi, vi_end) = vertices(graph.graph); vi != vi_end; ++vi) {
        ++bin[degree[*vi]];
    }

    size_t start = 0;
    for (size_t d = 0; d <= max_degree; ++d) {
        size_t num = bin[d];
        bin[d] = start;
        start += num;
    }

    for (boost::tie(vi, vi_end) = vertices(graph.graph); vi != vi_end; ++vi) {
        pos[*vi] = bin[degree[*vi]];
        vert[pos[*vi]] = *vi;
        ++bin[degree[*vi]];
    }

    /* Restore bin after the sort pass */
    for (size_t d = max_degree; d > 0; --d) {
        bin[d] = bin[d - 1];
    }
    bin[0] = 0;
}

std::vector<II_t_rt> compute_core_numbers(
        const pgrouting::UndirectedGraph &graph) {
    size_t n = boost::num_vertices(graph.graph);
    if (n == 0) return {};

    /** degree - current degree of each graph vertex */
    std::vector<size_t> degree(n);

    pgrouting::UndirectedGraph::V_i vi, vi_end;
    /**
     * Get the degree of all vertices in the graph.
     */
    for (boost::tie(vi, vi_end) = vertices(graph.graph); vi != vi_end; ++vi) {
        degree[*vi] = boost::out_degree(*vi, graph.graph);
    }

    size_t max_degree = *std::max_element(degree.begin(), degree.end());

    std::vector<V> vert(n);
    std::vector<size_t> pos(n);
    std::vector<size_t> bin;
    bin_sort_vertices(graph, degree, max_degree, vert, pos, bin);

    /**
     * Process vertices in order of increasing degree.
     * For each vertex v, its current degree[v] becomes its core number.
     * Then for each neighbor u with degree[u] > degree[v], decrement degree[u]
     * and re-sort by swapping u with the first vertex of its bin.
     */
    for (size_t i = 0; i < n; ++i) {
        V v = vert[i];
        pgrouting::UndirectedGraph::EO_i ei, ei_end;
        for (boost::tie(ei, ei_end) = out_edges(v, graph.graph);
                ei != ei_end; ++ei) {
            V u = target(*ei, graph.graph);
            if (degree[u] > degree[v]) {
                size_t du = degree[u];
                size_t pu = pos[u];
                size_t pw = bin[du];
                V w = vert[pw];
                if (u != w) {
                    pos[u] = pw;
                    pos[w] = pu;
                    vert[pu] = w;
                    vert[pw] = u;
                }
                ++bin[du];
                --degree[u];
            }
        }
    }

    /* Build results: d1 = node id, d2 = core number */
    std::vector<II_t_rt> results;
    for (boost::tie(vi, vi_end) = vertices(graph.graph); vi != vi_end; ++vi) {
        int64_t node = graph[*vi].id;
        int64_t core = static_cast<int64_t>(degree[*vi]);
        results.push_back({node, core});
    }

    std::sort(results.begin(), results.end(),
            [](const II_t_rt &row1, const II_t_rt &row2) {
            return row1.d1 < row2.d1;
            });

    return results;
}

}  //  namespace

namespace pgrouting {
namespace metrics {

std::vector<II_t_rt>
coreNumbers(const pgrouting::UndirectedGraph &graph) {
    CHECK_FOR_INTERRUPTS();
    return compute_core_numbers(graph);
}

}  // namespace metrics
}  // namespace pgrouting
