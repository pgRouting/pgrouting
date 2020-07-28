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
             int64_t max_depth,
             std::vector<boost::default_color_type> &colors,
             G &graph) :
         m_roots(root),
         m_max_depth(max_depth),
         m_colors(colors),
         m_graph(graph),
         depth(0) {}
     template <typename B_G>
         void initialize_vertex(V v, const B_G&) {
             std::cout << "initialize vertex " << v << "\n";
         }
     template <typename B_G>
         void start_vertex(V v, const B_G&) {
             std::cout << "start vertex " << v << "\t\t" << "color " << m_colors[v];
             std::cout << "\tdepth " << depth << "\n";
         }
     template <typename B_G>
         void discover_vertex(V v, const B_G&) {
             std::cout << "\n";
             std::cout << "discover vertex " << v << "\t" << "color " << m_colors[v];
             std::cout << "\tdepth " << depth << "\n";
         }
     template <typename B_G>
         void examine_edge(E e, const B_G&) {
             int64_t source = boost::source(e, m_graph), target = boost::target(e, m_graph);
             std::cout << "examine edge " << e << "\t" << "vertex " << source << " color " << m_colors[source]
                       << " vertex " << target << " color " << m_colors[target];
             std::cout << "\tdepth " << depth << "\n";
         }
     template <typename B_G>
         void tree_edge(E e, const B_G&) {
             int64_t source = boost::source(e, m_graph), target = boost::target(e, m_graph);
             std::cout << "tree edge " << e << "\t\t" << "vertex " << source << " color " << m_colors[source]
                       << " vertex " << target << " color " << m_colors[target];
             depth++;
             edge_set.insert(e);
             std::cout << "\tdepth " << depth << "\n";
         }
#if 1
     template <typename B_G>
         void back_edge(E e, const B_G&) {
             int64_t source = boost::source(e, m_graph), target = boost::target(e, m_graph);
             std::cout << "back edge " << e << "\t\t" << "vertex " << source << " color " << m_colors[source]
                       << " vertex " << target << " color " << m_colors[target];
             std::cout << "\tdepth " << depth << "\n";
         }
     template <typename B_G>
         void forward_or_cross_edge(E e, const B_G&) {
             int64_t source = boost::source(e, m_graph), target = boost::target(e, m_graph);
             std::cout << "forward or cross edge " << e << "\t" << "vertex " << source << " color " << m_colors[source]
                       << " vertex " << target << " color " << m_colors[target];
             std::cout << "\tdepth " << depth << "\n";
         }
#endif
     template <typename B_G>
         void finish_edge(E e, const B_G&) {
             int64_t source = boost::source(e, m_graph), target = boost::target(e, m_graph);
             std::cout << "finish edge " << e << "\t" << "vertex " << source << " color " << m_colors[source]
                       << " vertex " << target << " color " << m_colors[target];
             if (edge_set.find(e) != edge_set.end()) {
                 depth--;
             }
             std::cout << "\tdepth " << depth << "\n";
         }
     template <typename B_G>
         void finish_vertex(V v, const B_G&) {
             std::cout << "finish vertex " << v << "\t\t" << "color " << m_colors[v];
             std::cout << "\tdepth " << depth << "\n";
         }

 private:
     V m_roots;
     int64_t m_max_depth;
     std::vector<boost::default_color_type> &m_colors;
     G &m_graph;
     int64_t depth;
     std::set<E> edge_set;
};


}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DFS_VISITOR_HPP_
