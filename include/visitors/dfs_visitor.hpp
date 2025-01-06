/*PGR-GNU*****************************************************************
File: dfs_visitor.hpp

Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Ashish Kumar
Mail: ashishkr23438@gmail.com

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

#ifndef INCLUDE_VISITORS_DFS_VISITOR_HPP_
#define INCLUDE_VISITORS_DFS_VISITOR_HPP_
#pragma once

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <cstdint>

#include <boost/graph/depth_first_search.hpp>

#include "visitors/found_goals.hpp"

namespace pgrouting {
namespace  visitors {

template <typename V, typename E, typename G>
class Dfs_visitor : public boost::default_dfs_visitor {
 public:
     Dfs_visitor(
             V root,
             std::vector<E> &data,
             int64_t max_depth,
             std::vector<boost::default_color_type> &colors,
             G &graph) :
         m_roots(root),
         m_data(data),
         m_max_depth(max_depth),
         m_colors(colors),
         m_graph(graph) {
             m_depth.resize(m_graph.num_vertices(), 0);
         }
     template <typename B_G>
         void start_vertex(V v, const B_G&) {
             // exception for visitor termination
             if (v != m_roots) throw found_goals();
             m_depth[v] = 0;
         }
     template <typename B_G>
         void examine_edge(E e, const B_G&) {
             auto source = m_graph.source(e), target = m_graph.target(e);
             // If the target has not been visited before
             if (m_depth[target] == 0 && target != m_roots)
                 m_depth[target] = m_depth[source] + 1;

             // If the max_depth is reached, mark the target as visited, and push the corresponding edge
             if (m_depth[target] == m_max_depth && m_colors[target] != 4) {
                 m_colors[target] = boost::black_color;
                 m_data.push_back(e);
             }
         }
     template <typename B_G>
         void tree_edge(E e, const B_G&) {
             m_data.push_back(e);
         }

 private:
     V m_roots;
     std::vector<E> &m_data;
     int64_t m_max_depth;
     std::vector<boost::default_color_type> &m_colors;
     G &m_graph;
     std::vector<int64_t> m_depth;
};


}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DFS_VISITOR_HPP_
