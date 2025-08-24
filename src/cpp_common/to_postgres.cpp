/*PGR-GNU*****************************************************************
File: to_postgres.cpp

Copyright (c) 2025 pgRouting developers
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

#include "cpp_common/to_postgres.hpp"

#include <cstddef>
#include <deque>

#include "c_types/routes_t.h"

#include "cpp_common/path.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

namespace {

void
get_path(
        int route_id,
        int path_id,
        const pgrouting::Path &path,
        Routes_t **postgres_data,
        double &route_agg_cost,
        size_t &sequence) {
    int path_seq = 0;
    for (const auto e : path) {
        (*postgres_data)[sequence] = {
            route_id,
            path_id,
            path_seq,
            path.start_id(),
            path.end_id(),
            e.node,
            e.edge,
            e.cost,
            e.agg_cost,
            route_agg_cost};
        route_agg_cost += path[path_seq].cost;
        path_seq++;
        ++sequence;
    }
}

}  // namespace

namespace pgrouting {
namespace to_postgres {

/**
 * @param[in] paths  The set of Paths
 * @param[out] tuples The C array of Route_t
 * @returns number of tuples on the C array
 *
 * Currently works for
 * - pgr_dijkstraVia
 * - pgr_trspVia
 * - pgr_trspVia_withPoints
 * - pgr_withPointsVia
 */
size_t
get_viaRoute(
        std::deque<pgrouting::Path> &paths,
        Routes_t **tuples) {
    pgassert(!(*tuples));

    auto count = count_tuples(paths);
    if (count == 0) return 0;

    (*tuples) = pgr_alloc(count, (*tuples));

    size_t sequence = 0;
    int path_id = 1;
    int route_id = 1;
    double route_agg_cost = 0;
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }
    for (const auto &path : paths) {
        if (path.size() > 0) {
            ::get_path(route_id, path_id, path, tuples, route_agg_cost, sequence);
        }
        ++path_id;
    }
    (*tuples)[count - 1].edge = -2;

    pgassert(count == sequence);

    return sequence;
}

}  // namespace to_postgres
}  // namespace pgrouting
