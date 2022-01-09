/*PGR-GNU*****************************************************************
File: dijkstras_driver.cpp

Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
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

#include "drivers/dijkstra/dijkstras_driver.h"

#include <utility>
#include <vector>
#include <cstdint>
#include <sstream>
#include <deque>
#include <algorithm>
#include <limits>
#include <set>
#include <map>
#include <cassert>

#include "trsp/pgr_trspHandler.h"
#include "cpp_common/rule.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/combinations.h"
#include "c_types/restriction_t.h"
#include "c_types/ii_t_rt.h"
#include "dijkstra/dijkstra.hpp"
#include "withPoints/pgr_withPoints.hpp"


namespace {

template < class G >
std::deque< Path >
pgr_dijkstra(
        G &graph,
        std::map<int64_t , std::set<int64_t>> &combinations,
        bool only_cost,
        bool normal) {
    size_t n_goals = (std::numeric_limits<size_t>::max)();
    auto paths = pgrouting::dijkstra(graph, combinations, only_cost, n_goals);

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

void
do_dijkstras(
        Edge_t *edges, size_t total_edges,
        Restriction_t *restrictions, size_t restrictions_size,
        Point_on_edge_t *points_p, size_t total_points,
        Edge_t *edges_of_points, size_t total_edges_of_points,

        II_t_rt *combinations_arr, size_t total_combinations,
        int64_t *start_pidsArr, size_t size_start_pidsArr,
        int64_t *end_pidsArr, size_t size_end_pidsArr,

        char driving_side,
        bool details,
        bool directed,
        bool only_cost,
        bool normal,
        int64_t n_goals,
        bool global,

        Path_rt **return_tuples, size_t *return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
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
            pgrouting::utilities::get_combinations(combinations_arr, total_combinations)
            : pgrouting::utilities::get_combinations(start_pidsArr, size_start_pidsArr, end_pidsArr, size_end_pidsArr);

        auto vertices(pgrouting::extract_vertices(edges, total_edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        graphType gType = directed? DIRECTED: UNDIRECTED;

        size_t n = n_goals <= 0? (std::numeric_limits<size_t>::max)() : static_cast<size_t>(n_goals);
        std::deque< Path > paths;
        if (directed) {
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(pg_graph.new_edges());

            paths = pgr_dijkstra(
                    digraph,
                    combinations,
                    only_cost, normal);
        } else {
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());
            paths = pgr_dijkstra(
                    undigraph,
                    combinations,
                    only_cost, normal);
        }

        if (restrictions_size == 0) {
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
            (*return_count) = (collapse_paths(return_tuples, paths));
            return;
        }

        /*
         * When there are turn restrictions
         */
        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < restrictions_size; ++i) {
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        std::deque<Path> new_paths;
        if (!new_combinations.empty()) {
            pgrouting::trsp::Pgr_trspHandler gdef(
                    edges,
                    total_edges,
                    pg_graph.new_edges(),
                    directed,
                    ruleList);
            auto new_paths = gdef.process(new_combinations);
            paths.insert(paths.end(), new_paths.begin(), new_paths.end());
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

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
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
