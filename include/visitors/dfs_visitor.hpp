/*PGR-GNU*****************************************************************
File: Dfs_visitor_with_root.hpp

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

#include <boost/graph/depth_first_search.hpp>

#include <vector>
#include <set>
#include <map>

namespace pgrouting {
namespace  visitors {

template <typename V, typename E, typename G>
class Dfs_visitor : public boost::default_dfs_visitor {
 public:
     Dfs_visitor(
             V root,
             std::vector<boost::default_color_type> &colors,
             G &graph) :
         m_roots(root),
         m_colors(colors),
         m_graph(graph),
         time(0),
         depth(0) {}
     template <typename B_G>
         void initialize_vertex(V v, const B_G&) {
             std::cout << "initialize vertex " << v << "\n";
         }
     template <typename B_G>
         void start_vertex(V v, const B_G&) {
             std::cout << "color " << m_colors[v] << "\n";
             std::cout << "start vertex " << v << "\n";
         }
     template <typename B_G>
         void discover_vertex(V v, const B_G&) {
             std::cout << "color " << m_colors[v] << "\n";
             std::cout << "\ntime " << time++ << "\n";
             std::cout << "discover vertex " << v << "\n";
         }
     template <typename B_G>
         void examine_edge(E e, const B_G&) {
             std::cout << "examine edge " << e << "\n";
         }
     template <typename B_G>
         void tree_edge(E e, const B_G&) {
             std::cout << "tree edge " << e << "\n";
             depth++;
             edge_set.insert(e);
             std::cout << "depth " << depth << "\n";
         }
     template <typename B_G>
         void back_edge(E e, const B_G&) {
             std::cout << "back edge " << e << "\n";
         }
     template <typename B_G>
         void forward_or_cross_edge(E e, const B_G&) {
             std::cout << "forward or cross edge " << e << "\n";
         }
     template <typename B_G>
         void finish_edge(E e, const B_G&) {
             std::cout << "finish edge " << e << "\n";
             if (edge_set.find(e) != edge_set.end()) {
                 depth--;
                 std::cout << "depth " << depth << "\n";
             }
         }
     template <typename B_G>
         void finish_vertex(V v, const B_G&) {
             std::cout << "color " << m_colors[v] << "\n";
             std::cout << "finish vertex " << v << "\n";
         }

 private:
     V m_roots;
     std::vector<boost::default_color_type> &m_colors;
     G &m_graph;
     int64_t time;
     int64_t depth;
     std::set<E> edge_set;
};


}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DFS_VISITOR_HPP_
