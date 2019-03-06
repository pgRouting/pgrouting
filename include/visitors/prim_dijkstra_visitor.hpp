/*PGR-GNU*****************************************************************
File: prim_dijkstra_visitor.hpp

Copyright (c) 2018 pgRouting developers
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
/*! @file */

#ifndef INCLUDE_VISITORS_PRIM_DIJKSTRA_VISITOR_HPP_
#define INCLUDE_VISITORS_PRIM_DIJKSTRA_VISITOR_HPP_
#pragma once

#if BOOST_VERSION_OK
#include <boost/graph/dijkstra_shortest_paths.hpp>
#else
#include "boost/dijkstra_shortest_paths.hpp"
#endif
#include <vector>


namespace pgrouting {
namespace  visitors {

/* Prim */
template <class V>
class Prim_dijkstra_visitor : public boost::default_dijkstra_visitor {
 public:
     explicit Prim_dijkstra_visitor(
             std::vector<V> &data) :
         m_data(data)  {}
     template <class B_G>
         void finish_vertex(V v, B_G&) {
             m_data.push_back(v);
         }
 private:
     std::vector<V> &m_data;
};

}  // namespace visitors
}  // namespace pgrouting

#endif  // INCLUDE_VISITORS_PRIM_DIJKSTRA_VISITOR_HPP_
