/*PGR-GNU*****************************************************************
file: dijkstra_distance_visitor.hpp

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

#ifndef INCLUDE_VISITORS_DIJKSTRA_DISTANCE_VISITOR_HPP_
#define INCLUDE_VISITORS_DIJKSTRA_DISTANCE_VISITOR_HPP_

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <deque>
#include <vector>

#include "visitors/found_goals.hpp"
#include "cpp_common/pgr_assert.h"

namespace pgrouting {
namespace visitors {

template <class V>
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
     std::deque< V > &m_nodes;
     std::vector< double > &m_dist;
};

}  // namespace visitors
}  // namespace pgrouting


#endif  // INCLUDE_VISITORS_DIJKSTRA_DISTANCE_VISITOR_HPP_
