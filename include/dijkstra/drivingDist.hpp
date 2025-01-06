/*PGR-GNU*****************************************************************
File: drivingDist.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakri at yahoo.com, estebanzimanyi at gmail.com

Copyright (c) 2023 Aryan Gupta
guptaaryan1010 AT gmail.com

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

#ifndef INCLUDE_DIJKSTRA_DRIVINGDIST_HPP_
#define INCLUDE_DIJKSTRA_DRIVINGDIST_HPP_
#pragma once

#include <deque>
#include <set>
#include <vector>
#include <functional>
#include <limits>
#include <map>
#include <numeric>
#include <cstdint>


#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "visitors/dijkstra_visitors.hpp"


namespace bg_detail {

/** @brief Dijkstra 1 to distance
 *
 * Used on:
 * - 1 to distance
 * - many to distance
 * - On the first call of many to distance with equi_cost
 *
 * @param [in] bg  boost graph
 * @param [in] root  vertex root of the spanning tree
 * @param [in] distance  maximum distance.
 * @param [in] predecessors  predecessors list
 * @param [in] distances  distances from root
 */
template <typename B_G, typename V, typename T_E>
void dijkstra_1_to_distance(
        const B_G &bg,
        V root,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        double distance) {
    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(bg, root,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&T_E::cost, bg))
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::dijkstra_distance_visitor<V>(distance, distances)));
    } catch(pgrouting::found_goals &) {
        /*No op*/
    } catch (boost::exception const&) {
        throw;
    } catch (std::exception&) {
        throw;
    } catch (...) {
        throw;
    }
}

/** @brief Dijkstra 1 to distance no initialization
 *
 * Used on:
 *   On the subsequent calls of many to distance with equi_cost
 *
 * @param [in] bg  boost graph
 * @param [in] root  vertex root of the spanning tree
 * @param [in] distance  maximum distance.
 * @param [in] predecessors  predecessors list
 * @param [in] distances  distances from root
 */
template <typename B_G, typename V, typename E, typename G_T_E>
void dijkstra_1_to_distance_no_init(
        const B_G &bg,
        V root,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        double distance) {
    pgassert(predecessors.size() == num_vertices(bg));
    pgassert(distances.size() == num_vertices(bg));

    distances[root] = 0;
    /*
     * Not using the default, because vertices visited from other roots are marked color black
     */
    std::vector<boost::default_color_type> color_map(num_vertices(bg));
    auto vidx(boost::get(boost::vertex_index, bg));

    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths_no_init(bg, root,
                make_iterator_property_map(predecessors.begin(), vidx),
                make_iterator_property_map(distances.begin(), vidx),
                get(&G_T_E::cost, bg),
                vidx,
                std::less<double>(),
                boost::closed_plus<double>(),
                static_cast<double>(0),
                pgrouting::visitors::dijkstra_distance_visitor_no_init<V, E>(root, distance, predecessors, distances,
                    color_map),
                make_iterator_property_map(color_map.begin(), vidx, color_map[0]));
    } catch(pgrouting::found_goals &) {
        /* no op */
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
        throw;
    }
}

}  // namespace bg_detail

namespace detail {

/* notation:
 * node: vertex or point
 * vertex: node with id >= 0
 * point: node with id < 0
 */
template <typename G, typename V>
void remove_details(const G &graph,
        const std::vector<double> &distances,
        std::vector<V> &predecessors) {
    /*
     * find all the points that are predecessors
     */
    std::set<V> node_with_predecessor_point;
    CHECK_FOR_INTERRUPTS();
    for (V v = 0; v < predecessors.size() ; ++v) {
        /*
         * skipping unreachable nodes and or initial node
         * skipping predecessors that are vertices
         */
        if (predecessors[v] == v) continue;
        if (graph[predecessors[v]].id >= 0) continue;
        node_with_predecessor_point.insert(v);
    }

    /*
     * Compact all nodes that have predecessor point
     */
    for (const auto node : node_with_predecessor_point) {
        /*
         * Cycle predecessors
         * u -> v  cost to arrive to v is distances[v]
         */
        auto v = node;
        auto u = predecessors[v];
        pgassert(graph[u].id < 0);

        /*
         * while u is a point and it's predecessor is not itself
         */
        CHECK_FOR_INTERRUPTS();
        while (graph[u].id < 0 && predecessors[u] != u) {
            pgassert(graph[u].id < 0);
            pgassert(distances[v] !=  std::numeric_limits<double>::infinity());
            v = u;
            u = predecessors[v];
        }

        /* the vertex (or initial point) that is a predecesor of p */
        predecessors[node] = u;
    }
}

/** @brief gets results in form of a container of paths with depth
 *
 * @param [in] graph The graph that is being worked
 * @param [in] root The starting node
 * @param [in] distances An array of vertices @b id
 * @param [in] predecessors an array of predecessors
 * @param [in] distance the max distance
 * @param [in] details the max distance
 */
template <typename G, typename V>
std::map<int64_t, int64_t> get_depth(
        const G &graph,
        V root,
        const std::vector<double>& distances,
        std::vector<V>& predecessors,
        double distance,
        bool details) {
    std::map<int64_t, int64_t> depth;
    if (predecessors.empty()) return depth;
    if (predecessors.size() != distances.size()) return depth;
    depth[graph[root].id] = 0;

    std::set<V> vertices;
    vertices.insert(root);

    if (!details) {
        remove_details(graph, distances, predecessors);
    }

    /* cycle depth max depth can be number of nodes*/
    for (int64_t d = 1; static_cast<size_t>(d) < graph.num_vertices() ; ++d) {
        /*
         * there is no more to search for
         */
        if (vertices.empty()) break;

        /*
         * One next cycle these vertices have the next depth
         */
        std::set<V> vertices_next;
        std::set<V> point_vertices;


        for (const auto v : vertices) {
            /*
             * Cycle predecessors looking for vertices on the depth d
             * v -> p
             */
            for (V p = 0; p < graph.num_vertices() ; ++p) {
                /*
                 * Sikiping unassigned predecessors
                 * Sikiping distances greater than the one asked for
                 */
                if (predecessors[p] == p) continue;
                if (!(distances[p] <= distance)) continue;
                if (!(predecessors[p] == v)) continue;


                /* found */
                depth[graph[p].id] = d;
                vertices_next.insert(p);
            }
        }
        vertices = vertices_next;
    }
    return depth;
}

/** @brief gets results for many vertices and equi costs
 *
 * @param [in] graph The graph that is being worked
 * @param [in] roots a set of roots
 * @param [in] predecessors an array of predecessors
 * @param [in] noDetailsPredecessors veritces that do not have a predecessor
 * @param [in] distances a map of distances
 * @param [in] distance maximum distance
 * @param [in] details flag to indicate to include points
 *
 * @pre one predecessor per root
 */
template <typename G, typename V>
std::deque<pgrouting::Path> get_drivingDistance_with_equicost_paths(
        const G &graph,
        const std::set<int64_t> &roots,
        std::deque<std::vector<V>> &predecessors,
        std::vector<double> &distances,
        std::deque<std::vector<V>> &noDetailsPredecessors,
        double distance, bool details) {
    using Path = pgrouting::Path;

    pgassert(roots.size() == predecessors.size());

    /*
     * Creating all the result "paths"
     */
    std::deque<Path> paths;
    for (const auto r : roots) {
        paths.push_back(Path(r, r));
        paths.back().push_back({r, -1, 0, 0, r});
    }

    /*
     *  Ciclying the vertices:
     *  To which vertex do they belong to?
     */
    for (V v = 0; v < distances.size(); ++v) {
        /*
         * Sikiping distances greater than the one asked for
         */
        if (!(distances[v] <= distance)) continue;

        for (auto i = roots.size(); i > 0; --i) {
            const auto &predecessor = predecessors[i - 1];
            /*
             * The vertex does not exist on the graph
             * The predecessor = current then its unreachable to this vertex
             */
            if (predecessor.empty()) break;
            if (predecessor[v] == v) continue;

            auto u =  details? predecessor[v] : noDetailsPredecessors[i - 1][v];
            /*
             * precalculating cost to find the edge
             */
            auto cost = distances[v] - distances[predecessor[v]];
            auto edge_id = graph.get_edge_id(predecessor[v], v, cost);
            pgassert(edge_id != -1);
            /*
             * real cost is with real predecessor
             */
            cost = details? cost : distances[v] - distances[u];
                 paths[i - 1].push_back({graph[v].id, edge_id, cost, distances[v], graph[u].id});
                 break;
             }
         }

    for (auto &path : paths) {
        path.sort_by_node_agg_cost();
    }
    return paths;
}

/* preparation for many to distance with equicost
 *
 *   The distances vector does not change
 *   The predecessors vector does not change
 *   The first @b valid execution is done normally:
 *     - The distances will have:
 *       - inf
 *       - values < distance
 *       - values > distance
 *   Subsequent @b valid executions
 *       - will not change the:
 *         - values < distance
 *   Don't know yet what happens to predecessors
 */
template <typename G>
std::deque<pgrouting::Path> drivingDistance_with_equicost(
        G &graph,
        const std::set<int64_t> &roots,
        std::vector<std::map<int64_t, int64_t>> &depths,
        double distance, bool details) {
    using V = typename G::V;
    using E = typename G::E;
    using T_E = typename G::G_T_E;
    using B_G = typename G::B_G;

    depths.resize(roots.size());
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    std::deque<std::vector<V>> pred(roots.size());
    std::deque<std::vector<V>> noDetailsPredecessors(roots.size());

    size_t i = 0;
    for (const auto &root : roots) {
        /*
         * The vertex does not exist Nothing to do
         */
        if (!(graph.has_vertex(root))) continue;

        std::iota(predecessors.begin(), predecessors.end(), 0);
        bg_detail::dijkstra_1_to_distance_no_init<B_G, V, E, T_E>(graph.graph, graph.get_V(root), predecessors,
                distances, distance);

        pred[i] = predecessors;
        depths[i] = detail::get_depth(graph, graph.get_V(root), distances, predecessors, distance, details);
        if (!details) {
            noDetailsPredecessors[i] = predecessors;
        }
        ++i;
    }

    /*
     * predecessors of root vertices are themselves
     */
    for (const auto &root : roots) {
        for (auto &p : pred) {
            if (!p.empty() && graph.has_vertex(root))
                p[graph.get_V(root)] = graph.get_V(root);
        }
    }

    return get_drivingDistance_with_equicost_paths(
            graph,
            roots,
            pred,
            distances,
            noDetailsPredecessors,
            distance, details);
}

/** @brief gets results for many vertices and equi costs
 *
 * @param [in] graph The graph that is being worked
 * @param [in] roots a set of roots
 * @param [in] depths a vector of map of depths
 * @param [in] distance maximum distance
 * @param [in] details flag to indicate to include points
 */
template <typename G>
std::deque<pgrouting::Path> drivingDistance_no_equicost(
        const G &graph,
        const std::set<int64_t> &roots,
        std::vector<std::map<int64_t, int64_t>> &depths,
        double distance, bool details) {
    using Path = pgrouting::Path;
    using B_G = typename G::B_G;
    using V = typename G::V;
    using T_E = typename G::G_T_E;

    std::deque<Path> paths;

    for (const auto &root : roots) {
        if (graph.has_vertex(root)) {
            std::vector<V> predecessors(graph.num_vertices());
            std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

            bg_detail::dijkstra_1_to_distance<B_G, V, T_E>(
                    graph.graph, graph.get_V(root), predecessors, distances, distance);

            auto path = Path(graph, root, distance, predecessors, distances);
            path.sort_by_node_agg_cost();
            depths.push_back(detail::get_depth(graph, graph.get_V(root), distances, predecessors, distance, details));
            /*
             * When details are not wanted update costs
             */
            if (!details) {
                for (auto &pathstop : path) {
                    auto node = graph.get_V(pathstop.node);

                    /* skip points */
                    if (graph[node].id < 0) continue;

                    pathstop.cost = distances[node] - distances[predecessors[node]];
                }
            }
            paths.push_back(path);

        } else {
            Path p(root, root);
            p.push_back({root, -1, 0, 0, root});
            paths.push_back(p);

            std::map<int64_t, int64_t> m;
            m[root] = 0;
            depths.push_back(m);
        }
    }
    return paths;
}


}  // namespace detail


namespace pgrouting {
namespace algorithm {

template <typename G>
std::deque<Path> drivingDistance(
        const G &graph,
        const std::set<int64_t> &roots,
        double distance,
        bool equicost,
        std::vector<std::map<int64_t, int64_t>> &depths,
        bool details) {
    if (equicost) {
        return detail::drivingDistance_with_equicost(
                graph,
                roots,
                depths,
                distance, details);
    } else {
        return detail::drivingDistance_no_equicost(
                graph,
                roots,
                depths,
                distance, details);
    }
}

}  // namespace algorithm
}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_DRIVINGDIST_HPP_
