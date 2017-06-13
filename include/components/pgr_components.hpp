/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Maoguang Wang 
xjtumg1007@gmail.com

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

#ifndef INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <deque>
#include <set>
#include <vector>
#include <algorithm>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#if 0
#include "./../../common/src/signalhandler.h"
#endif


template < class G > class Pgr_components;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_components {
 public:
     typedef typename G::V V;

     //! @name Components 
     //@{
     //! one to one
     Path components(
             G &graph);

 private:
     //! Call to Dijkstra  1 source to 1 target
     bool dijkstra_1_to_1(
             G &graph,
             V source,
             V target);

     void clear() {
         predecessors.clear();
         distances.clear();
         nodesInDistance.clear();
     }

     //! @name members;
     //@{
     struct found_goals{};  //!< exception for termination
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::deque< V > nodesInDistance;
     //@}

     //! @name Stopping classes
     //@{
     //! class for stopping when 1 target is found
     class dijkstra_one_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_one_goal_visitor(V goal) : m_goal(goal) {}
          template <class B_G>
              void examine_vertex(V &u, B_G &g) {
#if 0
                  REG_SIGINT;
                  THROW_ON_SIGINT;
#endif
                  if (u == m_goal) throw found_goals();
                  num_edges(g);
              }
      private:
          V m_goal;
     };

     //@}
};


/******************** IMPLEMENTTION ******************/


//! Components
template < class G >
Path
Pgr_components< G >::components(
        G &graph) {
    int64_t end_vertex = 0;
    int64_t start_vertex = 0;
    bool only_cost = false;
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
    dijkstra_1_to_1(graph, v_source, v_target);

    // get the results
    return Path(
            graph,
            v_source, v_target,
            predecessors, distances,
            only_cost, true);
}

//! Call to Dijkstra  1 source to 1 target
template < class G >
bool
Pgr_components< G >::dijkstra_1_to_1(
        G &graph,
        V source,
        V target) {
    bool found = false;
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(dijkstra_one_goal_visitor(target)));
    }
    catch(found_goals &) {
        found = true;  // Target vertex found
    } catch (...) {
    }
    return found;
}

#endif  // INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
