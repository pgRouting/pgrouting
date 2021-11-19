/*PGR-GNU*****************************************************************
File: dijkstra.hpp

Copyright (c) 2021 Celia Virginia Vergara Castillo
mail: vicky_vergara@hotmail.com


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

#ifndef INCLUDE_DIJKSTRA_DIJKSTRA_HPP_
#define INCLUDE_DIJKSTRA_DIJKSTRA_HPP_
#pragma once

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>
#include <map>
#include <numeric>

#include "dijkstra/bg_dijkstra.hpp"
#include "cpp_common/basePath_SSEC.hpp"


namespace pgrouting {
namespace detail {

/**
 * @brief gets the path sequence
 *
 * For a given graph and a source with all the corresponding targets gets the path sequence
 *
 * @tparam G graph instance from Pgr_base_graph
 * @tparam V graph The corresponding vertex descriptor
 * @param[in] graph Original graph
 * @param[in] source for this given source descriptor
 * @param[in] targets to all the set of target descriptors
 * @param[in] only_cost when @b true Only get the cost to reach the targets
 * @param[in] predecessors vector result from processing
 * @param[in] distance vector result from processings
 *
 * @returns the rows needed for postgres one Path per combination
 */
template <class G, typename V>
std::deque<Path>
get_paths(
        const G &graph,
        V source,
        const std::set<V> &targets,
        bool only_cost,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances) {
    std::deque<Path> paths;
    for (const auto target : targets) {
        paths.push_back(Path(
                    graph,
                    source, target,
                    predecessors, distances,
                    only_cost, true));
    }
    return paths;
}

/** @brief Preparing driving distance BGL call
 *
 * Prepares the execution for a driving distance
 *
 * @tparam G graph instance from Pgr_base_graph
 * @param[in] graph Original graph
 * @param[in] start_vertex the starting vertex
 * @param[in] distance The limit distance
 *
 * Results are kept on predecessor & distances
 *
 */
template <class G>
Path execute_drivingDistance(
        const G &graph,
        int64_t start_vertex,
        double distance) {
    using V = typename G::V;
    if (graph.has_vertex(start_vertex)) {
        std::vector<V> predecessors;
        std::vector<double> distances;
        std::deque<V> nodesInDistance;

        predecessors.resize(graph.num_vertices());
        distances.resize(
                graph.num_vertices(),
                std::numeric_limits<double>::infinity());

        if (pgrouting::bg_algorithms::dijkstra_1_to_distance(
                    graph.graph,
                    graph.get_V(start_vertex),
                    distance,
                    predecessors, distances, nodesInDistance)) {
            auto path = Path(graph,
                    start_vertex,
                    distance,
                    predecessors,
                    distances);
            path.sort_by_node_agg_cost();
            return path;
        }
    }

    /* The result is empty */
    Path p(start_vertex, start_vertex);
    p.push_back({start_vertex, -1, 0, 0});
    return p;
}

/** @brief Preparing driving distance BGL call for multimple starts with no equi-cost
 *
 * Prepares the execution for a driving distance for multiple starts when
 *
 * @tparam G graph instance from Pgr_base_graph
 * @param[in] graph Original graph
 * @param[in] start_vertex the starting vertex
 * @param[in] distance The limit distance
 */
template <class G>
std::deque<Path>
drivingDistance_no_equicost(
        const G &graph,
        const std::set<int64_t> &start_vertex,
        double distance) {
    std::deque<Path> paths;
    for (const auto &vertex : start_vertex) {
        paths.push_back(execute_drivingDistance(graph, vertex, distance));
    }
    return paths;
}

/** @brief to use with driving distance
 *
 * Prepares the execution for a driving distance when there is equi-cost
 *
 * @tparam G graph instance from Pgr_base_graph
 * @tparam V graph The corresponding vertex descriptor
 * @param[in] graph Original graph
 * @param[in] start_vertex the starting vertex
 * @param[in] distance The limit distance
 * @param[in/out] predecessors Common list of predecessors
 * @param[in/out] distances Common list of distances
 *
 * Results are kept on predecessor & distances
 *
 * @returns bool  @b True when results are found
 */
template <class G, typename V>
bool execute_drivingDistance_no_init(
        G &graph,
        V start_vertex,
        double distance,
        std::vector<V> &predecessors,
        std::vector<double> &distances) {
    pgassert(predecessors.size() == graph.num_vertices());
    pgassert(distances.size() == graph.num_vertices());

    std::iota(predecessors.begin(), predecessors.end(), 0);

    return pgrouting::bg_algorithms::dijkstra_1_to_distance_no_init(
            graph.graph,
            start_vertex,
            distance,
            predecessors, distances);
}

/** @brief gets results in form of a container of paths
 *
 * @tparam G graph instance from Pgr_base_graph
 * @tparam V graph The corresponding vertex descriptor
 * @param[in] graph The graph that is being worked
 * @param[in] all_predecessors An array of predecessors
 * @param[in] distance the max distance
 * @param[in] distances the Array of minimum distances
 */
template <class G, typename V>
std::deque<Path>
get_drivingDistance_with_equicost_paths(
        G &graph,
        const std::map<int64_t, std::vector<V>> &all_predecessors,
        double distance,
        const std::vector<double> &distances) {
    /*
     * Creating all the result "paths"
     */
    std::map<int64_t, Path> paths;
    for (const auto &p : all_predecessors) {
        auto vertex = p.first;
        paths[vertex] = Path(vertex, vertex);
        paths[vertex].push_back({vertex, -1, 0, 0});
    }

    for (V v = 0; v < distances.size(); ++v) {
        /*
         * Sikiping distances greater than the one asked for
         */
        if (!(distances[v] <= distance)) continue;

        for (auto iter = all_predecessors.rbegin(); iter != all_predecessors.rend(); ++iter) {
            auto vertex = iter->first;
            const auto predecessors = iter->second;

            /*
             * The vertex does not exist on the graph
             */
            if (predecessors.empty()) break;
            /*
             * The predecessor = current then its unreachable to this vertex
             */
            if (predecessors[v] == v) continue;

            auto cost = distances[v] - distances[predecessors[v]];
            auto edge_id = graph.get_edge_id(predecessors[v], v, cost);
            paths[vertex].push_back({graph[v].id, edge_id, cost, distances[v]});

            break;
        }
    }

    std::deque<Path> result;
    for (auto &p : paths) {
        p.second.sort_by_node_agg_cost();
        result.push_back(p.second);
    }
    return result;
}


/* preparation for many to distance with equicost
 *
 * @tparam G graph instance from Pgr_base_graph
 * Idea:
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
template <class G>
std::deque<Path>
drivingDistance_with_equicost(
        G &graph,
        std::set<int64_t> start_vertex,
        double distance) {
    using V = typename G::V;

    std::deque<V> nodesInDistance;
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    /*
     * Vector to store the different predessesors
     * each is of size = graph.num_vertices()
     */
    std::map<int64_t, std::vector<V>> all_predecessors;

    /*
     * perform the algorithm
     */
    for (const auto &vertex : start_vertex) {
        nodesInDistance.clear();
        /*
         * The vertex does not exist
         *   Nothing to do
         */
        if (graph.has_vertex(vertex)) {
            if (execute_drivingDistance_no_init(
                        graph,
                        graph.get_V(vertex),
                        distance,
                        predecessors, distances)) {
                all_predecessors[vertex] = predecessors;
            }
        } else {
            all_predecessors[vertex] = {};
        }
    }

    /*
     * predecessors of vertices in the set are themselves
     */
    for (const auto &vertex : start_vertex) {
        for (auto &p : all_predecessors) {
            if (!p.second.empty() && graph.has_vertex(vertex)) {
                p.second[graph.get_V(vertex)] = graph.get_V(vertex);
                pgassert(p.second.size() == graph.num_vertices());
            }
        }
    }

    return get_drivingDistance_with_equicost_paths(
            graph,
            all_predecessors,
            distance,
            distances);
}


/** Dijkstra 1 to many
 * @tparam G graph instance from Pgr_base_graph
 */
template <class G>
std::deque<Path> dijkstra_one_to_many(
        const G &graph,
        int64_t start_vertex,
        const std::set<int64_t> &end_vertex,
        bool only_cost,
        size_t n_goals) {
    using V = typename G::V;
    /*
     * adjust predecessors and distances vectors
     */
    std::vector<V> predecessors;
    std::vector<double> distances;
    predecessors.resize(graph.num_vertices());
    distances.resize(
            graph.num_vertices(),
            std::numeric_limits<double>::infinity());

    /*
     * get the graphs source and target
     */
    if (!graph.has_vertex(start_vertex)) return std::deque<Path>();

    auto v_source(graph.get_V(start_vertex));

    std::set<V> v_targets;
    for (const auto &vertex : end_vertex) {
        if (graph.has_vertex(vertex)) v_targets.insert(graph.get_V(vertex));
    }

    // perform the algorithm
    pgrouting::bg_algorithms::dijkstra_1_to_many(graph.graph, v_source, v_targets, n_goals, predecessors, distances);

    std::deque<Path> paths;
    // get the results // route id are the targets
    paths = get_paths(graph, v_source, v_targets, only_cost, predecessors, distances);

    return paths;
}

}  // namespace detail

/* **************************************************************************************************************** */

/**
 * @brief Driving distance for a set of start vertices
 *
 * @tparam G graph instance from Pgr_base_graph
 * @param[in] graph instance from Pgr_base_graph
 * @param[in] start_vertex original vertex identifier
 * @param[in] distance include vertices within the distance
 * @param[in] equicost Use equicost
 * @return Path
 */
template <class G>
std::deque<Path>
drivingDistance(
        const G &graph,
        const std::set<int64_t> &start_vertex,
        double distance,
        bool equicost) {
    return equicost?
        detail::drivingDistance_with_equicost(graph, start_vertex, distance)
        : detail::drivingDistance_no_equicost(graph, start_vertex, distance);
}

/**
 * @brief Dijkstra for one path
 *
 * @tparam G graph instance from Pgr_base_graph
 * @param[in] graph instance from Pgr_base_graph
 * @param[in] start_vertex original vertex identifier
 * @param[in] distance include vertices within the distance
 * @param[in] only_cost Get only the cost summaries
 * @return Path
 */
template <class G>
Path dijkstra(
        const G &graph,
        int64_t start_vertex,
        int64_t end_vertex,
        bool only_cost = false) {
    using V = typename G::V;
    /* adjust predecessors and distances vectors */
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    if (!graph.has_vertex(start_vertex) || !graph.has_vertex(end_vertex)) {
        return Path(start_vertex, end_vertex);
    }

    /* get the graphs source and target */
    auto v_source(graph.get_V(start_vertex));
    auto v_target(graph.get_V(end_vertex));

    /* perform the algorithm */
    bg_algorithms::dijkstra_1_to_1(graph.graph, v_source, v_target, predecessors, distances);

    /* get the results */
    return Path(
            graph,
            v_source, v_target,
            predecessors, distances,
            only_cost, true);
}

/**
 * @brief Dijkstra for combinations of from -> to
 *
 * @tparam G graph instance from Pgr_base_graph
 * @param[in] graph instance from Pgr_base_graph
 * @param[in] from -> to combinations to be calculated
 * @param[in] only_cost Get only the cost summaries
 * @param[in] n_goals For DijkstraNear
 * @return Path
 */
template <class G>
std::deque<Path>
dijkstra(
        const G &graph,
        const std::map<int64_t , std::set<int64_t>> &combinations,
        bool only_cost,
        size_t n_goals) {
    std::deque<Path> paths;

    for (const auto &c : combinations) {
        auto r_paths = detail::dijkstra_one_to_many(
                graph,
                c.first, c.second,
                only_cost, n_goals);
        paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
    }

    return paths;
}

}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_DIJKSTRA_HPP_
