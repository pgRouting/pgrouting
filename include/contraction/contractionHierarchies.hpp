/*PGR-GNU*****************************************************************
File: contractionhierarchies.hpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) Aurélie Bousquet - 2024
Mail: aurelie.bousquet at oslandia.com

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

#ifndef INCLUDE_CONTRACTION_CONTRACTIONHIERARCHIES_HPP_
#define INCLUDE_CONTRACTION_CONTRACTIONHIERARCHIES_HPP_
#pragma once

#include <queue>
#include <functional>
#include <vector>
#include <limits>
#include <set>
#include <string>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "cpp_common/alloc.hpp"
#include "cpp_common/ch_edge.hpp"
#include "cpp_common/messages.hpp"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/interruption.hpp"

#include "c_common/e_report.h"

#include "visitors/dijkstra_visitors.hpp"



namespace pgrouting {
namespace detail {


/*!
    @brief builds shortcuts inside the graph to contract it
    @param [in] graph the graph being contracted
    @param [in] u vertex_descriptor
    @param [in] v vertex_descriptor
    @param [in] out_vertices out vertices from v
    @param [out] shortcuts list of built shortcuts
    @param [in] log log output
    @param [in] err err output
*/
template < class G >
void compute_shortcuts(
    G &graph,
    typename G::V u,
    typename G::V v,
    Identifiers<typename G::V> out_vertices,
    std::vector<typename G::E> &shortcuts,
    std::ostringstream &log,
    std::ostringstream &err
) {
    std::vector<typename G::V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices());
    typename G::V_i out_i, out_end;

    int64_t p_max = graph.compute_pmax(u, v, out_vertices);
    log << "    p_max = " << p_max << std::endl;

    if ( p_max > 0 ) {
        // Launch of a shortest paths query from u to all nodes
        // with distance less than p_max
        std::set<int64_t> reached_vertices_ids;
        try {
            boost::dijkstra_shortest_paths(
            graph.graph,
            u,
            boost::predecessor_map(&predecessors[0])
            .weight_map(get(&G::G_T_E::cost, graph.graph))
            .distance_map(&distances[0])
            .distance_inf(std::numeric_limits<double>::infinity())
            .visitor(pgrouting::visitors::dijkstra_max_distance_visitor<typename G::V>(
                p_max, distances, reached_vertices_ids)));
        }
        catch ( pgrouting::found_goals & ) {
            log << "    PgRouting exception during labelling!"
                << std::endl;
            log << "    >>> Labelling interrupted"
                << " because max distance is reached. " << std::endl;
            log << "    >>> Number of labelled vertices: "
                << reached_vertices_ids.size() << std::endl;
            Identifiers<int64_t> r;
            r.set_ids(reached_vertices_ids);
            log << "    >>> Reached vertices: " << r << std::endl;
        }
        catch ( boost::exception const &except ) {
            err << std::endl << "Boost exception "
                << "during vertex contraction! "
                << dynamic_cast<std::exception const &>(except).what()
                << std::endl;
        }
        catch ( ... ) {
            err << "    Unknown exception during labelling!" << std::endl;
        }
        /* abort in case of an interruption occurs
        (e.g. the query is being cancelled) */
        CHECK_FOR_INTERRUPTS();

        // Create a shortcut, for each w,
        // when c(u, v) + c(u, w) = cost(shortest path from u to v)
        for (const auto &w : out_vertices) {
            if ( u != w ) {
                typename G::E e, f, g;
                bool found_e, found_f, found_g;
                boost::tie(e, found_e) = boost::edge(u, v, graph.graph);
                boost::tie(f, found_f) = boost::edge(v, w, graph.graph);
                boost::tie(g, found_g) = boost::edge(u, w, graph.graph);
                if (found_e && found_f &&
                (!found_g || (distances[w] < graph[g].cost))) {
                    if ((predecessors[w] == v) && (predecessors[v] == u) &&
                        (distances[w] == graph[e].cost + graph[f].cost)) {
                        graph.process_ch_shortcut(
                            u, v, w,
                            shortcuts, log);
                    }
                }
            }
        }
    }
}

/** @brief contracts vertex *v*

    @param[in] graph
    @param[in] directed set to @b true when the graph is directed
    @param[in] v vertex_descriptor
    @param[in] simulation set to true when graph is not modified
    @param[in,out] shortcuts edge shurtcuts
    @param[out] log log output
    @param[out] err err output
    @return contraction metric: the node-contraction associated metrics
*/
template <class G>
int64_t vertex_contraction(
    G &graph,
    bool directed,
    typename G::V v,
    bool simulation,
    std::vector<CH_edge> &shortcuts,
    std::ostringstream &log,
    std::ostringstream &err
) {
    Identifiers<typename G::V> adjacent_in_vertices;
    Identifiers<typename G::V> adjacent_out_vertices;
    int64_t n_old_edges;
    std::vector<typename G::E> shortcut_edges;

    if (directed) {
        adjacent_in_vertices = graph.find_adjacent_in_vertices(v);
        adjacent_out_vertices = graph.find_adjacent_out_vertices(v);
        n_old_edges =
            static_cast<int64_t>(adjacent_in_vertices.size()
            + adjacent_out_vertices.size());
    } else {
        adjacent_in_vertices = graph.find_adjacent_vertices(v);
        adjacent_out_vertices = adjacent_in_vertices;
        n_old_edges = static_cast<int64_t>(adjacent_in_vertices.size());
    }

    log << ">> Contraction of node " << graph[v].id << std::endl
        << num_vertices(graph.graph) << " vertices and "
        << num_edges(graph.graph) << " edges " << std::endl;

    for (auto &u : adjacent_in_vertices) {
        log << "  >> from " << graph[u].id << std::endl;
        compute_shortcuts(
            graph,
            u,
            v,
            adjacent_out_vertices,
            shortcut_edges,
            log,
            err);
    }
    if (!simulation) {
        for (auto &w : adjacent_out_vertices) {
            boost::remove_edge(v, w, graph.graph);
        }
        for (auto &u : graph.find_adjacent_in_vertices(v)) {
            boost::remove_edge(u, v, graph.graph);
        }
        graph[v].clear_contracted_vertices();
        for (auto &e : shortcut_edges) {
            shortcuts.push_back(graph.graph[e]);
        }
    } else {
        for (auto &e : shortcut_edges) {
            boost::remove_edge(e, graph.graph);
        }
    }

    log << "  Size of the graph after contraction: "
        << num_vertices(graph.graph)
        << " vertices and " << num_edges(graph.graph)
        << " edges" << std::endl
        << "  " << shortcut_edges.size()
        << " shortcuts created, " << n_old_edges
        << " old edges" << std::endl;

    int64_t m;
    m = static_cast<int64_t>(shortcut_edges.size())
      - static_cast<int64_t>(n_old_edges);
    log << "  Metric: edge difference = " << shortcut_edges.size()
        << " - " << n_old_edges << " = " << m << std::endl;
    return m;
}

template < class G >
void contractionHierarchies(
    G &graph,
    bool directed,
    std::ostringstream &log,
    std::ostringstream &err
) {
    // First iteration over vertices
    // graph_copy must stay unchanged during this first step,
    // graph will be modified
    G graph_copy = graph;
    std::vector<CH_edge> shortcuts;

    // Fill the priority queue with vertices ordered by their IDs
    log << "Do contraction" << std::endl;
    typename G::PQ minPQ, priority_queue;
    for (const auto &v:
        boost::make_iterator_range(boost::vertices(graph.graph))) {
        if (!graph.is_forbidden(v)) {
            minPQ.push(std::make_pair(graph[v].id, v));
        }
    }

    while (!minPQ.empty()) {
        std::pair< int64_t, typename G::V > ordered_vertex = minPQ.top();
        minPQ.pop();
        int64_t corrected_metric =
            detail::vertex_contraction(
                graph_copy,
                directed,
                ordered_vertex.second,
                true,
                shortcuts,
                log,
                err);
        log << "  Vertex: " << graph[ordered_vertex.second].id
            << ", min value of the queue: "
            << minPQ.top().first << std::endl
            << "  Lazy non-destructive simulation: initial order "
            << ordered_vertex.first << ", new order "
            << corrected_metric << std::endl;

        if (minPQ.top().first < corrected_metric) {
            log << "   Vertex reinserted in the queue" << std::endl;
            minPQ.push(
                std::make_pair(corrected_metric, ordered_vertex.second));
        } else {
            std::pair< int64_t, typename G::V > contracted_vertex;
            typename G::V u =
                graph.vertices_map[graph[ordered_vertex.second].id];
            contracted_vertex.first = detail::vertex_contraction(
                graph_copy,
                directed,
                u,
                false,
                shortcuts,
                log,
                err);
            log << "  Vertex endly contracted in the queue" << std::endl;
            contracted_vertex.second = ordered_vertex.second;
            priority_queue.push(contracted_vertex);
        }
    }
    log << std::endl << "Copy shortcuts" << std::endl;
    graph.copy_shortcuts(shortcuts, log);
    log << std::endl << "Priority queue: " << std::endl;
    graph.set_vertices_metric_and_hierarchy(priority_queue, log);
}

}  // namespace detail

namespace functions {

/** @brief execute the contraction hierarchies, after having forbidden the needed vertices

    @param[in,out] graph in original graph out contracted graph
    @param[in]     directed when true then the graph is directed
    @param[in]     forbidden_vertices vector of forbidden vertices
    @param[in,out] log string stream containing log information
    @param[in,out] err string stream containing err information
 */
template <class G>
void contractionHierarchies(
        G& graph,
        bool directed,
        const std::vector<int64_t> &forbidden_vertices,
        std::ostringstream &log,
        std::ostringstream &err) {
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
        detail::contractionHierarchies(graph, directed, log, err);
    }
    catch ( ... ) {
        err << "Contractions hierarchy failed" << std::endl;
        throw;
    }
}

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_CONTRACTION_CONTRACTIONHIERARCHIES_HPP_
