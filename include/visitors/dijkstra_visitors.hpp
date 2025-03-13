/*PGR-GNU*****************************************************************
File: dijkstra_visitors.hpp

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2018 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#ifndef INCLUDE_VISITORS_DIJKSTRA_VISITORS_HPP_
#define INCLUDE_VISITORS_DIJKSTRA_VISITORS_HPP_

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <deque>
#include <set>
#include <vector>
#include "visitors/found_goals.hpp"
#include "cpp_common/assert.hpp"

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
class dijkstra_max_distance_visitor: public boost::default_dijkstra_visitor {
 public:
    explicit dijkstra_max_distance_visitor(
            double distance_goal,
            std::vector<double> &distances,
            std::set<int64_t> &reached_vertices_ids,
            std::ostringstream &log):
        m_distance_goal(distance_goal),
        m_dist(distances),
        m_reached_vertices_ids(reached_vertices_ids),
        m_log(log) {
            pgassert(m_distance_goal > 0);
        }
        template <class B_G>
        void examine_vertex(V u, B_G &g) {
            if (m_dist[u] > m_distance_goal) {
                throw max_dist_reached();
            }
            m_reached_vertices_ids.insert(g[u].id);
        }

 private:
    double m_distance_goal;
    std::vector<double> &m_dist;
    std::set<int64_t> &m_reached_vertices_ids;
    std::ostringstream &m_log;
};

template <typename V>
class dijkstra_many_goal_visitor : public boost::default_dijkstra_visitor {
 public:
     dijkstra_many_goal_visitor(
             const std::set<V> &goals,
             size_t n_goals,
             std::set<V> &f_goals) :
         m_goals(goals),
         m_n_goals(n_goals),
         m_found_goals(f_goals)   {
         }
     template <class B_G>
         void examine_vertex(V u, B_G &) {
             auto s_it = m_goals.find(u);

             /* examined vertex is not a goal */
             if (s_it == m_goals.end()) return;

             // found one more goal
             m_found_goals.insert(*s_it);
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
     std::set<V> &m_found_goals;
};

template <typename V>
class dijkstra_distance_visitor : public boost::default_dijkstra_visitor {
 public:
     explicit dijkstra_distance_visitor(
             double distance_goal,
             std::vector<double> &distances) :
         m_distance_goal(distance_goal),
         m_dist(distances) {
             pgassert(m_distance_goal > 0);
         }
     template <class B_G>
         void examine_vertex(V u, B_G &) {
             if (m_dist[u] > m_distance_goal) {
                 throw found_goals();
             }
         }

 private:
     double m_distance_goal;
     std::vector<double> &m_dist;
};

template <typename V, typename E>
class dijkstra_distance_visitor_no_init : public boost::default_dijkstra_visitor {
 public:
     explicit dijkstra_distance_visitor_no_init(
             V root,
             double distance_goal,
             std::vector<V> &predecessors,
             std::vector<double> &distances,
             std::vector<boost::default_color_type> &color_map) :
         m_root(root),
         m_distance_goal(distance_goal),
         m_num_examined(0),
         m_predecessors(predecessors),
         m_dist(distances),
         m_color(color_map) {
             pgassert(m_distance_goal > 0);
         }

     template <class B_G>
         void examine_vertex(V u, B_G &) {
             if ( 0 == m_num_examined++) m_root = u;
             if (m_dist[u] > m_distance_goal) {
                 throw found_goals();
             }
             if (u != m_root && m_predecessors[u] == u) {
                 m_color[u] = boost::black_color;
             }
         }

     template <class B_G>
         void examine_edge(E e, B_G &g) {
             if (source(e, g) != m_root && m_predecessors[source(e, g)] == source(e, g)) {
                 m_color[target(e, g)] = boost::black_color;
             }
         }


     template <class B_G>
         void edge_not_relaxed(E e, B_G &g) {
             if (source(e, g) != m_root && m_predecessors[source(e, g)] == source(e, g)) {
                 m_color[target(e, g)] = boost::black_color;
             }
         }

     template <class B_G>
         void discover_vertex(V u, B_G &) {
             if (u  != m_root && m_predecessors[u] == u) {
                 m_color[u] = boost::black_color;
             }
         }

 private:
     V m_root;
     double m_distance_goal;
     size_t m_num_examined;
     std::vector<V > &m_predecessors;
     std::vector<double> &m_dist;
     std::vector<boost::default_color_type> &m_color;
};


}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DIJKSTRA_VISITORS_HPP_
