/*PGR-GNU*****************************************************************
File: dijkstra.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2022 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakri at yahoo.com, estebanzimanyi at gmail.com

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
#include <cstdint>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "c_types/ii_t_rt.h"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "visitors/dijkstra_visitors.hpp"

namespace detail {

template <typename G, typename V>
std::deque<pgrouting::Path> get_paths(
        const G &graph,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        V source,
        std::set<V> &targets,
        bool only_cost) {
    std::deque<pgrouting::Path> paths;
    for (const auto target : targets) {
        paths.push_back(pgrouting::Path(
                    graph,
                    source, target,
                    predecessors, distances,
                    only_cost, true));
    }
    return paths;
}

template <typename B_G, typename V, typename T_E>
bool dijkstra_1_to_many(
        B_G &graph,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        V departure,
        const std::set<V> &destinations,
        size_t n_goals) {
    CHECK_FOR_INTERRUPTS();
    std::set<V> goals_found;
    try {
        boost::dijkstra_shortest_paths(graph, departure,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&T_E::cost, graph))
                .distance_map(&distances[0])
                .distance_inf(std::numeric_limits<double>::infinity())
                .visitor(pgrouting::visitors::dijkstra_many_goal_visitor<V>(destinations, n_goals, goals_found)));
    } catch(pgrouting::found_goals &) {
        for (const auto &g : destinations) {
            if (goals_found.find(g) == goals_found.end()) {
                /* goal was not found */
                predecessors[g] = g;
            }
        }
        return true;
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
             throw;
         }
         return true;
     }


//! Dijkstra 1 to many
template <class G>
std::deque<pgrouting::Path> dijkstra(
        G &graph,
        int64_t start_vertex,
        const std::set<int64_t> &end_vertex,
        bool only_cost,
        size_t n_goals) {
    typedef typename G::V V;
    typedef typename G::G_T_E T_E;
    typedef typename G::B_G B_G;
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    if (!graph.has_vertex(start_vertex)) return std::deque<pgrouting::Path>();

    auto departure(graph.get_V(start_vertex));

    std::set<V> destinations;
    for (const auto &vertex : end_vertex) {
        if (graph.has_vertex(vertex)) destinations.insert(graph.get_V(vertex));
    }

    // perform the algorithm
    detail::dijkstra_1_to_many<B_G, V, T_E>(graph.graph, predecessors, distances, departure, destinations, n_goals);

    // get the results
    auto paths = get_paths(graph,  predecessors, distances, departure, destinations, only_cost);

    return paths;
}

}  // namespace detail


namespace pgrouting {
namespace algorithms {

template <class G>
std::deque<Path> dijkstra(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        bool only_cost,
        size_t n_goals = (std::numeric_limits<size_t>::max)()) {
    std::deque<Path> paths;

    for (const auto &c : combinations) {
        auto r_paths = detail::dijkstra<G>(graph, c.first, c.second, only_cost, n_goals);
        paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
    }

    return paths;
}

/* 1 to 1*/
template <class G>
Path dijkstra(
        G &graph,
        int64_t start_vertex,
        int64_t end_vertex,
        bool only_cost = false) {
    typedef typename G::V V;
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    std::map<int64_t, std::set<int64_t>> combinations;
    combinations[start_vertex].insert(end_vertex);

    auto paths = dijkstra(graph, combinations, only_cost);
    return paths.front();
}


}  // namespace algorithms
}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_DIJKSTRA_HPP_
