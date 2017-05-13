/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
#define INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
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


template < class G > class Pgr_dijkstra;
// user's functions
// for development

template < class G >
std::deque<Path>
pgr_drivingDistance(
        G &graph,
        std::vector< int64_t > start_vids,
        double distance,
        bool equicost) {
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.drivingDistance(graph, start_vids, distance, equicost);
}


template < class G >
Path
pgr_drivingDistance(
        G &graph,
        int64_t  source,
        double distance) {
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.drivingDistance(graph, source, distance);
}


/* 1 to 1*/
template < class G >
Path
pgr_dijkstra(
        G &graph,
        int64_t source,
        int64_t target,
        bool only_cost = false) {
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.dijkstra(graph, source, target, only_cost);
}

/* 1 to many*/
template < class G >
std::deque<Path>
pgr_dijkstra(
        G &graph,
        int64_t  source,
        std::vector< int64_t > targets,
        bool only_cost = false) {
    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.dijkstra(graph, source, targets, only_cost);
}

/* many to 1*/
template < class G >
std::deque<Path>
pgr_dijkstra(
        G &graph,
        std::vector< int64_t > sources,
        int64_t  target,
        bool only_cost = false) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.dijkstra(graph, sources, target, only_cost);
}

/* Many to Many */
template < class G >
std::deque<Path>
pgr_dijkstra(
        G &graph,
        std::vector< int64_t > sources,
        std::vector< int64_t > targets,
        bool only_cost = false) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.dijkstra(graph, sources, targets, only_cost);
}



//******************************************

template < class G >
class Pgr_dijkstra {
 public:
     typedef typename G::V V;

     //! @name drivingDistance
     //@{
     //! 1 to distance
     Path
         drivingDistance(
                 G &graph,
                 int64_t start_vertex,
                 double distance);


     //! many to distance
     std::deque<Path> drivingDistance(
             G &graph,
             std::vector< int64_t > start_vertex,
             double distance,
             bool equiCostFlag);
     //@}

     //! @name Dijkstra
     //@{
     //! one to one
     Path dijkstra(
             G &graph,
             int64_t start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to one
     std::deque<Path> dijkstra(
             G &graph,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to Many
     std::deque<Path> dijkstra(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);

     //! one to Many
     std::deque<Path> dijkstra(
             G &graph,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);
     //@}

 private:
     //! Call to Dijkstra  1 source to 1 target
     bool dijkstra_1_to_1(
             G &graph,
             V source,
             V target);

     //! Call to Dijkstra  1 source to distance
     bool dijkstra_1_to_distance(
             G &graph,
             V source,
             double distance);

     //! Call to Dijkstra  1 source to many targets
     bool dijkstra_1_to_many(
             G &graph,
             V source,
             const std::vector< V > &targets);

     void clear() {
         predecessors.clear();
         distances.clear();
         nodesInDistance.clear();
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

     //! class for stopping when all targets are found
     class dijkstra_many_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_many_goal_visitor(std::vector< V > goals)
              :m_goals(goals.begin(), goals.end()) {}
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
#if 0
                  REG_SIGINT;
                  THROW_ON_SIGINT;
#endif
                  auto s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;
                  // we found one more goal
                  m_goals.erase(s_it);
                  if (m_goals.size() == 0) throw found_goals();
                  num_edges(g);
              }
      private:
          std::set< V > m_goals;
     };


     //! class for stopping when a distance/cost has being surpassed
     class dijkstra_distance_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_distance_visitor(
                  double distance_goal,
                  std::deque< V > &nodesInDistance,
                  std::vector< double > &distances) :
              m_distance_goal(distance_goal),
              m_nodes(nodesInDistance),
              m_dist(distances) {
              }
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
#if 0
                  REG_SIGINT;
                  THROW_ON_SIGINT;
#endif
                  m_nodes.push_back(u);
                  if (m_dist[u] >= m_distance_goal) throw found_goals();
                  num_edges(g);
              }
      private:
          double m_distance_goal;
          std::deque< V > &m_nodes;
          std::vector< double > &m_dist;
     };


     //@}
};


/******************** IMPLEMENTTION ******************/



// preparation for many to distance
template < class G >
std::deque< Path >
Pgr_dijkstra< G >::drivingDistance(
        G &graph,
        std::vector< int64_t > start_vertex,
        double distance,
        bool equicost) {
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());


    // perform the algorithm
    std::deque< Path > paths;
    for (const auto &vertex : start_vertex) {
        paths.push_back(
                drivingDistance(graph, vertex, distance));
    }
    if (equicost) {
        equi_cost(paths);
    }
    return paths;
}

template < class G >
Path
Pgr_dijkstra< G >::drivingDistance(
        G &graph,
        int64_t start_vertex,
        double distance) {
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

    // get source;
    if (!graph.has_vertex(start_vertex)) {
        /* The node has to be in the path*/
        Path p(start_vertex, start_vertex);
        p.push_back({start_vertex, -1, 0, 0});
        return p;
    }

    auto v_source(graph.get_V(start_vertex));;
    dijkstra_1_to_distance(graph, v_source, distance);

    auto path = Path(graph, v_source, distance, predecessors, distances);

    std::sort(path.begin(), path.end(),
            [](const Path_t &l, const  Path_t &r)
            {return l.node < r.node;});
    std::stable_sort(path.begin(), path.end(),
            [](const Path_t &l, const  Path_t &r)
            {return l.agg_cost < r.agg_cost;});
    return path;
}

//! Dijkstra 1 to 1
template < class G >
Path
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        int64_t start_vertex,
        int64_t end_vertex,
        bool only_cost) {
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

//! Dijkstra 1 to many
template < class G >
std::deque<Path>
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        int64_t start_vertex,
        const std::vector< int64_t > &end_vertex,
        bool only_cost) {
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
    dijkstra_1_to_many(graph, v_source, v_targets);

    std::deque< Path > paths;
    // get the results // route id are the targets
    paths = get_paths(graph, v_source, v_targets, only_cost);

    std::stable_sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.end_id() < e2.end_id();
            });

    return paths;
}

// preparation for many to 1
template < class G >
std::deque<Path>
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        const std::vector < int64_t > &start_vertex,
        int64_t end_vertex,
        bool only_cost) {
    std::deque<Path> paths;

    for (const auto &start : start_vertex) {
        paths.push_back(
                dijkstra(graph, start, end_vertex, only_cost));
    }

    std::stable_sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.start_id() < e2.start_id();
            });
    return paths;
}


// preparation for many to many
template < class G >
std::deque<Path>
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        const std::vector< int64_t > &start_vertex,
        const std::vector< int64_t > &end_vertex,
        bool only_cost) {
    // a call to 1 to many is faster for each of the sources
    std::deque<Path> paths;
    for (const auto &start : start_vertex) {
        auto r_paths = dijkstra(graph, start, end_vertex, only_cost);
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


//! Call to Dijkstra  1 source to 1 target
template < class G >
bool
Pgr_dijkstra< G >::dijkstra_1_to_1(
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


//! Call to Dijkstra  1 source to distance
template < class G >
bool
Pgr_dijkstra< G >::dijkstra_1_to_distance(G &graph, V source, double distance) {
    bool found = false;
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(dijkstra_distance_visitor(
                        distance,
                        nodesInDistance,
                        distances)));
    }
    catch(found_goals &) {
        found = true;
    } catch (...) {
    }
    return found;
}

//! Call to Dijkstra  1 source to many targets
template <class G>
bool
Pgr_dijkstra< G >::dijkstra_1_to_many(
        G &graph,
        V source,
        const std::vector< V > &targets) {
    bool found = false;
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(dijkstra_many_goal_visitor(targets)));
    }
    catch(found_goals &) {
        found = true;  // Target vertex found
    } catch (...) {
    }
    return found;
}

#endif  // INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
