/*PGR-GNU*****************************************************************
File: maxFlow_driver.h

Generated with Template by:
Copyright (c) 2007-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#ifndef INCLUDE_DRIVERS_MAXFLOW_DRIVER_HPP_
#define INCLUDE_DRIVERS_MAXFLOW_DRIVER_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <sstream>

#include "c_common/enums.h"

using Flow_t = struct Flow_t;
using ArrayType = struct ArrayType;

namespace pgrouting {
namespace drivers {

void do_maxFlow(
        const std::string&, const std::string&,
        ArrayType*, ArrayType*,

        Which,
        Flow_t*&, size_t&,
        std::ostringstream&, std::ostringstream&, std::ostringstream&);

}  // namespace drivers
}  // namespace pgrouting

#endif  // INCLUDE_DRIVERS_MAXFLOW_DRIVER_HPP_
