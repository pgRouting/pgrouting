/*PGR-GNU*****************************************************************
File: trsp_withPoints_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo
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

#include "drivers/trsp/trsp_withPoints_driver.h"

#include <utility>
#include <vector>
#include <cstdint>
#include <sstream>
#include <deque>
#include <algorithm>
#include <limits>
#include <set>
#include <map>
#include <string>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/rule.hpp"
#include "cpp_common/combinations.hpp"
#include "withPoints/withPoints.hpp"
#include "trsp/trspHandler.hpp"

#include "dijkstra/dijkstra.hpp"

namespace {

void
post_process_trsp(std::deque<pgrouting::Path> &paths) {
    using pgrouting::Path;
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p) {
                return p.size() == 0;
                }),
            paths.end());
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }

    std::sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.end_id() < e2.end_id();
            });
    std::stable_sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.start_id() < e2.start_id();
            });
}
}  // namespace

void
pgr_do_trsp_withPoints(
        char *edges_sql,
        char *restrictions_sql,
        char *points_sql,
        char *edges_of_points_sql,
        char *combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        char driving_side,
        bool details,

        Path_rt **return_tuples, size_t *return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::utilities::get_combinations;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, true);
        hint = nullptr;

        if (combinations.empty() && combinations_sql) {
            *notice_msg = to_pg_msg("No (source, target) pairs found");
            *log_msg = to_pg_msg(combinations_sql);
            return;
        }

        hint = points_sql;
        auto points = pgrouting::pgget::get_points(std::string(points_sql));

        hint = edges_of_points_sql;
        auto edges_of_points = pgrouting::pgget::get_edges(std::string(edges_of_points_sql), true, false);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.size() + edges_of_points.size() == 0) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }

        hint = restrictions_sql;
        auto restrictions = restrictions_sql?
            pgrouting::pgget::get_restrictions(std::string(restrictions_sql)) : std::vector<Restriction_t>();




        /* Dealing with points */
        pgrouting::Pg_points_graph pg_graph(
                points, edges_of_points,
                true,
                driving_side,
                directed);
        log << pg_graph.get_log();

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = to_pg_msg(log);
            *err_msg = to_pg_msg(err);
            return;
        }

        auto vertices(pgrouting::extract_vertices(edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            digraph.insert_edges(pg_graph.new_edges());

            paths = pgrouting::algorithms::dijkstra(
                    digraph,
                    combinations,
                    false, (std::numeric_limits<size_t>::max)());
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            undigraph.insert_edges(pg_graph.new_edges());

            paths = pgrouting::algorithms::dijkstra(
                    undigraph,
                    combinations,
                    false, (std::numeric_limits<size_t>::max)());
        }

        post_process_trsp(paths);

        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            notice << "No paths found";
            *log_msg = to_pg_msg(notice);
            return;
        }

        if (!restrictions_sql || restrictions.empty()) {
            if (!details) {
                for (auto &path : paths) {
                    path = pg_graph.eliminate_details(path);
                }
            }

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
        for (const auto &r : restrictions) {
            if (r.via) ruleList.push_back(pgrouting::trsp::Rule(r));
        }
        hint = nullptr;

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        if (!new_combinations.empty()) {
            pgrouting::trsp::TrspHandler gdef(
                    edges,
                    pg_graph.new_edges(),
                    directed,
                    ruleList);
            auto new_paths = gdef.process(new_combinations);
            paths.insert(paths.end(), new_paths.begin(), new_paths.end());
        }
        post_process_trsp(paths);
        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }


        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = collapse_paths(return_tuples, paths);

        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
