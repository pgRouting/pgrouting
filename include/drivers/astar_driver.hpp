/*PGR-GNU*****************************************************************
File: astar_driver.hpp

Copyright (c) 2007-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
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

#ifndef INCLUDE_DRIVERS_ASTAR_DRIVER_HPP_
#define INCLUDE_DRIVERS_ASTAR_DRIVER_HPP_
#pragma once


#include <cstddef>
#include <cstdint>
#include <string>
#include <sstream>

#include "c_common/enums.h"

using Path_rt = struct Path_rt;
using ArrayType = struct ArrayType;

namespace pgrouting {
namespace drivers {

void do_astar(
        const std::string&, const std::string&,
        ArrayType*, ArrayType*,

        bool, bool, bool,

        int, double, double,

        Which,
        bool&,
        Path_rt*&, size_t&,
        std::ostringstream&, std::ostringstream&, std::ostringstream&);

}  // namespace drivers
}  // namespace pgrouting

#endif  // INCLUDE_DRIVERS_ASTAR_DRIVER_HPP_
