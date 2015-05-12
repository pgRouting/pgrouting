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
      // the constructor
    explicit Pgr_dijkstra(graphType gtype, const int initial_size)
     :Pgr_base_graph<G>(gtype, initial_size) {}

    // visitor that terminates when we find the goal
    struct found_one_goal{};  // exception for termination

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


    // the actual call to Dijkstra
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


    /*  Perfom the inizialization of the graph
     This example only requieres the data insertion
     Any other initialization should go here
    */
    void
    initialize_graph(pgr_edge_t *data_edges, int64_t count) {
          this->graph_insert_data(data_edges, count);
    }


    void
    process_dijkstra(Path &path, int64_t start_vertex, int64_t end_vertex) {
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
           path.clear();
           return;
      }

      // perform the algorithm
      dijkstra_1_to_1(v_source, v_target);

      // get the results
      return this->get_path(path, v_source, v_target);
    }
};

#endif  // SRC_DIJKSTRA_SRC_PGR_DIJKSTRA_H_
