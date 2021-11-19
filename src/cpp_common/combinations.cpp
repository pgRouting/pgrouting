/*PGR-GNU*****************************************************************
File: one_to_many_withPoints_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

#include "cpp_common/combinations.h"

#include <map>
#include <set>


namespace pgrouting {
namespace utilities {

std::map<int64_t , std::set<int64_t>>
get_combinations(const II_t_rt *combinations, size_t total) {
    std::map<int64_t, std::set<int64_t>> result;

    /* TODO(vicky) maybe use std::for_each */
    for (size_t i = 0; i < total; i++) {
        auto row = combinations[i];
        result[row.d1.source].insert(row.d2.target);
    }
    return result;
}

std::map<int64_t , std::set<int64_t>>
get_combinations(
        int64_t  *start_arr,
        size_t size_start_arr,
        int64_t  *end_arr,
        size_t size_end_arr) {
    std::map<int64_t, std::set<int64_t>> result;

    for (size_t i = 0; i < size_start_arr; ++i) {
        for (size_t j = 0; j < size_end_arr; ++j) {
            result[start_arr[i]].insert(end_arr[j]);
        }
    }
    return result;
}

}  // namespace utilities
}  // namespace pgrouting
