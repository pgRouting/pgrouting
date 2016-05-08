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

// Include C header first for windows build issue

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <cmath>
#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>

#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/pgr_base_graph.hpp"




template < class G >
class Pgr_astar {
 public:

     typedef typename G::V V;
     typedef typename G::B_G B_G;


     //! @name Astar
     //@{
     //! one to one
     void astar(
             G &graph,
             Path &path,
             int64_t start_vertex,
             int64_t end_vertex,
             int heuristic,
             double factor,
             double epsilon,
             bool only_cost = false);
#if 0
     //! Many to one
     void astar(
             G &graph,
             std::deque< Path > &paths,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to Many
     void astar(
             G &graph,
             std::deque< Path > &paths,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);

     //! one to Many
     void astar(
             G &graph,
             std::deque< Path > &paths,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);
     //@}
#endif

  private:
     //! Call to Astar  1 source to 1 target
     bool astar_1_to_1(
             G &graph,
             V source,
             V target,
             int heuristic,
             double factor,
             double epsilon);


     void clear() {
         predecessors.clear();
         distances.clear();
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



     //! @name members;
     //@{
     struct found_goals{};  //!< exception for termination
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::deque< V > nodesInDistance;
     //@}

     // euclidean distance heuristic for one goal
     class distance_heuristic : public boost::astar_heuristic< B_G, double > {
      public:
          distance_heuristic(B_G &g, V goal, int heuristic, double factor)
              : m_g(g),
                m_goal(goal),
                m_factor(factor),
                m_heuristic(heuristic) {}
          double operator()(V u) {
              double dx = m_g[m_goal].x() - m_g[u].x();
              double dy = m_g[m_goal].y() - m_g[u].y();
              switch (m_heuristic) {
                  case 0:
                      // == pgr_dijkstra ??
                      return 0;
                  case 1:
                      return std::fabs(std::max(dx, dy)) * m_factor;
                  case 2:
                      return std::fabs(std::min(dx, dy)) * m_factor;
                  case 3:
                      return (dx * dx + dy * dy) * m_factor * m_factor;
                  case 4:
                      return std::sqrt(dx * dx + dy * dy) * m_factor;
                  case 5:
                      return (std::fabs(dx) + std::fabs(dy)) * m_factor;
                  default:
                      return 0;
              }
          };

      private:
          B_G &m_g;
          V m_goal;
          double m_factor;
          int m_heuristic;
     }; // class distance_heuristic


     //! visitor that terminates when we find the goal
     class astar_one_goal_visitor : public boost::default_astar_visitor {
         public:
             astar_one_goal_visitor(V goal) : m_goal(goal) {}
             template <class B_G>
                 void examine_vertex(V u, B_G &g) {
                     if(u == m_goal)
                         throw found_goals();
                     // using g, otherwise is throws a warning
                     num_edges(g); 
                 }
         private:
             V m_goal;
     }; // class astar_one_goal_visitor

}; //pgr_astar

/******************** IMPLEMENTTION ******************/

//! astar 1 to 1
template < class G >
void
Pgr_astar< G >::astar(
        G &graph,
        Path &path,
        int64_t start_vertex,
        int64_t end_vertex,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost) {
    clear();

    // adjust predecessors and distances vectors
    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());


    if (!graph.has_vertex(start_vertex)
            || !graph.has_vertex(end_vertex)) {
        path.clear();
        return;
    }

    // get the graphs source and target
    auto v_source(graph.get_V(start_vertex));
    auto v_target(graph.get_V(end_vertex));

    // perform the algorithm
    astar_1_to_1(graph, v_source, v_target, heuristic, factor, epsilon);

    // get the results
    if (only_cost) {
        get_cost(graph, v_source, v_target, path);
    } else {
        get_path(graph, v_source, v_target, path);
    }
    return;
}


//! Call to Astar  1 source to 1 target
template < class G >
bool
Pgr_astar< G >::astar_1_to_1(
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
                distance_heuristic(graph.graph, target, heuristic, factor * epsilon),
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&pgRouting::Basic_edge::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(astar_one_goal_visitor(target)));
    }
    catch(found_goals &fg) {
        found = true;  // Target vertex found
    }
    return found;
}


template < class G >
void
Pgr_astar< G >::get_cost(
        const G &graph,
        V source,
        V target,
        Path &r_path) const {
    // backup of the target
    int64_t from(graph.graph[source].id);
    int64_t to(graph.graph[target].id);

    // no path was found
    if (target == predecessors[target]) {
        r_path.clear();
    } else {
        Path path(from, to);
        path.push_front(
                {to, -1, distances[target], distances[target]});
        r_path = path;
    }
}

template < class G >
void
Pgr_astar< G >::get_path(
        const G &graph,
        V source,
        V target,
        Path &r_path) const {
    // backup of the target
    V target_back = target;
    uint64_t from(graph.graph[source].id);
    uint64_t to(graph.graph[target].id);

    // no path was found
    if (target == predecessors[target]) {
        r_path.clear();
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
    double cost;

    // working from the last to the beginning

    // initialize the sequence
    auto seq = result_size;
    // the last stop is the target
    Path path(from, to);
    path.push_front(
            {graph.graph[target].id, -1,
            0,  distances[target]});

    while (target != source) {
        // we are done when the predecesor of the target is the target
        if (target == predecessors[target]) break;
        // values to be inserted in the path
        --seq;
        cost = distances[target] - distances[predecessors[target]];
        vertex_id = graph.graph[predecessors[target]].id;
        edge_id = graph.get_edge_id(predecessors[target], target, cost);

        path.push_front({vertex_id, edge_id,
                cost, (distances[target] - cost)});
        target = predecessors[target];
    }
    r_path = path;
    return;
}


