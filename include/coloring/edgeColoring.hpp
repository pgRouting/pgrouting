/*PGR-GNU*****************************************************************
File: edgeColoring.hpp

Generated with Template by:
Copyright (c) 2021-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2021 Veenit Kumar
Mail: 123sveenit@gmail.com
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

#ifndef INCLUDE_COLORING_EDGECOLORING_HPP_
#define INCLUDE_COLORING_EDGECOLORING_HPP_
#pragma once

#include <vector>
#include <cstdint>

#include "c_types/ii_t_rt.h"
#include "cpp_common/base_graph.hpp"

namespace pgrouting {
namespace functions {

std::vector<II_t_rt> edgeColoring(pgrouting::UndirectedGraph);


}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_COLORING_EDGECOLORING_HPP_
