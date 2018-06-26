/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Maoguang Wang 
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

#ifndef INCLUDE_CHPP_PGR_CHPP_HPP_
#define INCLUDE_CHPP_PGR_CHPP_HPP_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <limits>

#include "costFlow/pgr_costFlowGraph.hpp"
#include "c_types/general_path_element_t.h"
#include "c_types/pgr_edge_t.h"

namespace pgrouting {
namespace graph {

class PgrDirectedChPPGraph {
 public:
     PgrDirectedChPPGraph(
             pgr_edge_t *data_edges,
             size_t total_edges);
     double DirectedChPP() {
         double min_cost = flowGraph.minCostMaxFlow();


 private:
     PgrCostFlowGraph flowGraph;
     std::vector<pgr_edge_t> resultEdges;
}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_CHPP_PGR_CHPP_HPP_
