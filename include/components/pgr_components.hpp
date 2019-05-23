/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

#ifndef INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/biconnected_components.hpp>

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/identifiers.hpp"
#include "components/componentsResult.h"

namespace pgrouting {
namespace algorithms {

/**
 * works for undirected graph
 **/
std::vector<pgr_components_rt>
pgr_connectedComponents(pgrouting::UndirectedGraph &graph);

//! Strongly Connected Components Vertex Version
std::vector<pgr_components_rt>
strongComponents(pgrouting::DirectedGraph &graph);

//! Biconnected Components (for undirected)
std::vector<pgr_components_rt>
biconnectedComponents(pgrouting::UndirectedGraph &graph);

//! Articulation Points
Identifiers<int64_t>
articulationPoints(pgrouting::UndirectedGraph &graph);

//! Bridges
Identifiers<int64_t>
bridges(pgrouting::UndirectedGraph &graph);

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
