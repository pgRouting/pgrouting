
/*PGR-GNU*****************************************************************
File: combinations.h

Copyright (c) 2021 Celia Virginia Vergara Castillo
Mail: vicky at georepublic.de

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

#ifndef INCLUDE_CPP_COMMON_COMBINATIONS_H_
#define INCLUDE_CPP_COMMON_COMBINATIONS_H_
#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <deque>
#include <vector>

#include "c_types/ii_t_rt.h"
#include "cpp_common/basePath_SSEC.hpp"

namespace pgrouting {
namespace utilities {

std::map<int64_t, std::set<int64_t>>
get_combinations(const II_t_rt *, size_t);

std::map<int64_t, std::set<int64_t>>
get_combinations(int64_t*, size_t, int64_t*, size_t);

std::map<int64_t, std::set<int64_t>>
get_combinations(
        std::deque<Path>&,
        const std::vector<pgrouting::trsp::Rule>&);

}  // namespace utilities
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_COMBINATIONS_H_
