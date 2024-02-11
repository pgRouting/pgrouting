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

#ifndef INCLUDE_CPP_COMMON_LINEAR_DIRECTED_GRAPH_H_
#define INCLUDE_CPP_COMMON_LINEAR_DIRECTED_GRAPH_H_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "cpp_common/basic_edge.h"
#include "cpp_common/line_vertex.h"

namespace pgrouting {

/** @brief Data type to handle graph -> lineGaph transformation
 *
 * @details
 * Usage:
 * ~~~~{.c}
 * pgrouting::graph::Pgr_lineGraphFull<
 *      pgrouting::LinearDirectedGraph,  // using here
 *      pgrouting::Line_vertex,
 *      pgrouting::Basic_edge > line(graphType::DIRECTED);
 *
 * pgrouting::graph::Pgr_lineGraph<
 *      pgrouting::LinearDirectedGraph,  // using here
 *      pgrouting::Line_vertex,
 *      pgrouting::Basic_edge > line(graphType::DIRECTED);
 * ~~~~
 */
typedef boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Line_vertex, Basic_edge > LinearDirectedGraph;

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_LINEAR_DIRECTED_GRAPH_H_
