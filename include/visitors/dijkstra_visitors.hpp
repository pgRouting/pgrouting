/*PGR-GNU*****************************************************************
file: dijkstra_one_goal_visitor.hpp

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
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

#ifndef INCLUDE_VISITORS_DIJKSTRA_ONE_GOAL_VISITOR_HPP_
#define INCLUDE_VISITORS_DIJKSTRA_ONE_GOAL_VISITOR_HPP_

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "visitors/found_goals.hpp"

namespace pgrouting {
namespace visitors {

template <typename V>
class dijkstra_one_goal_visitor : public boost::default_dijkstra_visitor {
 public:
     explicit dijkstra_one_goal_visitor(V goal) : m_goal(goal) {}
     template <class B_G>
         void examine_vertex(V &u, B_G &) {
             if (u == m_goal) throw found_goals();
         }
 private:
     V m_goal;
};

template <typename V>
class dijkstra_distance_visitor : public boost::default_dijkstra_visitor {
 public:
     explicit dijkstra_distance_visitor(
             double distance_goal,
             std::deque<V> &nodesInDistance,
             std::vector<double> &distances) :
         m_distance_goal(distance_goal),
         m_nodes(nodesInDistance),
         m_dist(distances) {
             pgassert(m_nodes.empty());
             pgassert(m_distance_goal > 0);
         }
     template <class B_G>
         void examine_vertex(V u, B_G &) {
             if (m_dist[u] > m_distance_goal) {
                 throw found_goals();
             }
             m_nodes.push_back(u);
         }

 private:
     double m_distance_goal;
     std::deque<V> &m_nodes;
     std::vector<double> &m_dist;
};

template <typename V>
class dijkstra_distance_visitor_no_init : public boost::default_dijkstra_visitor {
 public:
     explicit dijkstra_distance_visitor_no_init(
             std::ostringstream &p_log,
             V source,
             double distance_goal,
             std::vector<V> &predecessors,
             std::vector<double> &distances,
             std::vector<boost::default_color_type> &color_map) :
         log(p_log),
         first(source),
         m_distance_goal(distance_goal),
         m_num_examined(0),
         m_predecessors(predecessors),
         m_dist(distances),
         m_color(color_map) {
             pgassert(m_num_examined == 0);
             pgassert(m_distance_goal > 0);
         }

     template <class B_G>
         void examine_vertex(V u, B_G &) {
             if ( 0 == m_num_examined++) first = u;
             if (m_dist[u] > m_distance_goal) {
                 throw found_goals();
             }
             if (u != first && m_predecessors[u] == u) {
                 m_color[u] = boost::black_color;
             }
         }

     template <class B_G>
         void examine_edge(E e, B_G &g) {
             if (source(e, g) != first
                     && m_predecessors[source(e, g)] == source(e, g)) {
                 m_color[target(e, g)] = boost::black_color;
             }
         }


     template <class B_G>
         void edge_relaxed(E, B_G &) {
         }

     template <class B_G>
         void edge_not_relaxed(E e, B_G &g) {
             if (source(e, g) != first
                     && m_predecessors[source(e, g)] == source(e, g)) {
                 m_color[target(e, g)] = boost::black_color;
             }
         }

     template <class B_G>
         void finish_vertex(V, B_G &) {
         }

     template <class B_G>
         void discover_vertex(V u, B_G &) {
             if (u  != first && m_predecessors[u] == u) {
                 m_color[u] = boost::black_color;
             }
         }

 private:
     std::ostringstream &log;
     V first;
     double m_distance_goal;
     size_t m_num_examined;
     std::vector<V > &m_predecessors;
     std::vector<double> &m_dist;
     std::vector<boost::default_color_type> &m_color;
};


}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DIJKSTRA_ONE_GOAL_VISITOR_HPP_
