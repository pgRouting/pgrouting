/*PGR-GNU*****************************************************************
File: combinations.hpp

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2021 Celia Virginia Vergara Castillo

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

#ifndef INCLUDE_CPP_COMMON_COMBINATIONS_HPP_
#define INCLUDE_CPP_COMMON_COMBINATIONS_HPP_
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <deque>
#include <vector>


#include "c_types/ii_t_rt.h"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/rule.hpp"


typedef struct ArrayType ArrayType;

namespace pgrouting {

class Path;

namespace utilities {


std::map<int64_t, std::set<int64_t>>
get_combinations(
        std::deque<Path>&,
        const std::vector<pgrouting::trsp::Rule>&);

std::map<int64_t, std::set<int64_t>>
get_combinations(const std::vector<II_t_rt>&);


std::map<int64_t, std::set<int64_t>>
get_combinations(const char*, ArrayType*, ArrayType*, bool);

}  // namespace utilities
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_COMBINATIONS_HPP_
