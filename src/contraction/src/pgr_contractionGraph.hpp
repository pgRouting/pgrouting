/*PGR-GNU*****************************************************************
File: pgr_contractionGraph.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

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
#pragma once
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif

#include "../../common/src/pgr_base_graph.hpp"
namespace pgRouting {

namespace graph {

template <class G, typename T_V, typename T_E>
        class Pgr_contractionGraph;
    }

typedef typename graph::Pgr_contractionGraph <
boost::adjacency_list < boost::listS, boost::vecS,
boost::undirectedS,
contraction::Vertex, Basic_edge >,
contraction::Vertex, Basic_edge > CHUndirectedGraph;

typedef typename graph::Pgr_contractionGraph <
boost::adjacency_list < boost::listS, boost::vecS,
boost::bidirectionalS,
contraction::Vertex, Basic_edge >,
contraction::Vertex, Basic_edge > CHDirectedGraph;

namespace graph{

template <class G, typename T_V, typename T_E>
class Pgr_contractionGraph : public Pgr_base_graph<G, T_V, T_E> {
 public:
     Pgr_contractionGraph< G , T_V, T_E >(const std::vector< T_V > &vertices, graphType gtype)
     : Pgr_base_graph< G , T_V, T_E >(vertices, gtype) {
     }

     explicit Pgr_contractionGraph< G , T_V, T_E >(graphType gtype)
         : Pgr_base_graph< G , T_V, T_E >(gtype) {
     }

     bool is_connected(int64_t v) const {
        if (this->in_degree(this->get_V(v)) == 0 && this->out_degree(this->get_V(v)) == 0) {
            return false;
        }
        return true;
    }

    };
}

}
