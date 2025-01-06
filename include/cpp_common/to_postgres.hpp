/*PGR-GNU*****************************************************************
File: to_postgres.hpp

Copyright (c) 2015 pgRouting developers
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

#ifndef INCLUDE_CPP_COMMON_TO_POSTGRES_HPP_
#define INCLUDE_CPP_COMMON_TO_POSTGRES_HPP_
#pragma once

#include <vector>
#include <cstddef>

#include "c_types/iid_t_rt.h"

#include "cpp_common/base_graph.hpp"
#include "cpp_common/alloc.hpp"

namespace pgrouting {
namespace to_postgres {

/** @brief Stored results on a vector are saved on a C array
 *
 * @param[in] graph Created graph with the base Graph
 * @param[in] results results[i] -> the ith element of the vector contains the results
 * @param[out] result_count The size of the vector
 * @param[out] result_tuples The C array of <bigint, bigint, float>
 *
 * <bigint, bigint, float> =  < i , 0, results[i] >
 *
 * Currently works for pgr_betweennessCentrality
 */
template <class G>
void vector_to_tuple(
            const G &graph,
            const std::vector<double> results,
            size_t &result_count,
            IID_t_rt **result_tuples) {
    result_count = results.size();
    *result_tuples = pgrouting::pgr_alloc(result_count, (*result_tuples));

    size_t seq = 0;
    for (typename G::V v_i = 0; v_i < graph.num_vertices(); ++v_i) {
        (*result_tuples)[seq].from_vid = graph[v_i].id;
        /*
         * These 2 lines are specifically for pgr_betweennessCentrality
         */
        (*result_tuples)[seq].to_vid = 0;
        (*result_tuples)[seq].cost = graph.is_directed()? results[v_i] / 2.0 : results[v_i];
        seq++;
    }
}

}  // namespace to_postgres
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_TO_POSTGRES_HPP_
