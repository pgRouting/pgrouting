/*PGR-GNU*****************************************************************
File: pgr_kruskal.hpp

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

#ifndef INCLUDE_MST_MST_VISITORS_HPP_
#define INCLUDE_MST_MST_VISITORS_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <vector>
#include "mst/details.hpp" // for found_goals()

namespace pgrouting {
namespace  visitors {

/* Prim */
template <class V>
class Prim_visitor : public boost::default_dijkstra_visitor {
    public:
        explicit Prim_visitor(
                std::vector<V> &data) :
            m_data(data)  {}
        template <class B_G>
            void finish_vertex(V v, B_G&) {
                m_data.push_back(v);
            }
    private:
        std::vector<V> &m_data;
};

/* BFS */
template <class E>
class Bfs_visitor : public boost::default_bfs_visitor {
    public:
        explicit Bfs_visitor(
                std::vector<E> &data) :
            m_data(data)  {}
        template <class B_G>
            void tree_edge(E e, const B_G&) {
                m_data.push_back(e);
            }
    private:
        std::vector<E> &m_data;
};

/* DFS */
template <class E>
class Dfs_visitor : public boost::default_dfs_visitor {
    public:
        explicit Dfs_visitor(
                std::vector<E> &data) :
            m_data(data)  {}
        template <typename B_G>
            void tree_edge(E e, const B_G&) {
                m_data.push_back(e);
            }
    private:
        std::vector<E> &m_data;
};

/* DFS */
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

#endif  // INCLUDE_MST_MST_VISITORS_HPP_
