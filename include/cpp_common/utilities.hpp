/*PGR-GNU*****************************************************************
File: utilities.hpp

Copyright (c) 2016-2026 pgRouting developers
Mail: project@pgrouting.org

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

#ifndef INCLUDE_CPP_COMMON_UTILITIES_HPP_
#define INCLUDE_CPP_COMMON_UTILITIES_HPP_
#pragma once

#include <string>

#include "c_common/enums.h"

namespace pgrouting {

std::string get_name(Which);
std::string get_name(Which, bool, bool);
std::string get_name(Which, bool, bool, bool);
char estimate_drivingSide(char, Which);
void get_new_queries(const std::string&, const std::string&, std::string&, std::string&);

}  //  namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_UTILITIES_HPP_
