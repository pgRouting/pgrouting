/*PGR-GNU*****************************************************************
File: driving_distance_driver.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Aryan Gupta
guptaaryan1010 AT gmail.com
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

#include "drivers/driving_distance/driving_distance_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <map>

#include "cpp_common/pgdata_getters.hpp"
#include "dijkstra/drivingDist.hpp"

#include "c_types/mst_rt.h"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"


void
pgr_do_drivingDistance(
        char *edges_sql,
        ArrayType* starts,
        double distance,
        bool directed,
        bool equiCostFlag,
        MST_rt  **return_tuples,     size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::algorithm::drivingDistance;
    using pgrouting::pgget::get_intSet;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert((*return_tuples) == NULL);

        auto roots = get_intSet(starts);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        std::deque<Path> paths;
        std::vector<std::map<int64_t, int64_t>> depths;

        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            paths = drivingDistance(digraph, roots, distance, equiCostFlag, depths, true);
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            paths = drivingDistance(undigraph, roots, distance, equiCostFlag, depths, true);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            log << "\nNo return values were found";
            *notice_msg = to_pg_msg(log);
            return;
        }

        *return_tuples = pgr_alloc(count, (*return_tuples));
        *return_count = collapse_paths(return_tuples, paths);

        for (size_t i = 0; i < count; i++) {
            auto row = (*return_tuples)[i];
            /* given the depth assign the correct depth */
            int64_t depth = -1;
            for (const auto &d : depths) {
                /* look for the correct path */
                auto itr = d.find(row.from_v);
                if (itr == d.end() || !(itr->second == 0)) continue;
                depth = d.at(row.node);
            }
            (*return_tuples)[i].depth = depth;
        }
        (*return_count) = count;

        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch( ... ) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
