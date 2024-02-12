/*PGR-GNU*****************************************************************
File: astar_visitors.hpp

Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2015 pgRouting developers
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

#ifndef INCLUDE_VISITORS_ASTAR_VISITORS_HPP_
#define INCLUDE_VISITORS_ASTAR_VISITORS_HPP_

#include <set>
#include <boost/graph/astar_search.hpp>
#include "visitors/found_goals.hpp"

namespace pgrouting {
namespace visitors {

/** @brief visitor stops when all targets are found */
template <typename V>
class astar_many_goals_visitor : public boost::default_astar_visitor {
 public:
     explicit astar_many_goals_visitor(const std::set<V> &goals)
         :m_goals(goals) {}
     template <class B_G>
         void examine_vertex(V u, B_G &g) {
             auto s_it = m_goals.find(u);
             if (s_it == m_goals.end()) return;
             // found one more goal
             m_goals.erase(s_it);
             if (m_goals.size() == 0) throw found_goals();
             num_edges(g);
         }
 private:
     std::set<V> m_goals;
};

}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_ASTAR_VISITORS_HPP_
