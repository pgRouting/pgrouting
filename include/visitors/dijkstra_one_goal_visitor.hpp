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

#if Boost_VERSION_MACRO >= 105500
#include <boost/graph/dijkstra_shortest_paths.hpp>
#else
#include "boost/dijkstra_shortest_paths.hpp"
#endif

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

}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DIJKSTRA_ONE_GOAL_VISITOR_HPP_
