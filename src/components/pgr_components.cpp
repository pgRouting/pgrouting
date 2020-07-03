/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

#include "components/pgr_components.hpp"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/biconnected_components.hpp>

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "cpp_common/identifiers.hpp"
#include "cpp_common/interruption.h"

namespace pgrouting {
namespace algorithms {

std::vector<pgr_components_rt>
pgr_connectedComponents(pgrouting::UndirectedGraph &graph) {
    // perform the algorithm
    std::vector< int > components(num_vertices(graph.graph));
    size_t num_comps;
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    try {
        num_comps = boost::connected_components(graph.graph, &components[0]);
    } catch (...) {
        throw;
    }

    // get the results
    std::vector< std::vector< int64_t > > results(num_comps);
    for (auto vd : boost::make_iterator_range(vertices(graph.graph))) {
        results[components[vd]].push_back(graph[vd].id);
    }

    return detail::componentsResult(results);
}

//! Strongly Connected Components Vertex Version
std::vector<pgr_components_rt>
strongComponents(
        pgrouting::DirectedGraph &graph) {
    // perform the algorithm
    std::vector< int > components(num_vertices(graph.graph));
    size_t num_comps;
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    try {
        num_comps = boost::strong_components(
                graph.graph,
                boost::make_iterator_property_map(components.begin(),
                    get(boost::vertex_index, graph.graph)));
    } catch (...) {
        throw;
    }

    // get the results
    std::vector< std::vector< int64_t > > results(num_comps);
    for (auto vd : boost::make_iterator_range(vertices(graph.graph))) {
        results[components[vd]].push_back(graph[vd].id);
    }

    return detail::componentsResult(results);
}



//! Biconnected Components
std::vector<pgr_components_rt>
biconnectedComponents(
        pgrouting::UndirectedGraph &graph) {
    using G = pgrouting::UndirectedGraph;
    using E = G::E;
    using Edge_map = std::map< E, size_t >;

    // perform the algorithm
    Edge_map bicmp_map;
    boost::associative_property_map<Edge_map> bimap(bicmp_map);
    size_t num_comps;
    try {
        num_comps = biconnected_components(graph.graph, bimap);
    } catch (...) {
        throw;
    }

    std::vector< std::vector< int64_t > > results(num_comps);
    for (auto ed : boost::make_iterator_range(edges(graph.graph))) {
        results[bimap[ed]].push_back(graph[ed].id);
    }

    return detail::componentsResult(results);
}

Identifiers<int64_t>
articulationPoints(
        pgrouting::UndirectedGraph &graph) {
    using G = pgrouting::UndirectedGraph;
    using V =  G::V;
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();

    // perform the algorithm
    std::vector<V> art_points;
    try {
        boost::articulation_points(graph.graph, std::back_inserter(art_points));
    } catch (...) {
        throw;
    }

    // get the results
    Identifiers<int64_t> results;
    for (const auto v : art_points) {
        results += graph[v].id;
    }

    return results;
}

/** Bridges
 * Bridges are closely related to the concept of articulation vertices,
 * vertices that belong to every path between some pair of other vertices.
 *
 * The two endpoints of a bridge are articulation vertices unless
 * they have a degree of 1, although it may also be possible for a non-bridge
 * edge to have two articulation vertices as endpoints.
 *
 * Analogously to bridgeless graphs being 2-edge-connected,
 * graphs without articulation vertices are 2-vertex-connected.
 */
Identifiers<int64_t>
bridges(pgrouting::UndirectedGraph &graph) {
    using G = pgrouting::UndirectedGraph;
    using V = G::V;
    using EO_i = G::EO_i;

    Identifiers<int64_t> bridge_edges;
    Identifiers<int64_t> processed_edges;
    std::vector<V> components(num_vertices(graph.graph));
    size_t ini_comps;
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    try {
        ini_comps = boost::connected_components(graph.graph, &components[0]);
    } catch (...) {
        throw;
    }
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    std::vector<V> art_points;
    try {
        boost::articulation_points(graph.graph, std::back_inserter(art_points));
    } catch (...) {
        throw;
    }

    for (auto v : boost::make_iterator_range(vertices(graph.graph))) {
        if (graph.out_degree(v) == 1) {
            art_points.push_back(v);
        }
    }

    for (const auto u : art_points) {
        for (const auto v : art_points) {
            /*
             * skip when the vertices are the same and do half the work
             */
            if (u < v) continue;
            auto p = boost::edge(u, v, graph.graph);

            /*
             * skip when there is no edge (u, v) on the graph
             */
            if (!p.second) continue;
            auto edge = p.first;
            auto id = graph[edge].id;

            /*
             * Skip when the edge has already being processed
             */
            if (processed_edges.has(id)) continue;

            /*
             * Processing edge
             */
            processed_edges += id;


            /*
             * At least one edge between articulation points u & v
             */
            int parallel_count = 0;
            EO_i ei, ei_end;
            boost::tie(ei, ei_end) = out_edges(u, graph.graph);

            for ( ; ei != ei_end; ++ei) {
                if (target(*ei, graph.graph) == v) ++parallel_count;
            }

            if (parallel_count == 1) {
                // TODO(vicky) filter graph instead of removing edges
                size_t now_comps;
                try {
                    boost::remove_edge(edge, graph.graph);

                    now_comps = boost::connected_components(graph.graph, &components[0]);

                    boost::add_edge(boost::source(edge, graph.graph),
                            boost::target(edge, graph.graph),
                            graph.graph);
                } catch (...) {
                    throw;
                }

                if (now_comps > ini_comps) {
                    bridge_edges += id;
                }
            }
        }
    }

    return bridge_edges;
}

}  // namespace algorithms
}  // namespace pgrouting
