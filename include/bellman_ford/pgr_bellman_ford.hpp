/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Sourabh Garg
Mail to: sourabh.garg.mat14@gmail.com

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

#ifndef INCLUDE_BELLMAN_FORD_PGR_BELLMAN_FORD_HPP_
#define INCLUDE_BELLMAN_FORD_PGR_BELLMAN_FORD_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"



//******************************************

template < class G >
class Pgr_bellman_ford {
 public:
     typedef typename G::V V;
     typedef typename G::E E;

     //@}

     //! @name BellmanFord
     //@{
     //! BellmanFord 1 to 1
     Path bellman_ford(
             G &graph,
             int64_t start_vertex,
             int64_t end_vertex,
             bool only_cost = false) {
         clear();

         // adjust predecessors and distances vectors
         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices());


         if (!graph.has_vertex(start_vertex)
                 || !graph.has_vertex(end_vertex)) {
             return Path(start_vertex, end_vertex);
         }

         // get the graphs source and target
         auto v_source(graph.get_V(start_vertex));
         auto v_target(graph.get_V(end_vertex));

         // perform the algorithm
         bellman_ford_1_to_1(graph, v_source, v_target);

         // get the results
         return Path(
                 graph,
                 v_source, v_target,
                 predecessors, distances,
                 only_cost, true);
     }


     //! BellmanFord 1 to many
     std::deque<Path> bellman_ford(
             G &graph,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost= false) {
         // adjust predecessors and distances vectors
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices());

         // get the graphs source and target
         if (!graph.has_vertex(start_vertex))
             return std::deque<Path>();
         auto v_source(graph.get_V(start_vertex));

         std::set< V > s_v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) {
                 s_v_targets.insert(graph.get_V(vertex));
             }
         }

         std::vector< V > v_targets(s_v_targets.begin(), s_v_targets.end());
         // perform the algorithm
         bellman_ford_1_to_many(graph, v_source, v_targets);

         std::deque< Path > paths;
         // get the results // route id are the targets
         paths = get_paths(graph, v_source, v_targets, only_cost);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });

         return paths;
     }

     // BellmanFord many to 1
     std::deque<Path> bellman_ford(
             G &graph,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex,
             bool only_cost = false) {
         std::deque<Path> paths;

         for (const auto &start : start_vertex) {
             paths.push_back(
                     bellman_ford(graph, start, end_vertex, only_cost));
         }

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.start_id() < e2.start_id();
                 });
         return paths;
     }


     // BellmanFord many to many
     std::deque<Path> bellman_ford(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false) {
         // a call to 1 to many is faster for each of the sources
         std::deque<Path> paths;
         for (const auto &start : start_vertex) {
             auto r_paths = bellman_ford(graph, start, end_vertex, only_cost);
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
     //! Call to BellmanFord  1 source to 1 target
     bool bellman_ford_1_to_1(
                 G &graph,
                 V source,
                 V target) {
         try {
             boost::bellman_ford_shortest_paths(graph.graph, int(graph.num_vertices()),
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .root_vertex(source)
                     .visitor(bellman_ford_one_goal_visitor(target)));
         } catch(found_goals &) {
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
     //! Call to BellmanFord  1 source to many targets
     bool bellman_ford_1_to_many(
             G &graph,
             V source,
             const std::vector< V > &targets) {
         try {
             boost::bellman_ford_shortest_paths(graph.graph, int(graph.num_vertices()),
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .root_vertex(source)
                     .visitor(bellman_ford_many_goal_visitor(targets))
                     );
         } catch(found_goals &) {
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


     // To Empty predecessors and distances vector for each function call 
     void clear() {
         predecessors.clear();
         distances.clear();
     }


     // used when multiple goals
     std::deque<Path> get_paths(
             const G &graph,
             V source,
             std::vector< V > &targets,
             bool only_cost) const {
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



     //! @name members
     //@{
     struct found_goals{};  //!< exception for termination
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::ostringstream log;
     //@}



     //! @name Stopping classes
     //@{

     //! class for stopping when 1 target is found
     class bellman_ford_one_goal_visitor : public boost::default_bellman_visitor {
      public:
          explicit bellman_ford_one_goal_visitor(V goal) : m_goal(goal) {}
          template <class B_G>
              void examine_vertex(V &u, B_G &) {
                  if (u == m_goal) throw found_goals();
              }
      private:
          V m_goal;
     };


     //! class for stopping when all targets are found
     class bellman_ford_many_goal_visitor : public boost::default_bellman_visitor {
      public:
          explicit bellman_ford_many_goal_visitor(std::vector< V > goals)
              :m_goals(goals.begin(), goals.end()) {}
          template <class B_G>
              void examine_vertex(V u, B_G &) {
                  auto s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;
                  // we found one more goal
                  m_goals.erase(s_it);
                  if (m_goals.size() == 0) throw found_goals();
              }
      private:
          std::set< V > m_goals;
     };


     //@}
};



#endif  // INCLUDE_BELLMAN_FORD_PGR_BELLMAN_FORD_HPP_
