/*PGR-GNU*****************************************************************
File: dagShortestPath.hpp

Copyright (c) 2013-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Sourabh Garg
sourabh.garg.mat@gmail.com

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

#ifndef INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
#define INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <cstdint>

#include "cpp_common/path.hpp"
#include "cpp_common/base_graph.hpp"

namespace pgrouting {

namespace algorithms {

std::deque<pgrouting::Path>
dagShortestPath(
        pgrouting::DirectedGraph&,
        std::map<int64_t, std::set<int64_t>>&,
        bool only_cost = false);

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_DAGSHORTESTPATH_DAGSHORTESTPATH_HPP_
