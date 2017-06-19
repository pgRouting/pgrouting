/*PGR-GNU*****************************************************************
File: pgr_deadend.hpp

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
  void setBorderVertices(Identifier<V> borderVertices);
  //To make target array for one to many dijkstra
  void makeTarget(V v);
  void callDijkstra(G &graph, V v);
  void doContraction(G &graph);



private:
     Identifiers<V> border;
     Identifiers<V> target;
     std::deque<Path> paths;
     std::ostringstream debug;
};

/******* IMPLEMENTATION ************/
template< class G >
void
Pgr_areaContraction< G >::setBorderVertices(
  Identifier<V> borderVertices){
#ifndef NDEBUG
  debug << "Setting forbidden vertices\n";
#endif
  border = borderVertices;
}

template< class G >
void
Pgr_areaContraction< G >::makeTarget(
  V v){
    #ifndef NDEBUG
      debug << "Creating target vector by removing vertex from border vertices\n";
    #endif
    target=border.erase(v)
  }

template< class G >
void
Pgr_areaContraction< G >::callDijkstra(
  G &graph, int64_t source, std::vector<int64_t> targets){
    Pgr_dijkstra< G > fn_dijkstra;
    auto paths = fn_dijkstra.dijkstra(graph, source, targets);
  }

  template< class G >
  void
  Pgr_areaContraction< G >::doContraction(G &graph){
    #ifndef NDEBUG
        debug << "Performing contraction\n";
    #endif

    for (V vertex: border){

    }

  }



}
}
