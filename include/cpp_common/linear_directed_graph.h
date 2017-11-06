/*PGR-GNU*****************************************************************
 *

Copyright (c) 2017 Anthony Nicola Tasca
atasca10@gmail.com

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

#ifndef INCLUDE_CPP_COMMON_LINEAR_DIRECTED_GRAPH_
#define INCLUDE_CPP_COMMON_LINEAR_DIRECTED_GRAPH_
#pragma once

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/line_vertex.h"

namespace pgrouting {

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_turnPenaltyGraph;

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph;

}  // namespace graph

typedef graph::Pgr_turnPenaltyGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Line_vertex, Basic_edge >,
    Line_vertex, Basic_edge > LinearDirectedGraph;

typedef graph::Pgr_lineGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Line_vertex, Basic_edge >,
    Line_vertex, Basic_edge > LinearDirectedGraph;

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_LINEAR_DIRECTED_GRAPH_
