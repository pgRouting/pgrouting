/*PGR-GNU*****************************************************************
File: bg_dijkstra.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: Vicky_vergara@hotmail.com

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakr@yahoo.com, estebanzimanyi@gmail.com

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

#ifndef INCLUDE_DIJKSTRA_BG_DIJKSTRA_HPP_
#define INCLUDE_DIJKSTRA_BG_DIJKSTRA_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <map>
#include <deque>
#include <limits>
#include <vector>
#include <set>
#include <functional>

#include "cpp_common/interruption.h"

#include "visitors/found_goals.hpp"
#include "visitors/dijkstra_one_goal_visitor.hpp"
#include "visitors/dijkstra_distance_visitor_no_init.hpp"
#include "visitors/dijkstra_many_goal_visitor.hpp"
#include "visitors/dijkstra_distance_visitor.hpp"

namespace pgrouting {
namespace bg_algorithms {

template <typename G, typename V>
bool dijkstra_1_to_1(
        G &graph,
        V source,
        V target,
        std::vector<V> &predecessors,
        std::vector<double> &distances
        ) {
    using E_Type = typename boost::edge_bundle_type<G>::type;

    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&E_Type::cost, graph))
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::dijkstra_one_goal_visitor<V>(target)));
    } catch(pgrouting::found_goals &) {
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

/** Call to Dijkstra  1 to distance
 *
 * Used on:
 *   1 to distance
 *   many to distance
 *   On the first call of many to distance with equi_cost
 */
template <typename G, typename V>
bool
dijkstra_1_to_distance(
        G &graph,
        V source,
        double distance,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        std::deque<V> &nodesInDistance) {
    using E_Type = typename boost::edge_bundle_type<G>::type;
    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&E_Type::cost, graph))
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::dijkstra_distance_visitor<V>(
                        distance,
                        nodesInDistance,
                        distances)));
    } catch(pgrouting::found_goals &) {
        return true;
    } catch (boost::exception const&) {
        throw;
    } catch (std::exception&) {
        throw;
    } catch (...) {
        throw;
    }
    return true;
}

/** Call to Dijkstra  1 to distance no init
 *
 * Used on:
 *   On the subsequent calls of many to distance with equi_cost
 */
template <typename G, typename V>
bool
dijkstra_1_to_distance_no_init(
        G &graph,
        V source,
        double distance,
        std::vector<V> &predecessors,
        std::vector<double> &distances) {
    pgassert(predecessors.size() == num_vertices(graph));
    pgassert(distances.size() == num_vertices(graph));

    using E_Type = typename boost::edge_bundle_type<G>::type;
    typename boost::property_map<G, boost::vertex_index_t>::type vertIndex;

    distances[source] = 0;
    std::vector<boost::default_color_type> color_map(num_vertices(graph));
    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths_no_init(graph, source,
                make_iterator_property_map(
                    predecessors.begin(),
                    vertIndex),
                make_iterator_property_map(
                    distances.begin(),
                    vertIndex),
                get(&E_Type::cost, graph),
                vertIndex,
                std::less<double>(),
                boost::closed_plus<double>(),
                static_cast<double>(0),
                pgrouting::visitors::dijkstra_distance_visitor_no_init<V>(
                    source,
                    distance,
                    predecessors,
                    distances,
                    color_map),
                boost::make_iterator_property_map(
                    color_map.begin(),
                    vertIndex,
                    color_map[0]));
    } catch(pgrouting::found_goals &) {
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

/**
 * @brief Dijkstra  1 source to many targets
 */
template <typename G, typename V>
bool
dijkstra_1_to_many(
        G &graph,
        V source,
        const std::set<V> &goals,
        size_t n_goals,
        std::vector<V> &predecessors,
        std::vector<double> &distances) {
    using E_Type = typename boost::edge_bundle_type<G>::type;

    CHECK_FOR_INTERRUPTS();
    std::set<V> goals_found;
    try {
        boost::dijkstra_shortest_paths(graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&E_Type::cost, graph))
                .distance_map(&distances[0])
                .distance_inf(std::numeric_limits<double>::infinity())
                .visitor(pgrouting::visitors::dijkstra_many_goal_visitor<V>(goals, n_goals, goals_found)));
    } catch(pgrouting::found_goals &) {
        for (const auto &g : goals) {
            if (goals_found.find(g) == goals_found.end()) {
                /* goal was not found */
                predecessors[g] = g;
            }
        }
        return true;
    } catch (boost::exception const&) {
        throw;
    } catch (std::exception &) {
        throw;
    } catch (...) {
        throw;
    }
    return true;
}
}  // namespace bg_algorithms
}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_BG_DIJKSTRA_HPP_
