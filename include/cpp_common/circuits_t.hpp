/*PGR-GNU*****************************************************************
File: circuits_t.hpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2026 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#ifndef INCLUDE_CPP_COMMON_CIRCUITS_T_HPP_
#define INCLUDE_CPP_COMMON_CIRCUITS_T_HPP_
#pragma once

#include <cstdint>
#include <vector>

namespace pgrouting {
namespace types {

/** @brief One hop of an arc-distinct circuit */
struct Circuit_step {
    int64_t node;
    int64_t edge;
    double cost;
};

using Circuit = std::vector<Circuit_step>;
using Circuits = std::vector<Circuit>;

}  // namespace types
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_CIRCUITS_T_HPP_
