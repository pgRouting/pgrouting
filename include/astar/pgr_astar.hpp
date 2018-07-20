/*PGR-GNU*****************************************************************

File: pgr_astar.hpp

Copyright (c) 2015 Vicky Vergara
Mail: vicky_vergara@hotmail.com
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

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>

#include <cmath>


#include <deque>
#include <limits>
#include <algorithm>
#include <vector>
#include <set>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"

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

     //! @name Astar
     //@{
     //! one to one
     //! astar 1 to 1
     Path astar(
             G &graph,
             int64_t start_vertex,
             int64_t end_vertex,
             int heuristic,
             double factor,
             double epsilon,
             bool only_cost) {
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices());

         if (!graph.has_vertex(start_vertex)
                 || !graph.has_vertex(end_vertex)) {
             return Path(start_vertex, end_vertex);
         }

         auto v_source(graph.get_V(start_vertex));
         auto v_target(graph.get_V(end_vertex));

         // perform the algorithm
         astar_1_to_1(graph, v_source, v_target, heuristic, factor, epsilon);

         return  Path(graph,
                 v_source, v_target,
                 predecessors, distances,
                 only_cost);
     }

     //! astar 1 to many
     std::deque<Path> astar(
             G &graph,
             int64_t start_vertex,
             std::vector<int64_t> end_vertex,
             int heuristic,
             double factor,
             double epsilon,
             bool only_cost) {
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices());

         if (!graph.has_vertex(start_vertex)) return std::deque<Path>();
         auto v_source(graph.get_V(start_vertex));

         std::vector<V> v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) {
                 v_targets.push_back(graph.get_V(vertex));
             }
         }

         astar_1_to_many(graph,
                 v_source,
                 v_targets,
                 heuristic,
                 factor,
                 epsilon);

         auto paths = get_paths(graph, v_source, v_targets, only_cost);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });

         return paths;
     }

     // preparation for many to many
     std::deque<Path> astar(
             G &graph,
             std::vector<int64_t> start_vertex,
             std::vector<int64_t> end_vertex,
             int heuristic,
             double factor,
             double epsilon,
             bool only_cost) {
         std::deque<Path> paths;
         for (const auto &start : start_vertex) {
             auto r_paths = astar(graph, start, end_vertex,
                     heuristic, factor, epsilon, only_cost);
              paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
         }

         std::sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });
         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.start_id() < e2.start_id();
                 });
         return paths;
     }
     //@}



 private:
     //! @name members;
     //@{
     struct found_goals{};  //!< exception for termination
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::deque< V > nodesInDistance;
     //@}

     // heuristic for one goal
     class distance_heuristic : public boost::astar_heuristic< B_G, double > {
      public:
          distance_heuristic(B_G &g, V goal, int heuristic, double factor)
              : m_g(g),
              m_factor(factor),
              m_heuristic(heuristic) {
                  m_goals.insert(goal);
              }
          distance_heuristic(
                  B_G &g,
                  std::vector< V > goals,
                  int heuristic,
                  double factor)
              : m_g(g),
              m_goals(goals.begin(), goals.end()),
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
          std::set< V > m_goals;
          double m_factor;
          int m_heuristic;
     };  // class distance_heuristic


     //! visitor that terminates when we find the goal
     class astar_one_goal_visitor : public boost::default_astar_visitor {
      public:
          explicit astar_one_goal_visitor(V goal) : m_goal(goal) {}
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
                  if (u == m_goal)
                      throw found_goals();
                  // using g, otherwise is throws a warning
                  num_edges(g);
              }
      private:
          V m_goal;
     };  // class astar_one_goal_visitor

     //! class for stopping when all targets are found
     class astar_many_goals_visitor : public boost::default_astar_visitor {
      public:
          explicit astar_many_goals_visitor(std::vector< V > goals)
              :m_goals(goals.begin(), goals.end()) {}
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
                  auto s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;
                  // found one more goal
                  m_goals.erase(s_it);
                  if (m_goals.size() == 0) throw found_goals();
                  num_edges(g);
              }
      private:
          std::set< V > m_goals;
     };

     /******************** IMPLEMENTTION ******************/



     //! Call to Astar  1 source to 1 target
     bool astar_1_to_1(
             G &graph,
             V source,
             V target,
             int heuristic,
             double factor,
             double epsilon) {
         bool found = false;
         try {
             // Call A* named parameter interface
             boost::astar_search(
                     graph.graph, source,
                     distance_heuristic(graph.graph, target,
                         heuristic, factor * epsilon),
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&pgrouting::Basic_edge::cost, graph.graph))
                     .distance_map(&distances[0])
                     .visitor(astar_one_goal_visitor(target)));
         }
         catch(found_goals &) {
             found = true;  // Target vertex found
         }
         return found;
     }


     //! Call to astar  1 source to many targets
     bool astar_1_to_many(
             G &graph,
             V source,
             const std::vector< V > &targets,
             int heuristic,
             double factor,
             double epsilon) {
         bool found = false;
         try {
             boost::astar_search(
                     graph.graph, source,
                     distance_heuristic(
                         graph.graph, targets,
                         heuristic, factor * epsilon),
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&pgrouting::Basic_edge::cost, graph.graph))
                     .distance_map(&distances[0])
                     .visitor(astar_many_goals_visitor(targets)));
         }
         catch(found_goals &) {
             found = true;  // Target vertex found
         }
         return found;
     }


     /*
      * GET_PATHS
      */


     std::deque<Path> get_paths(
             const G &graph,
             V source,
             const std::vector<V> &targets,
             bool only_cost) const {
         std::deque<Path> paths;
         for (const auto &target : targets) {
             paths.push_back(
                     Path(graph,
                         source, target,
                         predecessors, distances,
                         only_cost));
         }
         return paths;
     }
};


}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_ASTAR_PGR_ASTAR_HPP_
