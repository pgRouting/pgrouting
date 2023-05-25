/*PGR-GNU*****************************************************************
File: withPoints_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#include "drivers/withPoints/withPoints_driver.h"

#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <cassert>
#include <limits>


#include "dijkstra/dijkstra.hpp"
#include "withPoints/pgr_withPoints.hpp"

#include "cpp_common/combinations.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

namespace {

template < class G >
std::deque<pgrouting::Path>
pgr_dijkstra(
        G &graph,
        const std::map<int64_t , std::set<int64_t>> &combinations,
        bool only_cost,
        bool normal) {
    size_t n_goals = (std::numeric_limits<size_t>::max)();
    auto paths = pgrouting::algorithms::dijkstra(graph, combinations, only_cost, n_goals);

    if (!normal) {
        for (auto &path : paths) {
            path.reverse();
        }
    }
    if (!only_cost) {
        for (auto &p : paths) {
            p.recalculate_agg_cost();
        }
    }
    return paths;
}

}  // namespace

// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid ANYARRAY,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true

void
do_pgr_withPoints(
        Edge_t *edges, size_t total_edges,
        Point_on_edge_t *points_p, size_t total_points,
        Edge_t *edges_of_points, size_t total_edges_of_points,

        II_t_rt *combinationsArr, size_t total_combinations,

        int64_t *start_vidsArr, size_t size_start_vidsArr,
        int64_t *end_vidsArr, size_t size_end_vidsArr,

        char driving_side,
        bool details,
        bool directed,
        bool only_cost,
        bool normal,

        Path_rt **return_tuples, size_t *return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
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
        pgassert((*return_count) == 0);
        pgassert(edges || edges_of_points);

        pgrouting::Pg_points_graph pg_graph(
                std::vector<Point_on_edge_t>(
                    points_p,
                    points_p + total_points),
                std::vector< Edge_t >(
                    edges_of_points,
                    edges_of_points + total_edges_of_points),
                normal,
                driving_side,
                directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }

        auto combinations = total_combinations?
            pgrouting::utilities::get_combinations(combinationsArr, total_combinations)
            : pgrouting::utilities::get_combinations(start_vidsArr, size_start_vidsArr, end_vidsArr, size_end_vidsArr);

        auto vertices(pgrouting::extract_vertices(edges, total_edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(pg_graph.new_edges());

            paths = pgr_dijkstra(
                    digraph,
                    combinations,
                    only_cost, normal);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());
            paths = pgr_dijkstra(
                    undigraph,
                    combinations,
                    only_cost, normal);
        }

        if (!details) {
            for (auto &path : paths) {
                path = pg_graph.eliminate_details(path);
            }
        }

        /*
         * order paths based on the start_pid, end_pid
         */
        std::sort(paths.begin(), paths.end(),
                [](const Path &a, const Path &b)
                -> bool {
                if (b.start_id() != a.start_id()) {
                return a.start_id() < b.start_id();
                }
                return a.end_id() < b.end_id();
                });

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

        log << "************************************************";
        log << pg_graph.get_log();
        log << "************************************************";

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
