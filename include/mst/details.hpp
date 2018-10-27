/*PGR-GNU*****************************************************************
File: pgr_kruskal.hpp

Copyright (c) 2018 Vicky Vergara


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

#ifndef INCLUDE_MST_DETAILS_KRUSKAL_HPP_
#define INCLUDE_MST_DETAILS_KRUSKAL_HPP_
#pragma once

#include <vector>
#include "c_types/pgr_edge_t.h"

namespace pgrouting {

namespace {
struct found_goals{}; //!< exception for dfs termination

std::vector<pgr_kruskal_t>
get_no_edge_graph_result(
        std::vector<int64_t> roots) {
    std::vector<pgr_kruskal_t> results;
    if (roots.empty()) return results;
    for (auto const root : roots) {
        results.push_back({root, 0, root, -1, 0.0, 0.0});
    }
    return results;
}

}  //namespace
}  // namespace pgrouting

#endif  // INCLUDE_MST_DETAILS_KRUSKAL_HPP_
