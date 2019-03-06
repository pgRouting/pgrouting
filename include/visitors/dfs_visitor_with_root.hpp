/*PGR-GNU*****************************************************************
File: Dfs_visitor_with_root.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Aditya Pratap Singh
adityapratap.singh28@gmail.com

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

#ifndef INCLUDE_VISITORS_DFS_VISITOR_WITH_ROOT_HPP_
#define INCLUDE_VISITORS_DFS_VISITOR_WITH_ROOT_HPP_
#pragma once

#include <boost/graph/depth_first_search.hpp>

#include <vector>

#include "visitors/found_goals.hpp"

namespace pgrouting {
namespace  visitors {

template <typename V, typename E>
class Dfs_visitor_with_root : public boost::default_dfs_visitor {
 public:
     Dfs_visitor_with_root(
             V root,
             std::vector<E> &data) :
         m_data(data),
         m_roots(root) {}
     template <typename B_G>
         void tree_edge(E e, const B_G&) {
             m_data.push_back(e);
         }
     template <typename B_G>
         void start_vertex(V v, const B_G&) {
             if (v != m_roots) throw found_goals();
         }

 private:
     std::vector<E> &m_data;
     V m_roots;
};


}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_DFS_VISITOR_WITH_ROOT_HPP_
