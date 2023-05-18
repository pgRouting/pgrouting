/*PGR-GNU*****************************************************************

File: astar.hpp

Copyright (c) 2015 Vicky Vergara
Mail: vicky at erosion.dev
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

#ifndef INCLUDE_ASTAR_PGR_ASTAR_HPP_
#define INCLUDE_ASTAR_PGR_ASTAR_HPP_
#pragma once

#include <cmath>
#include <deque>
#include <limits>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>
#include <visitors/astar_visitors.hpp>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
#include "c_types/ii_t_rt.h"

namespace {

template <typename B_G, typename V>
class distance_heuristic : public boost::astar_heuristic<B_G, double> {
 public:
     distance_heuristic(B_G &g, V goal, int heuristic, double factor)
         : m_g(g),
         m_factor(factor),
         m_heuristic(heuristic) {
             m_goals.insert(goal);
         }
     distance_heuristic(
             B_G &g,
             const std::set<V> &goals,
             int heuristic,
             double factor)
         : m_g(g),
         m_goals(goals),
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
};  // class distance_heuristic

bool astar_1_to_many(
        G &graph,
        V source,
        const std::set<V> &targets,
        int heuristic,
        double factor,
        double epsilon) {
    bool found = false;
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    try {
        boost::astar_search(
                graph.graph, source,
                distance_heuristic<B_G, V>(
                    graph.graph, targets,
                    heuristic, factor * epsilon),
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&pgrouting::Basic_edge::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(visitors::astar_many_goals_visitor<V>(targets)));
    }
    catch(found_goals &) {
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

}  // namespace

namespace pgrouting {
namespace algorithms {

template < class G >
class Pgr_astar {
 public:
     typedef typename G::V V;
     typedef typename G::B_G B_G;


     void clear() {
         predecessors.clear();
         distances.clear();
     }

     //! astar 1 to many
     std::deque<Path> astar(
             G &graph,
             int64_t start_vertex,
             std::set<int64_t> end_vertex,
             int heuristic,
             double factor,
             double epsilon,
             bool only_cost) {
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices());

         if (!graph.has_vertex(start_vertex)) return std::deque<Path>();
         auto v_source(graph.get_V(start_vertex));

         std::set<V> v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) {
                 v_targets.insert(graph.get_V(vertex));
             }
         }

         astar_1_to_many(graph,
                 v_source,
                 v_targets,
                 heuristic,
                 factor,
                 epsilon);

         auto paths = get_paths(graph, predecessors, distances, v_source, v_targets, only_cost);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });

         return paths;
     }

     // preparation for parallel arrays
     /* TODO make this a function */
     std::deque<Path> astar(
             G &graph,
             const std::map<int64_t, std::set<int64_t>> &combinations,
             int heuristic,
             double factor,
             double epsilon,
             bool only_cost) {
         // a call to 1 to many is faster for each of the sources
         std::deque<Path> paths;

         for (const auto &c : combinations) {
             auto r_paths = astar(
                     graph,
                     c.first, c.second,
                     heuristic, factor, epsilon, only_cost);
             paths.insert(paths.end(), r_paths.begin(), r_paths.end());
         }

         return paths;
     }
     //@}



 private:
     //! @name members;
     //@{
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::deque< V > nodesInDistance;
     //@}


};


}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_ASTAR_PGR_ASTAR_HPP_
