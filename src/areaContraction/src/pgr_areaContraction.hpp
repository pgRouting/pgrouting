/*PGR-GNU*****************************************************************
File: pgr_areaContraction.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Ankur Shukla
Mail: work.ankurshukla@gmail.com

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

#ifndef SRC_AREACONTRACTION_SRC_PGR_AREACONTRACTION_HPP_
#define SRC_AREACONTRACTION_SRC_PGR_AREACONTRACTION_HPP_
#pragma once


#include <queue>
#include <functional>
#include <vector>

#include "cpp_common/identifiers.hpp"
#include "dijkstra/pgr_dijkstra.hpp"

namespace pgrouting {
namespace areacontraction {

template < class G >
class Pgr_areaContraction {
private:
  typedef typename G::V V;
  typedef typename G::E E;

public:
  void setBorderVertices(Identifiers<V> borderVertices);
  void doContraction(G &graph);

private:
  void dijkstra_many_many(G &graph, std::vector<int64_t> border_vertices);

private:
     Identifiers<V> border;
     Identifiers<V> target;
     std::deque<Path> paths;
     std::ostringstream debug;
};

/******* IMPLEMENTATION ************/
template< class G >
void
Pgr_areaContraction< G >::setBorderVertices(Identifiers<V> borderVertices){
#ifndef NDEBUG
  debug << "Setting forbidden vertices\n";
#endif
  border = borderVertices;
}


template< class G >
void
Pgr_areaContraction< G >::dijkstra_many_many(
  G &graph, std::vector<int64_t> border_vertices){
    Pgr_dijkstra< G > fn_dijkstra;
    paths = fn_dijkstra.dijkstra(graph, border_vertices, border_vertices);
  }

template< class G >
void
Pgr_areaContraction< G >::doContraction(G &graph){
#ifndef NDEBUG
    debug << "Performing contraction\n";
#endif

dijkstra_many_many(graph,border,border);


}

}  // namespace areacontraction
}  // namespace pgrouting

#endif  // SRC_AREACONTRACTION_SRC_PGR_AREACONTRACTION_HPP_
