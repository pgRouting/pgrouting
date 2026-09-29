/*PGR-GNU*****************************************************************
File: maxWeightedMatching.hpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2026 Mayur Galhate
Mail: galhatemayur at gmail.com

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

#ifndef INCLUDE_MAX_FLOW_MAXWEIGHTEDMATCHING_HPP_
#define INCLUDE_MAX_FLOW_MAXWEIGHTEDMATCHING_HPP_
#pragma once

#include <vector>

#include "c_types/iid_t_rt.h"

#include "cpp_common/identifiers.hpp"
#include "cpp_common/undirectedHasCostBG.hpp"


namespace pgrouting {
namespace functions {

Identifiers<int64_t> maxWeightedMatch(pgrouting::graph::UndirectedHasCostBG&);

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_MAXWEIGHTEDMATCHING_HPP_
