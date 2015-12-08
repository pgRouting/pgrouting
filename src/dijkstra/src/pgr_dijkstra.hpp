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

#ifndef SRC_DIJKSTRA_SRC_PGR_DIJKSTRA_H_
#define SRC_DIJKSTRA_SRC_PGR_DIJKSTRA_H_


#include <deque>
#include <vector>
#include <set>

#include <boost/config.hpp>


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

extern "C" {
#include "postgres.h"
}

#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/baseGraph.hpp"



template < class G > class Pgr_dijkstra;
// user's functions
// for development

template < class G >
void pgr_drivingDistance(
        G &graph,
        std::deque< Path > &paths,
        std::vector< int64_t > start_vids,
        double distance) {
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.drivingDistance(graph, paths, start_vids, distance);
}

template < class G >
void
pgr_drivingDistance(
        G &graph,
        Path &path,
        int64_t  source,
        double distance) {
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.drivingDistance(graph, path, source, distance);
}

template < class G >
void
pgr_dijkstra(
        G &graph,
        Path &path,
        int64_t source,
        int64_t target,
        bool only_cost = false) {
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.dijkstra(graph, path, source, target, only_cost);
}

template < class G >
void
pgr_dijkstra(
        G &graph,
        std::deque<Path> &paths,
        int64_t  source,
        const std::vector< int64_t > &targets,
        bool only_cost = false) {
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.dijkstra(graph, paths, source, targets, only_cost);
}

template < class G >
void
pgr_dijkstra(G &graph,
        std::deque<Path> &paths,
        const std::vector< int64_t > &sources,
        int64_t  target,
        bool only_cost = false) {
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.dijkstra(graph, paths, sources, target, only_cost);
}

template < class G >
void
pgr_dijkstra(
        G &graph, std::deque<Path> &paths,
        const std::vector< int64_t > sources,
        const std::vector< int64_t > &targets,
        bool only_cost = false) {
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.dijkstra(graph, paths, sources, targets, only_cost);
}



//******************************************

template < class G >
class Pgr_dijkstra {
 public:
     typedef typename G::V V;

     //! @name drivingDistance
     //@{
     //! 1 to distance
     void drivingDistance(
             G &graph,
             Path &path,
             int64_t start_vertex,
             float8 distance);


     //! many to distance
     void drivingDistance(
             G &graph, std::deque< Path > &paths,
             std::vector< int64_t > start_vertex,
             float8 distance);
     //@}

     //! @name Dijkstra
     //@{
     //! one to one
     void dijkstra(
             G &graph,
             Path &path,
             int64_t start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to one
     void dijkstra(
             G &graph,
             std::deque< Path > &paths,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to Many
     void dijkstra(
             G &graph,
             std::deque< Path > &paths,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);

     //! one to Many
     void dijkstra(
             G &graph,
             std::deque< Path > &paths,
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
             float8 distance);

     //! Call to Dijkstra  1 source to many targets
     bool dijkstra_1_to_many(
             G &graph,
             V source,
             const std::set< V > &targets);

     void get_nodesInDistance(
             G &graph,
             Path &path,
             V source,
             float8 distance);

     void clear() {
         predecessors.clear();
         distances.clear();
         nodesInDistance.clear();
     }


     void get_path(
             const G &graph,
             V source,
             V target,
             Path &path) const;
     void get_cost(
             const G &graph,
             V source,
             V target,
             Path &path) const;


     // used when multiple goals
     void get_path(
             const G &graph,
             std::deque< Path > &paths,
             V source,
             std::set< V > &targets) const;

     void get_cost(
             const G &graph,
             std::deque< Path > &paths,
             V source,
             std::set< V > &targets) const;


     //! @name Variables
     //@{
     struct found_goals{};  //!< exception for termination
     std::vector< V > predecessors;
     std::vector< float8 > distances;
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
                  if (u == m_goal) throw found_goals();
              }
      private:
          V m_goal;
     };

     //! class for stopping when all targets are found
     class dijkstra_many_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_many_goal_visitor(std::set< V > goals)
              :m_goals(goals) {}
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
                  typename std::set< V >::iterator s_it;
                  s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;
                  // we found one more goal
                  m_goals.erase(s_it);
                  if (m_goals.size() == 0) throw found_goals();
              }
      private:
          std::set< V > m_goals;
     };


     //! class for stopping when a distance/cost has being surpassed
     class dijkstra_distance_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_distance_visitor(float8 distance_goal,
                  std::deque< V > &nodesInDistance,
                  std::vector< float8 > &distances) :
              m_distance_goal(distance_goal),
              m_nodes(nodesInDistance),
              m_dist(distances) {}
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
                  m_nodes.push_back(u);
                  if (m_dist[u] >= m_distance_goal) throw found_goals();
              }
      private:
          float8 m_distance_goal;
          std::deque< V > &m_nodes;
          std::vector< float8 > &m_dist;
     };


     //@}
};


/******************** IMPLEMENTTION ******************/


template < class G >
void
Pgr_dijkstra< G >::get_nodesInDistance(
        G &graph,
        Path &path,
        V source,
        float8 distance) {
    path.clear();
    int seq = 0;
    float8 cost;
    int64_t edge_id;
    for (V i = 0; i < distances.size(); ++i) {
        if (distances[i] <= distance) {
            cost = distances[i] - distances[predecessors[i]];
            edge_id = graph.get_edge_id(predecessors[i], i, cost);
            path.push_back(
                    seq,
                    graph[source].id,
                    graph[source].id,
                    graph[i].id,
                    edge_id, cost,
                    distances[i]);
            seq++;
        }
    }
}


template < class G >
void
Pgr_dijkstra< G >::get_path(
        const G &graph,
        std::deque< Path > &paths,
        V source,
        std::set< V > &targets) const {
    Path path;
    typename std::set< V >::iterator s_it;
    for (s_it = targets.begin(); s_it != targets.end(); ++s_it) {
        path.clear();
        get_path(graph, source, *s_it, path);
        paths.push_back(path);
    }
}

template < class G >
void
Pgr_dijkstra< G >::get_path(
        const G &graph,
        V source,
        V target,
        Path &path) const {
    // backup of the target
    V target_back = target;
    uint64_t from(graph.graph[source].id);
    uint64_t to(graph.graph[target].id);

    // no path was found
    if (target == predecessors[target]) {
        path.clear();
        return;
    }

    // findout how large is the path
    int64_t result_size = 1;
    while (target != source) {
        if (target == predecessors[target]) break;
        result_size++;
        target = predecessors[target];
    }

    // recover the target
    target = target_back;

    // variables that are going to be stored
    int64_t vertex_id;
    int64_t edge_id;
    float8 cost;

    // working from the last to the beginning

    // initialize the sequence
    int seq = result_size;
    // the last stop is the target
    path.push_front(
            seq,
            from, to,
            graph.graph[target].id, -1,
            0,  distances[target]);

    while (target != source) {
        // we are done when the predecesor of the target is the target
        if (target == predecessors[target]) break;
        // values to be inserted in the path
        --seq;
        cost = distances[target] - distances[predecessors[target]];
        vertex_id = graph.graph[predecessors[target]].id;
        edge_id = graph.get_edge_id(predecessors[target], target, cost);

        path.push_front(
                seq,
                from, to,
                vertex_id, edge_id,
                cost, distances[target] - cost);
        target = predecessors[target];
    }
    return;
}

template < class G >
void
Pgr_dijkstra< G >::get_cost(
        const G &graph,
        std::deque< Path > &paths,
        V source,
        std::set< V > &targets) const {
    Path path;
    typename std::set< V >::iterator s_it;
    for (s_it = targets.begin(); s_it != targets.end(); ++s_it) {
        path.clear();
        get_cost(graph, source, *s_it, path);
        paths.push_back(path);
    }
}


template < class G >
void
Pgr_dijkstra< G >::get_cost(
        const G &graph,
        V source,
        V target,
        Path &path) const {
    // backup of the target
    uint64_t from(graph.graph[source].id);
    uint64_t to(graph.graph[target].id);

    // no path was found
    if (target == predecessors[target]) {
        if (from == to) {
            path.push_front(1, from, to, graph.graph[target].id, -1, 0, 0);
        } else {
            path.clear();
        }
    } else {
        path.push_front(
                1,
                from, to,
                graph[target].id,
                -1,
                0,  distances[target]);
    }
}

template < class G >
// preparation for many to distance
void
Pgr_dijkstra< G >::drivingDistance(G &graph, std::deque< Path > &paths,
        std::vector< int64_t > start_vertex,
        float8 distance) {
    // adjust predecessors and distances vectors
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

    // get the graph's source
    std::set< V > v_sources;
    for (unsigned int i = 0; i < start_vertex.size(); i++) {
        V v_source;
        if (!graph.get_gVertex(start_vertex[i], v_source)) {
            paths.clear();
            return;
        }
        v_sources.insert(v_source);
    }

    // perform the algorithm
    for (const auto &v_source : v_sources) {
        dijkstra_1_to_distance(graph, v_source, distance);
        Path path;
        get_nodesInDistance(graph, path, v_source, distance);
        paths.push_back(path);
    }

    return;
}

template < class G >
void
Pgr_dijkstra< G >::drivingDistance(
        G &graph,
        Path &path,
        int64_t start_vertex,
        float8 distance) {
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

    // get source;
    V v_source;
    if (!graph.get_gVertex(start_vertex, v_source)) {
        path.clear();
        return;
    }

    // perform the algorithm
    if (dijkstra_1_to_distance(graph, v_source, distance)) {
        // get the results
        get_nodesInDistance(graph, path, v_source, distance);
    } else {
        path.clear();
    }
    return;
}

template < class G >
void
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        Path &path,
        int64_t start_vertex,
        int64_t end_vertex,
        bool only_cost) {
    clear();

    // adjust predecessors and distances vectors
    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

    // get the graphs source and target
    V v_source;
    V v_target;

    if (!graph.get_gVertex(start_vertex, v_source)
            || !graph.get_gVertex(end_vertex, v_target)) {
        path.clear();
        return;
    }

    // perform the algorithm
    dijkstra_1_to_1(graph, v_source, v_target);

    // get the results
    if (only_cost) {
        get_cost(graph, v_source, v_target, path);
    } else {
        get_path(graph, v_source, v_target, path);
    }
    return;
}
//! Dijkstra 1 to many
template < class G >
void
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        std::deque< Path > &paths,
        int64_t start_vertex,
        const std::vector< int64_t > &end_vertex,
        bool only_cost) {
    // adjust predecessors and distances vectors
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

    // get the graphs source and target
    V v_source;
    if (!graph.get_gVertex(start_vertex, v_source)) {
        // paths.clear();
        return;
    }

    std::set< V > v_targets;
    for (unsigned int i = 0; i < end_vertex.size(); i++) {
        V v_target;
        if (graph.get_gVertex(end_vertex[i], v_target)) {
            v_targets.insert(v_target);
        }
    }

    // perform the algorithm
    dijkstra_1_to_many(graph, v_source, v_targets);

    // get the results // route id are the targets
    if (only_cost) {
        get_cost(graph, paths, v_source, v_targets);
    } else {
        get_path(graph, paths, v_source, v_targets);
    }
    return;
}

// preparation for many to 1
template < class G >
void
Pgr_dijkstra< G >::dijkstra(
        G &graph, std::deque< Path > &paths,
        const std::vector < int64_t > &start_vertex,
        int64_t end_vertex,
        bool only_cost) {
    // perform the algorithm // a call for each of the sources
    for (const auto &start : start_vertex) {
        Path path;
        // each call cleans the local structures
        dijkstra(graph, path, start, end_vertex, only_cost);
        paths.push_back(path);
    }
    return;
}


// preparation for many to many
template < class G >
void
Pgr_dijkstra< G >::dijkstra(
        G &graph, std::deque< Path > &paths,
        const std::vector< int64_t > &start_vertex,
        const std::vector< int64_t > &end_vertex,
        bool only_cost) {
    // a call to 1 to many is faster for each of the sources
    for (const auto &start : start_vertex) {
        dijkstra(graph, paths, start, end_vertex, only_cost);
    }
    return;
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
                .weight_map(get(&boost_edge_t::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(dijkstra_one_goal_visitor(target)));
    }
    catch(found_goals &fg) {
        found = true;  // Target vertex found
    }
    return found;
}


//! Call to Dijkstra  1 source to distance
template < class G >
bool
Pgr_dijkstra< G >::dijkstra_1_to_distance(G &graph, V source, float8 distance) {
    bool found = false;
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&boost_edge_t::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(dijkstra_distance_visitor(
                        distance,
                        nodesInDistance,
                        distances)));
    }
    catch(found_goals &fg) {
        found = true;
    }
    return found;
}

//! Call to Dijkstra  1 source to many targets
template <class G>
bool
Pgr_dijkstra< G >::dijkstra_1_to_many(
        G &graph,
        V source,
        const std::set< V > &targets) {
    bool found = false;
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&boost_edge_t::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(dijkstra_many_goal_visitor(targets)));
    }
    catch(found_goals &fg) {
        found = true;  // Target vertex found
    }
    return found;
}

#endif  // SRC_DIJKSTRA_SRC_PGR_DIJKSTRA_H_
