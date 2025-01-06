/*PGR-GNU*****************************************************************
File: dagShortestPath.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Sourabh Garg
sourabh.garg.mat@gmail.com

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

#ifndef INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
#define INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
#pragma once

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>
#include <map>
#include <cstdint>

#include <boost/config.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dag_shortest_paths.hpp>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

#include "c_types/ii_t_rt.h"

namespace pgrouting {

template <class G>
class Pgr_dag {
 public:
     typedef typename G::V V;
     typedef typename G::E E;


     /** dag 1 to many */
     std::deque<Path> dag(
             G &graph,
             int64_t start_vertex,
             const std::set<int64_t> &end_vertex,
             bool only_cost) {
         std::deque<Path> paths;

         /* precondition */
         if (!graph.has_vertex(start_vertex)) return paths;

         /* adjust predecessors and distances vectors */
         clear();
         size_t n_goals = (std::numeric_limits<size_t>::max)();
         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());


         auto v_source(graph.get_V(start_vertex));

         std::set<V> v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) v_targets.insert(graph.get_V(vertex));
         }
         if (v_targets.empty()) return paths;

         dag_1_to_many(graph, v_source, v_targets, n_goals);
         paths = get_paths(graph, v_source, v_targets, only_cost);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });

         return paths;
     }


     /** combinations */
     std::deque<Path> dag(
             G &graph,
             const std::map<int64_t, std::set<int64_t>> &combinations,
             bool only_cost) {
         std::deque<Path> paths;

         for (const auto &c : combinations) {
             auto result_paths = dag(graph, c.first, c.second, only_cost);
             paths.insert(
                     paths.end(),
                     std::make_move_iterator(result_paths.begin()),
                     std::make_move_iterator(result_paths.end()));
         }

         return paths;
     }

     //@}

 private:
     /** DAG  1 source to many targets */
     bool dag_1_to_many(
             G &graph,
             V source,
             const std::set<V> &targets,
             size_t n_goals = (std::numeric_limits<size_t>::max)()) {
         CHECK_FOR_INTERRUPTS();
         try {
             boost::dag_shortest_paths(graph.graph, source,
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .distance_inf(std::numeric_limits<double>::infinity())
                     .visitor(dijkstra_many_goal_visitor(targets, n_goals)));
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


     void clear() {
         predecessors.clear();
         distances.clear();
         nodesInDistance.clear();
     }




     // used when multiple goals
     std::deque<Path> get_paths(
             const G &graph,
             V source,
             std::set<V> &targets,
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
     std::deque< V > nodesInDistance;
     std::ostringstream log;
     //@}



     //! class for stopping when all targets are found
     class dijkstra_many_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_many_goal_visitor(
                  const std::set<V> &goals,
                  size_t n_goals) :
              m_goals(goals),
              m_n_goals(n_goals) {}
          template <class B_G>
              void examine_vertex(V u, B_G &) {
                  auto s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;

                  // found one more goal
                  m_goals.erase(s_it);

                  // all goals found
                  if (m_goals.size() == 0) throw found_goals();

                  // number of requested goals found
                  --m_n_goals;
                  if (m_n_goals == 0) throw found_goals();
              }

      private:
          std::set<V> m_goals;
          size_t m_n_goals;
     };
};

}  // namespace pgrouting

#endif  // INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
