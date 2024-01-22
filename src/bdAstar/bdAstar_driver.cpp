/*PGR-GNU*****************************************************************
File: bdAstar_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2016 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#include "drivers/bdAstar/bdAstar_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "bdAstar/bdAstar.hpp"

#include "cpp_common/combinations.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "c_types/ii_t_rt.h"


void
pgr_do_bdAstar(
        Edge_xy_t *edges, size_t total_edges,

        II_t_rt *combinationsArr, size_t total_combinations,

        int64_t  *start_vidsArr, size_t size_start_vidsArr,
        int64_t  *end_vidsArr, size_t size_end_vidsArr,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,

        Path_rt **return_tuples, size_t *return_count,
        char** log_msg, char** notice_msg, char** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        auto combinations = total_combinations?
            pgrouting::utilities::get_combinations(combinationsArr, total_combinations)
            : pgrouting::utilities::get_combinations(start_vidsArr, size_start_vidsArr, end_vidsArr, size_end_vidsArr);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque<Path> paths;
        if (directed) {
            pgrouting::xyDirectedGraph digraph(pgrouting::extract_vertices(edges, total_edges), gType);
            digraph.insert_edges(edges, total_edges);

            paths = pgrouting::algorithms::bdastar(digraph, combinations, heuristic, factor, epsilon, only_cost);
        } else {
            pgrouting::xyUndirectedGraph undigraph(pgrouting::extract_vertices(edges, total_edges), gType);
            undigraph.insert_edges(edges, total_edges);

            paths = pgrouting::algorithms::bdastar(undigraph, combinations, heuristic, factor, epsilon, only_cost);
        }

        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = nullptr;
            (*return_count) = 0;
            notice << "No paths found\n";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = (collapse_paths(return_tuples, paths));


        pgassert(*err_msg == nullptr);
        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
