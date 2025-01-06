/*PGR-GNU*****************************************************************
File: astar.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Vicky Vergara
Copyright (c) 2015 Vicky Vergara
Mail: vicky at erosion.dev

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

#ifndef INCLUDE_ASTAR_ASTAR_HPP_
#define INCLUDE_ASTAR_ASTAR_HPP_
#pragma once

#include <deque>
#include <vector>
#include <limits>
#include <set>
#include <map>
#include <cstdint>

#include <boost/graph/astar_search.hpp>

#include "visitors/astar_visitors.hpp"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "c_types/ii_t_rt.h"

namespace detail {

template <typename B_G, typename V>
class distance_heuristic : public boost::astar_heuristic<B_G, double> {
 public:
     distance_heuristic(
             B_G &g,
             const std::set<V> &destinations,
             int heuristic,
             double factor)
         : m_g(g),
         m_goals(destinations),
         m_factor(factor),
         m_heuristic(heuristic) {}

     double operator()(V u) {
         if (m_heuristic == 0) return 0;
         if (m_goals.empty()) return 0;
         double best_h((std::numeric_limits<double>::max)());
         for (auto goal : m_goals) {
             double current((std::numeric_limits<double>::max)());
             double dx = m_g[goal].x() - m_g[u].x();
             double dy = m_g[goal].y() - m_g[u].y();
             switch (m_heuristic) {
                 case 0:
                     current = 0;
                     break;
                 case 1:
                     current = std::fabs((std::max)(dx, dy)) * m_factor;
                     break;
                 case 2:
                     current = std::fabs((std::min)(dx, dy)) * m_factor;
                     break;
                 case 3:
                     current = (dx * dx + dy * dy) * m_factor * m_factor;
                     break;
                 case 4:
                     current = std::sqrt(dx * dx + dy * dy) * m_factor;
                     break;
                 case 5:
                     current = (std::fabs(dx) + std::fabs(dy)) * m_factor;
                     break;
                 default:
                     current = 0;
             }
             if (current < best_h) {
                 best_h = current;
             }
         }
         {
             auto s_it = m_goals.find(u);
             if (!(s_it == m_goals.end())) {
                 // found one more goal
                 m_goals.erase(s_it);
             }
         }
         return best_h;
     }

 private:
     B_G &m_g;
     std::set<V> m_goals;
     double m_factor;
     int m_heuristic;
};

template <typename G, typename V>
bool astar_1_to_many(
        G &graph,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        V departure,
        const std::set<V> &destinations,
        int heuristic,
        double factor,
        double epsilon) {
    typedef typename G::B_G B_G;
    bool found = false;
    CHECK_FOR_INTERRUPTS();
    try {
        boost::astar_search(
                graph.graph, departure,
                distance_heuristic<B_G, V>(
                    graph.graph, destinations,
                    heuristic, factor * epsilon),
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&pgrouting::Basic_edge::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::astar_many_goals_visitor<V>(destinations)));
    }
    catch(pgrouting::found_goals &) {
        found = true;  // Target vertex found
    }
    return found;
}

template <typename G, typename V>
std::deque<pgrouting::Path> get_paths(
        const G &graph,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        V source,
        const std::set<V> &targets,
        bool only_cost) {
    using Path = pgrouting::Path;
    std::deque<Path> paths;
    for (const auto &target : targets) {
        auto p = Path(graph,
                source, target,
                predecessors, distances,
                false);
        paths.push_back(Path(graph, p, only_cost));
    }
    return paths;
}

}  // namespace detail

namespace pgrouting {
namespace algorithms {

template < class G >
std::deque<Path> astar(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost) {
    typedef typename G::V V;
    std::deque<Path> paths;

    for (const auto &c : combinations) {
        if (!graph.has_vertex(c.first)) continue;
        std::vector<V> predecessors(graph.num_vertices());
        std::vector<double> distances(graph.num_vertices());

        auto departure(graph.get_V(c.first));
        std::set<V> destinations;

        for (const auto &vertex : c.second) {
            if (graph.has_vertex(vertex)) {
                destinations.insert(graph.get_V(vertex));
            }
        }

        detail::astar_1_to_many(graph, predecessors, distances, departure, destinations, heuristic, factor, epsilon);
        auto r_paths = detail::get_paths(graph, predecessors, distances, departure, destinations, only_cost);

        std::stable_sort(r_paths.begin(), r_paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e1.end_id() < e2.end_id();
                });
        paths.insert(paths.end(), r_paths.begin(), r_paths.end());
    }

    return paths;
}

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_ASTAR_ASTAR_HPP_
