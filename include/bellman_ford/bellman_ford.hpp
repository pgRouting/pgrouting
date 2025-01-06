/*PGR-GNU*****************************************************************
File: bellman_ford.hpp

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

#ifndef INCLUDE_BELLMAN_FORD_BELLMAN_FORD_HPP_
#define INCLUDE_BELLMAN_FORD_BELLMAN_FORD_HPP_
#pragma once

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <functional>
#include <limits>
#include <map>
#include <cstdint>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include "cpp_common/messages.hpp"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "c_types/ii_t_rt.h"

namespace pgrouting {

template < class G >
class Pgr_bellman_ford : public pgrouting::Pgr_messages {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     //@{



     /** @brief bellman_ford one to a set of many
      * @todo document this function
      */

     std::deque<Path> bellman_ford(
             G &graph,
             int64_t start_vertex,
             const std::set<int64_t> &end_vertex,
             bool only_cost = false) {
         // adjust predecessors and distances vectors
         clear();
         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices());

         // get the graphs source and targets
         if (!graph.has_vertex(start_vertex)) return std::deque<Path>();
         auto v_source(graph.get_V(start_vertex));

         std::set<V> s_v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) {
                 s_v_targets.insert(graph.get_V(vertex));
             }
         }

         std::vector<V> v_targets(s_v_targets.begin(), s_v_targets.end());
         // perform the algorithm
         bellman_ford_1_to_many(graph, v_source);

         // get the results // route id are the targets
         auto paths = get_paths(graph, v_source, v_targets, only_cost);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });

         return paths;
     }


     /** @brief BellmanFord combinations **/
     std::deque<Path> bellman_ford(
             G &graph,
             const std::map<int64_t, std::set<int64_t>> &combinations,
             bool only_cost = false) {
         std::deque<Path> paths;

         for (const auto &comb : combinations) {
             if (!graph.has_vertex(comb.first)) continue;
             auto tmp_paths = bellman_ford(graph, comb.first, comb.second, only_cost);
             paths.insert(paths.end(), tmp_paths.begin(), tmp_paths.end());
         }

         return paths;
     }

     //@}

 private:
     //! Call to BellmanFord  1 source to many targets
     bool bellman_ford_1_to_many(
             G &graph,
             V source) {
        log << std::string(__FUNCTION__) << "\n";
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         try {
             boost::bellman_ford_shortest_paths(
                     graph.graph,
                     static_cast<int>(graph.num_vertices()),
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .root_vertex(source));
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
     std::vector<V> predecessors;
     std::vector<double> distances;

     //@}
};

}  // namespace pgrouting

#endif  // INCLUDE_BELLMAN_FORD_BELLMAN_FORD_HPP_
