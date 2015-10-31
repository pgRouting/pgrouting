/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

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
void
pgr_dijkstra(G &graph, Path &path,  int64_t  source, int64_t target) {
     Pgr_dijkstra< G > fn_dijkstra;
     fn_dijkstra.dijkstra(graph, path, source, target);
}

template < class G >
void
pgr_dijkstra(G &graph, std::deque<Path> &paths,  int64_t  source, const std::vector< int64_t > &targets) {
     Pgr_dijkstra< G > fn_dijkstra;
     fn_dijkstra.dijkstra(graph, paths, source, targets);
}

template < class G >
void
pgr_dijkstra(G &graph, std::deque<Path> &paths,  const std::vector< int64_t > &sources, int64_t  target) {
     Pgr_dijkstra< G > fn_dijkstra;
     fn_dijkstra.dijkstra(graph, paths, sources, target);
}

template < class G >
void
pgr_dijkstra(G &graph, std::deque<Path> &paths,  const std::vector< int64_t > sources, const std::vector< int64_t > &targets) {
     Pgr_dijkstra< G > fn_dijkstra;
     fn_dijkstra.dijkstra(graph, paths, sources, targets);
}


// for postgres not working 
#if 0
template < class G >
void
pgr_dijkstra(G &graph, int64_t  source, int64_t target, size_t &result_tuple_count, General_path_element_t **postgres_rows) {
     Pgr_dijkstra< G > fn_dijkstra;
     fn_dijkstra.dijkstra(graph, source, target, result_tuple_count, postgres_rows);
     if ((*postgres_rows) == NULL)      std::cout << "result_tuple_count" << result_tuple_count <<"\n";

}
#endif
//******************************************

template < class G >
class Pgr_dijkstra {
 // :public Pgr_base_graph<G> {
 public:
    // CONSTRUCTORS
    // default constructors

    typedef typename G::V V;
    struct found_goals{};  //!< exception for termination

    //! @name Dijkstra 1 to 1
    //@{


    //! class for stopping when 1 target is found
    template <class V>
    class dijkstra_one_goal_visitor
      :public boost::default_dijkstra_visitor {
     public:
       explicit dijkstra_one_goal_visitor(V goal) : m_goal(goal) {}
       template <class B_G>
       void examine_vertex(V &u, B_G &g) {
         if (u == m_goal) throw found_goals();
       }
     private:
       V m_goal;
    };

    // preparation for 1 to 1
    // G: is either:
    // Pgr_base_graph< DirectedGraph >
    // Pgr_base_graph< UndirectedGraph >
    // reusing allways clears
 public:
    void
    //dijkstra(Path &path, int64_t start_vertex, int64_t end_vertex) {
    dijkstra(G &graph, Path &path, int64_t start_vertex, int64_t end_vertex) {
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
      dijkstra_1_to_1(graph.graph, v_source, v_target);

      // get the results
      get_path(graph, v_source, v_target, path);
      return;
    }


    //! Call to Dijkstra  1 source to 1 target
    template <class B_G, class V>
    bool
    dijkstra_1_to_1(
      B_G &graph, V source, V target
      ) {

      bool found = false;
      try {
      boost::dijkstra_shortest_paths(graph, source,
          boost::predecessor_map(&predecessors[0])
          .weight_map(get(&boost_edge_t::cost, graph))
          .distance_map(&distances[0])
          .visitor(dijkstra_one_goal_visitor<V>(target)));
      }
      catch(found_goals &fg) {
        found = true;  // Target vertex found
      }
      return found;
    }
    //@}



    //! @name Dijkstra 1 to many
    //@{
    void
    dijkstra(G &graph, std::deque< Path > &paths,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex) {

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
      dijkstra_1_to_many(graph.graph, v_source, v_targets);

      // get the results // route id are the targets
      get_path(graph, paths, v_source, v_targets);
      return;
    }

    //! Call to Dijkstra  1 source to many targets
    template <class B_G, class V>
    bool
    dijkstra_1_to_many(B_G &graph, V source, const std::set< V > &targets) {
      bool found = false;
      try {
      boost::dijkstra_shortest_paths(graph, source,
          boost::predecessor_map(&predecessors[0])
          .weight_map(get(&boost_edge_t::cost, graph))
          .distance_map(&distances[0])
          .visitor(dijkstra_many_goal_visitor< V >(targets)));
      }
      catch(found_goals &fg) {
        found = true;  // Target vertex found
      }
      return found;
    }

    //! class for stopping when all targets are found
    template <class V>
    class dijkstra_many_goal_visitor
      :public boost::default_dijkstra_visitor {
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

    //@}


    //! @name Dijkstra many to 1
    //@{
    void
    // preparation for many to 1
    dijkstra(G &graph, std::deque< Path > &paths,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex) {
      // perform the algorithm // a call for each of the sources
      for (const auto &start : start_vertex) {
         Path path;
         // each call cleans the local structures
         dijkstra(graph, path, start, end_vertex);
         paths.push_back(path);
      }
      return;
    }


    // preparation for many to many
    void
    dijkstra(G &graph, std::deque< Path > &paths,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex) {
      // a call to 1 to many is faster for each of the sources
      for (const auto &start : start_vertex) {
         dijkstra(graph, paths, start, end_vertex);
      }
      return;
    }
    //@}

    //! @name drivingDistance many to 1
    //@{
    // preparation for 1 to distance
    void
    dijkstra_dd(G &graph, Path &path, int64_t start_vertex, float8 distance) {
      clear();

      // adjust predecessors and distances vectors
      predecessors.resize(graph.num_vertices());
      distances.resize(graph.num_vertices());

      // get source;
      V v_source;
      if (!graph.get_gVertex(start_vertex, v_source)) {
        path.clear();
        return;
      }

      // perform the algorithm
      dijkstra_1_to_distance(graph.graph, v_source, distance);

      // get the results
      get_nodesInDistance(graph, path, v_source, distance);
      return;
    }


    // preparation for many to distance
    void
    dijkstra_dd(G &graph, std::deque< Path > &paths,
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
        dijkstra_1_to_distance(graph.graph, v_source, distance);
        Path path;
        get_nodesInDistance(graph, path, v_source, distance);
        paths.push_back(path);
      }

      return;
    }


 private:
    //! class for stopping when a distance/cost has being surpassed
    template <class Vertex>
    class dijkstra_distance_visitor
      :public boost::default_dijkstra_visitor {
     public:
       explicit dijkstra_distance_visitor(float8 goal,
               std::deque< Vertex > &nodesInDistance,
               std::vector< float8 > &distances)
         :m_goal(goal), m_nodes(nodesInDistance), m_dist(distances) {}
       template <class B_G>
       void examine_vertex(V u, B_G &g) {
         m_nodes.push_back(u);
         if (m_dist[u] >= m_goal) throw found_goals();
       }
     private:
       float8 m_goal;
       std::deque< Vertex > &m_nodes;
       std::vector< float8 > &m_dist;
    };

#if 0
    //! visitor that terminates when we find  all goals
    struct found_all_goals{};  //!< exception for termination

    //! class for stopping when all targets are found
    template <class V>
    class dijkstra_many_goal_visitor
      :public boost::default_dijkstra_visitor {
     public:
       explicit dijkstra_many_goal_visitor(std::set< V > goals)
                :m_goals(goals) {}
       template <class Graph>
       void examine_vertex(V u, Graph &g) {
         typename std::set< V >::iterator s_it;
         s_it = m_goals.find(u);
         if (s_it == m_goals.end()) return;
         // we found one more goal
         m_goals.erase(s_it);
         if (m_goals.size() == 0) throw found_all_goals();
       }
     private:
       std::set< V > m_goals;
    };



    //! Call to Dijkstra  1 source to many targets
    template <class V>
    bool
    dijkstra_1_to_many(V source, std::set< V > targets) {
      bool found = false;
      try {
      boost::dijkstra_shortest_paths(this->graph, source,
          boost::predecessor_map(&this->predecessors[0])
          .weight_map(get(&boost_edge_t::cost, this->graph))
          .distance_map(&this->distances[0])
          .visitor(dijkstra_many_goal_visitor< V >(targets)));
      }
      catch(found_all_goals &fg) {
        found = true;  // Target vertex found
      }
      return found;
    }

    //! Call to Dijkstra  1 source to 1 target
    template <class V>
    bool
    dijkstra_1_to_1(V source, V target) {
      bool found = false;
      try {
      boost::dijkstra_shortest_paths(this->graph, source,
          boost::predecessor_map(&this->predecessors[0])
          .weight_map(get(&boost_edge_t::cost, this->graph))
          .distance_map(&this->distances[0])
          .visitor(dijkstra_one_goal_visitor<V>(target)));
      }
      catch(found_one_goal &fg) {
        found = true;  // Target vertex found
      }
      return found;
    }
#endif

    //! Call to Dijkstra  1 source to distance
    template <class B_G, class V>
    bool
    dijkstra_1_to_distance(B_G &graph, V source, float8 distance) {
      bool found = false;
      try {
      boost::dijkstra_shortest_paths(graph, source,
          boost::predecessor_map(&predecessors[0])
          .weight_map(get(&boost_edge_t::cost, graph))
          .distance_map(&distances[0])
          .visitor(dijkstra_distance_visitor< V >(
              distance,
              nodesInDistance,
              distances)));
      }
      catch(found_goals &fg) {
        found = true;  // Target vertex found
      }
      return found;
    }


 //private:
  //! @name Used by dijkstra
  //@{
  std::vector<typename G::V> predecessors;
  std::vector<float8> distances;
  std::deque<typename G::V> nodesInDistance;
  //@}

#if 0
    template <class V>
    void get_path(std::deque< Path > &paths, std::set< V > sources, V &target) const{
      // used with multiple sources
      Path path;
      for (const auto source: sources) {
        path.clear();
        get_path(path, source, target);
        paths.push_back(path);
      }
    }
#endif

private:
   void get_nodesInDistance(G &graph, Path &path, V source, float8 distance) {
      path.clear();
      int seq = 0;
      float8 cost;
      int64_t edge_id;
      for (V i = 0; i < distances.size(); ++i) {
        if (distances[i] <= distance ) {
          cost = distances[i] - distances[predecessors[i]];
          edge_id = graph.get_edge_id(predecessors[i], i, cost);
          path.push_back(seq, graph.graph[source].id, graph.graph[source].id, graph.graph[i].id, edge_id, cost, distances[i]);
          seq++;
        }
      }
    }


 private:
    template <class V>
    void get_path(const G &graph, std::deque< Path > &paths, V source, std::set< V > &targets) const{
      // used when multiple goals
      Path path;
      typename std::set< V >::iterator s_it;
      for (s_it = targets.begin(); s_it != targets.end(); ++s_it) {
        path.clear();
        get_path(graph, source, *s_it, path);
        paths.push_back(path);
      }
    }

 public:
    void clear() {
      predecessors.clear();
      distances.clear();
      nodesInDistance.clear();
    }

 private:
    template <class V>
    void get_path(
      const G &graph, V source, V target,
      Path &path) const{

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
      path.push_front(seq, from, to, graph.graph[target].id, -1, 0,  distances[target]);

      while (target != source) {
        // we are done when the predecesor of the target is the target
        if (target == predecessors[target]) break;
          // values to be inserted in the path
          --seq;
          cost = distances[target] - distances[predecessors[target]];
          vertex_id = graph.graph[predecessors[target]].id;
          edge_id = graph.get_edge_id(predecessors[target], target, cost);

          path.push_front(seq, from, to, vertex_id, edge_id, cost, distances[target] - cost);
          target = predecessors[target];
      }
      return;
    }

};

#endif  // SRC_DIJKSTRA_SRC_PGR_DIJKSTRA_H_
