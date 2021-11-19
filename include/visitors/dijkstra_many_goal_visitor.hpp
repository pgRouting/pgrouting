/*PGR-GNU*****************************************************************
file: dijkstra_many_goal_visitor.hpp

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2021 Celia Virginia Vergara Castillo
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

#ifndef INCLUDE_VISITORS_DIJKSTRA_MANY_GOAL_VISITOR_HPP_
#define INCLUDE_VISITORS_DIJKSTRA_MANY_GOAL_VISITOR_HPP_

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <set>

#include "visitors/found_goals.hpp"

namespace pgrouting {
namespace visitors {

/** stopping when a distance/cost has being surpassed */
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
     std::set< V > m_goals;
     size_t m_n_goals;
     std::set< V > &m_found_goals;
};

}  // namespace visitors
}  // namespace pgrouting


#endif  // INCLUDE_VISITORS_DIJKSTRA_MANY_GOAL_VISITOR_HPP_
