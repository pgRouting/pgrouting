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

#include <boost/config.hpp>
#include "postgres.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/baseGraph.hpp"


template < class G >
class Pgr_dijkstra
  :public Pgr_base_graph<G> {
 public:
    //! \brief the constructor
    explicit Pgr_dijkstra(graphType gtype, const int initial_size)
     :Pgr_base_graph<G>(gtype, initial_size) {}


    /*! \brief Perfom the inizialization of the graph
     For Dijkstra  only requieres the data insertion.

     Any other different initialization needs a different function.
     \param[in] data_edges
     \param[in] count
    */
    void
    initialize_graph(pgr_edge_t *data_edges, int64_t count) {
          this->graph_insert_data(data_edges, count);
    }

    void
    dijkstra(Path &path, int64_t start_vertex, int64_t end_vertex) {
      typedef typename boost::graph_traits < G >::vertex_descriptor V;

      // adjust predecessors and distances vectors
      this->predecessors.clear();
      this->distances.clear();

      this->predecessors.resize(boost::num_vertices(this->graph));
      this->distances.resize(boost::num_vertices(this->graph));

      // get the graphs source and target
      V v_source;
      V v_target;

      if (!this->get_gVertex(start_vertex, v_source)
          || !this->get_gVertex(end_vertex, v_target)) {
           // path.clear();
           return;
      }

      // perform the algorithm
      dijkstra_1_to_1(v_source, v_target);

      // get the results
      this->get_path(path, v_source, v_target, 0);
      return;
    }




    // preparation for 1 to many
    void
    dijkstra(std::deque< Path > &paths, int64_t start_vertex, std::vector< int64_t > end_vertex) {
      typedef typename boost::graph_traits < G >::vertex_descriptor V;

      // adjust predecessors and distances vectors
      this->predecessors.clear();
      this->distances.clear();

      this->predecessors.resize(boost::num_vertices(this->graph));
      this->distances.resize(boost::num_vertices(this->graph));

      // get the graphs source and target
      V v_source;
      if (!this->get_gVertex(start_vertex, v_source)) {
        // paths.clear();
        return;
      }
      
      std::set< V > v_targets;
      for (unsigned int i = 0; i < end_vertex.size(); i++) {
          V v_target;
          if (!this->get_gVertex(end_vertex[i], v_target)) {
            // paths.clear();
            return;
          }
          v_targets.insert(v_target);
      }

      // perform the algorithm
      dijkstra_1_to_many(v_source, v_targets);

      // get the results // route id are the targets
      this->get_path(paths, v_source, v_targets);
      return;
    }

    // preparation for many to 1
    void
    dijkstra(std::deque< Path > &paths, std::vector < int64_t > start_vertex, int64_t end_vertex) {
      typedef typename boost::graph_traits < G >::vertex_descriptor V;

      // adjust predecessors and distances vectors
      this->predecessors.clear();
      this->distances.clear();

      this->predecessors.resize(boost::num_vertices(this->graph));
      this->distances.resize(boost::num_vertices(this->graph));

      // get the graphs source and target
      V v_target;
      if (!this->get_gVertex(end_vertex, v_target)) {
        // paths.clear();
        return;
      }

      std::set< V > v_sources;
      for (unsigned int i = 0; i < start_vertex.size(); i++) {
          V v_source;
          if (!this->get_gVertex(start_vertex[i], v_source)) {
            // paths.clear();
            return;
          }
          v_sources.insert(v_source);
      }

      // perform the algorithm // a call for each of the sources
      for (const auto &v_source: v_sources) {
         Path path;
         dijkstra_1_to_1(v_source, v_target);
         this->get_path(path, v_source, v_target, this->graph[v_source].id);
         paths.push_back(path);
      }
      return;
    }

    // preparation for many to many
    void
    dijkstra(std::deque< Path > &paths, std::vector < int64_t > start_vertex, std::vector < int64_t > end_vertex) {
      // a call to 1 to many is faster for each of the sources
      for (const auto &start: start_vertex) {
         dijkstra(paths, start, end_vertex);
      }
      return;
    }



    // preparation for 1 to distance
    void
    dijkstra_dd(Path &path, int64_t start_vertex, float8 distance) {
      typedef typename boost::graph_traits < G >::vertex_descriptor V;

      // adjust predecessors and distances vectors
      this->predecessors.clear();
      this->distances.clear();
      this->nodesInDistance.clear();

      this->predecessors.resize(boost::num_vertices(this->graph));
      this->distances.resize(boost::num_vertices(this->graph));

      // get ource;
      V v_source;
      if (!this->get_gVertex(start_vertex, v_source)) {
        path.clear();
        return;
      }

      // perform the algorithm
      dijkstra_1_to_distance(v_source, distance);

      // get the results
      this->get_nodesInDistance(path, v_source, distance);
      return; 
    }


    // preparation for many to distance
    void
    dijkstra_dd(std::deque< Path > &paths, std::vector< int64_t > start_vertex, float8 distance) {
      typedef typename boost::graph_traits < G >::vertex_descriptor V;

      // adjust predecessors and distances vectors
      this->predecessors.clear();
      this->distances.clear();
      this->nodesInDistance.clear();

      this->predecessors.resize(boost::num_vertices(this->graph));
      this->distances.resize(boost::num_vertices(this->graph));

      // get the graph's source
      std::set< V > v_sources;
      for (unsigned int i = 0; i < start_vertex.size(); i++) {
          V v_source;
          if (!this->get_gVertex(start_vertex[i], v_source)) {
            paths.clear();
            return;
          }
          v_sources.insert(v_source);
      }

      // perform the algorithm
      for (const auto &v_source: v_sources) {
        dijkstra_1_to_distance(v_source, distance);
        Path path;
        this->get_nodesInDistance(path, v_source, distance);
        paths.push_back(path);
      }

      return;
    }



 private:
    //! visitor that terminates when we find the goal
    struct found_one_goal{};  //!< exception for termination

    //! class for stopping when 1 target is found
    template <class Vertex>
    class dijkstra_one_goal_visitor
      :public boost::default_dijkstra_visitor {
     public:
       explicit dijkstra_one_goal_visitor(Vertex goal) : m_goal(goal) {}
       template <class Graph>
       void examine_vertex(Vertex u, Graph& g) {
         if (u == m_goal) throw found_one_goal();
       }
     private:
       Vertex m_goal;
    };


    //! class for stopping when a distance/cost has being surpassed
    template <class Vertex>
    class dijkstra_distance_visitor
      :public boost::default_dijkstra_visitor {
     public:
       explicit dijkstra_distance_visitor(float8 goal,
               std::deque< Vertex > &nodesInDistance,
               std::vector< float8 > &distances)
         :m_goal(goal), m_nodes(nodesInDistance), m_dist(distances) {}
       template <class Graph>
       void examine_vertex(Vertex u, Graph& g) {
         m_nodes.push_back(u); 
         if (m_dist[u] >= m_goal) throw found_one_goal();
       }
     private:
       float8 m_goal;
       std::deque< Vertex > &m_nodes;
       std::vector< float8 > &m_dist;
    };

    //! visitor that terminates when we find  all goals
    struct found_all_goals{};  //!< exception for termination

    //! class for stopping when all targets are found
    template <class V>
    class dijkstra_many_goal_visitor
      :public boost::default_dijkstra_visitor {
     public:
       explicit dijkstra_many_goal_visitor(std::set< V > goals) : m_goals(goals) {}
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

    //! Call to Dijkstra  1 source to distance
    template <class V>
    bool
    dijkstra_1_to_distance(V source, float8 distance) {
      bool found = false;
      try {
      boost::dijkstra_shortest_paths(this->graph, source,
          boost::predecessor_map(&this->predecessors[0])
          .weight_map(get(&boost_edge_t::cost, this->graph))
          .distance_map(&this->distances[0])
          .visitor(dijkstra_distance_visitor< V >(
              distance, 
              this->nodesInDistance,
              this->distances)));
      }
      catch(found_one_goal &fg) {
        found = true;  // Target vertex found
      }
      return found;
    }
};

#endif  // SRC_DIJKSTRA_SRC_PGR_DIJKSTRA_H_
