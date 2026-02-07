/*PGR-GNU*****************************************************************
File: combinations.cpp

Copyright (c) 2022-2026 pgRouting developers
Mail: project@pgrouting.org

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

#include <map>
#include <set>
#include <deque>
#include <vector>
#include <string>
#include "cpp_common/combinations.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/path.hpp"


namespace pgrouting {
namespace utilities {


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
             * And edge on the beginning of a rule list was found
             * Checking if the complete rule applies
             */

            /*
             * Suppose the rule is used
             */
            bool rule_breaker = true;
            for (const auto &e : r.precedences()) {
                if (*ptr != e) {
                    rule_breaker = false; break;
                }
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

std::map<int64_t , std::set<int64_t>>
get_combinations(const std::vector<II_t_rt> &combinations) {
    std::map<int64_t, std::set<int64_t>> result;

    for (const auto &row : combinations) {
        result[row.d1.source].insert(row.d2.target);
    }
    return result;
}

/** @brief gets all the departures and destinations
 * @param[in] combinations_sql from the @b combinations signatures
 * @param[in] startsArr PostgreSQL array with the departures
 * @param[in] endsArr PostgreSQL array with the destinations
 * @param[in] normal the graph is reversed so reverse starts & ends
 * @returns[out] map: for each departure a set of destinations
 *
 * The resulting map can be empty
 */
std::map<int64_t , std::set<int64_t>>
get_combinations(
        const char *combinations_sql,
        ArrayType* startsArr, ArrayType* endsArr, bool normal) {
    using pgrouting::pgget::get_intSet;
    std::map<int64_t, std::set<int64_t>> result;

    std::set<int64_t> starts;
    std::set<int64_t> ends;

    if (startsArr && endsArr) {
        starts = normal? get_intSet(startsArr) : get_intSet(endsArr);
        ends =   normal? get_intSet(endsArr) : get_intSet(startsArr);
    }

    /* TODO Read query storing like std::map<int64_t , std::set<int64_t>> */
    /* queries are stored in vectors */
    auto combinations = combinations_sql?
        pgrouting::pgget::get_combinations(std::string(combinations_sql)) : std::vector<II_t_rt>();

    /* data comes from a combinations */
    for (const auto &row : combinations) {
        result[row.d1.source].insert(row.d2.target);
    }

    /* data comes from many to many */
    for (const auto &s : starts) {
        result[s] = ends;
    }
    return result;
}

/** @brief gets all the departures and destinations
 * @param[in] combinations_sql from the @b combinations signatures
 * @param[in] startsArr PostgreSQL array with the departures
 * @param[in] endsArr PostgreSQL array with the destinations
 * @param[in] normal the graph is reversed so reverse starts & ends
 * @param[in] is_matrix set to @b true when data comes from costMatrix
 * @returns[out] map: for each departure a set of destinations
 *
 * When: combinations_sql comes from a combinations signature
 * When: startsArr && endsArr comes from a Cost signature
 * When: startsArr && !endsArr comes from a CostMatrix signature
 *
 * The resulting std::map can be empty
 */
std::map<int64_t , std::set<int64_t>>
get_combinations(
        const std::string &combinations_sql,
        ArrayType* startsArr, ArrayType* endsArr, bool normal, bool &is_matrix) {
    using pgrouting::pgget::get_intSet;
    using pgrouting::pgget::get_combinations;
    std::map<int64_t, std::set<int64_t>> result;

    auto starts = startsArr? (normal? get_intSet(startsArr) : get_intSet(endsArr))   : std::set<int64_t>();
    auto ends =     endsArr? (normal? get_intSet(endsArr)   : get_intSet(startsArr)) : std::set<int64_t>();

    /* TODO Read query storing like std::map<int64_t , std::set<int64_t>> */
    /* queries are stored in vectors */
    auto combinations = !combinations_sql.empty()? get_combinations(combinations_sql) : std::vector<II_t_rt>();

    /* data comes from CostMatrix */
    is_matrix = combinations.empty() && !starts.empty() && ends.empty();

    ends = is_matrix? starts : ends;

    /* data comes from a combinations */
    for (const auto &row : combinations) {
        result[row.d1.source].insert(row.d2.target);
    }

    /* data comes from many to many */
    for (const auto &s : starts) {
        result[s] = ends;
    }
    return result;
}

}  // namespace utilities
}  // namespace pgrouting
