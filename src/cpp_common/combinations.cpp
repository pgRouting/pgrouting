/*PGR-GNU*****************************************************************
File: combinations.cpp

Copyright (c) 2022 Celia Virginia Vergara Castillo

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
#include <deque>
#include <vector>
#include "cpp_common/basePath_SSEC.hpp"


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

std::map<int64_t, std::set<int64_t>>
get_combinations(
        std::deque<Path> &paths,
        const std::vector<pgrouting::trsp::Rule> &ruleList) {
    std::map<int64_t, std::set<int64_t>> new_combinations;

    for (auto &p : paths) {
        if (p.empty()) {
            new_combinations[p.start_id()].insert(p.end_id());
            continue;
        }
        std::deque<int64_t> edgesList(p.size());
        for (const auto &row : p) {
            edgesList.push_back(row.edge);
        }
        for (const auto &r : ruleList) {
            auto ptr = std::find(edgesList.begin(), edgesList.end(), r.precedences().front());
            if (ptr == edgesList.end()) continue;
            /*
             * And edge on the begining of a rule list was found
             * Checking if the complete rule applies
             */

            /*
             * Suppose the rule is used
             */
            bool rule_breaker = true;
            for (const auto &e : r.precedences()) {
                if (*ptr != e) {rule_breaker = false; break;}
                ++ptr;
            }
            if (rule_breaker) {
                new_combinations[p.start_id()].insert(p.end_id());
                p.clear();
            }
        }
    }
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p) {
                    return p.size() == 0;
                }),
                paths.end());
    return new_combinations;
}

}  // namespace utilities
}  // namespace pgrouting
