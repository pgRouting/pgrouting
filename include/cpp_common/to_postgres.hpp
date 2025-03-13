/*PGR-GNU*****************************************************************
File: to_postgres.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

#ifndef INCLUDE_CPP_COMMON_TO_POSTGRES_HPP_
#define INCLUDE_CPP_COMMON_TO_POSTGRES_HPP_
#pragma once

#include <vector>
#include <cstddef>

#include "c_types/contractionHierarchies_rt.h"
#include "c_types/iid_t_rt.h"

#include "cpp_common/base_graph.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/identifiers.hpp"

#include "contraction/contractionHierarchies.hpp"

namespace pgrouting {
namespace to_postgres {

/** @brief Stored results on a vector are saved on a C array
 *
 * @param[in] graph Created graph with the base Graph
 * @param[in] results results[i] -> the ith element of the vector contains the results
 * @param[out] result_count The size of the vector
 * @param[out] result_tuples The C array of <bigint, bigint, float>
 *
 * <bigint, bigint, float> =  < i , 0, results[i] >
 *
 * Currently works for pgr_betweennessCentrality
 */
template <class G>
void vector_to_tuple(
            const G &graph,
            const std::vector<double> results,
            size_t &result_count,
            IID_t_rt **result_tuples) {
    result_count = results.size();
    *result_tuples = pgrouting::pgr_alloc(result_count, (*result_tuples));

    size_t seq = 0;
    for (typename G::V v_i = 0; v_i < graph.num_vertices(); ++v_i) {
        (*result_tuples)[seq].from_vid = graph[v_i].id;
        /*
         * These 2 lines are specifically for pgr_betweennessCentrality
         */
        (*result_tuples)[seq].to_vid = 0;
        (*result_tuples)[seq].cost = graph.is_directed()? results[v_i] / 2.0 : results[v_i];
        seq++;
    }
}

}  // namespace to_postgres
}  // namespace pgrouting

namespace detail {

/*! @brief execute the contraction hierarchies, after having forbidden the needed vertices
    @param [in] graph created graph with base Graph
    @param [in] forbidden_vertices vector of forbidden vertices
    @param [in/out] log string stream containing log information
    @param [in/out] err string stream containing err information
 */
template <class G>
void perform_contractionHierarchies(
        G &graph,
        bool directed,
        const std::vector< Edge_t > &edges,
        const std::vector< int64_t > &forbidden_vertices,
        std::ostringstream &log,
        std::ostringstream &err) {
    // Create the graph
    graph.insert_edges(edges);

    // Transform the forbidden vertices IDs vector to a collection of vertices
    pgrouting::Identifiers<typename G::V> forbid_vertices;
    for (const auto &vertex : forbidden_vertices) {
        if (graph.has_vertex(vertex)) {
            forbid_vertices += graph.get_V(vertex);
        }
    }
    graph.set_forbidden_vertices(forbid_vertices);

    // Execute the contraction
    try {
        pgrouting::functions::contractionHierarchies(graph, directed, log, err);
    }
    catch ( ... ) {
        err << "Contractions hierarchy failed" << std::endl;
        throw;
    }
}

/*! @brief returns results to the SQL function
    @param [in] graph created graph
    @param [out] return_tuples tuples containing the results to pass to the SQL function
    @param [out] count string stream containing log information
*/
template <class G>
void get_postgres_result_contractionHierarchies(
        G &graph,
        contractionHierarchies_rt **return_tuples,
        size_t *count) {
    using pgrouting::pgr_alloc;
    auto modified_vertices(graph.get_modified_vertices());
    auto shortcut_edges(graph.get_shortcuts());

    (*count) = modified_vertices.size() + shortcut_edges.size();
    (*return_tuples) = pgr_alloc((*count), (*return_tuples));
    size_t sequence = 0;

    for (const auto id : modified_vertices) {
        auto v = graph.get_V(id);
        int64_t* contracted_vertices = NULL;
        auto vids = graph[v].contracted_vertices();
        int64_t o = graph.get_vertex_order(id);
        int64_t m = graph.get_vertex_metric(id);

        contracted_vertices = pgr_alloc(vids.size(), contracted_vertices);

        int count = 0;
        for (const auto id : vids) {
            contracted_vertices[count++] = id;
        }
        (*return_tuples)[sequence] = {
            id,
            const_cast<char*>("v"),
            -1, -1, -1.00,
            contracted_vertices,
            count,
            o, m};

        ++sequence;
    }

    int64_t eid = 0;
    for (auto e : shortcut_edges) {
        auto edge = graph[e];
        int64_t* contracted_vertices = NULL;
        const auto vids(edge.contracted_vertices());
        pgassert(!vids.empty());

        contracted_vertices = pgr_alloc(vids.size(), contracted_vertices);
        int count = 0;
        for (const auto vid : vids) {
            contracted_vertices[count++] = vid;
        }
        (*return_tuples)[sequence] = {
            --eid,
            const_cast<char*>("e"),
            edge.source, edge.target, edge.cost,
            contracted_vertices, count, -1, -1};
        ++sequence;
    }
}

}  // namespace detail

#endif  // INCLUDE_CPP_COMMON_TO_POSTGRES_HPP_
