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

#include "trsp/pgr_trspHandler.h"
#include "cpp_common/pgget.hpp"
#include "cpp_common/rule.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/combinations.h"
#include "c_types/restriction_t.h"
#include "c_types/ii_t_rt.h"
#include "dijkstra/dijkstra.hpp"
#include "withPoints/pgr_withPoints.hpp"


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

template <class G>
std::deque<pgrouting::Path>
pgr_dijkstra(
        G &graph,
        std::map<int64_t, std::set<int64_t>> &combinations
        ) {
    auto paths = pgrouting::algorithms::dijkstra(
            graph,
            combinations,
            false, (std::numeric_limits<size_t>::max)());

    return paths;
}

}  // namespace

void
pgr_do_trsp_withPoints(
        char *edges_sql,
        char *restrictions_sql,
        char *points_sql,
        char *edges_of_points_sql,
        char *combinations_sql,

        int64_t *start_vidsArr, size_t size_start_vidsArr,
        int64_t *end_vidsArr, size_t size_end_vidsArr,

        bool directed,
        char driving_side,
        bool details,

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
    char *hint;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = points_sql;
        auto points = pgrouting::pgget::get_points(std::string(points_sql));

        hint = edges_of_points_sql;
        auto edges_of_points = pgrouting::pgget::get_edges(std::string(edges_of_points_sql), true, false);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.size() + edges_of_points.size() == 0) {
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }

        hint = combinations_sql;
        auto combinationsArr = combinations_sql?
            pgrouting::pgget::get_combinations(std::string(combinations_sql)) : std::vector<II_t_rt>();
        hint = nullptr;

        auto combinations = combinationsArr.empty()?
            pgrouting::utilities::get_combinations(start_vidsArr, size_start_vidsArr, end_vidsArr, size_end_vidsArr)
            : pgrouting::utilities::get_combinations(combinationsArr);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        /* Dealing with points */
        pgrouting::Pg_points_graph pg_graph(points, edges_of_points,
                true,
                driving_side,
                directed);
        log << pg_graph.get_log();

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }

        auto vertices(pgrouting::extract_vertices(edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges);
            digraph.insert_edges(pg_graph.new_edges());

            paths = pgr_dijkstra(
                    digraph,
                    combinations);
        } else {
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            undigraph.insert_edges(edges);
            undigraph.insert_edges(pg_graph.new_edges());

            paths = pgr_dijkstra(
                    undigraph,
                    combinations);
        }

        post_process_trsp(paths);
        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }

        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            notice << "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        if (!restrictions_sql) {
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
        hint = restrictions_sql;
        auto restrictions = restrictions_sql?
            pgrouting::pgget::get_restrictions(std::string(restrictions_sql)) : std::vector<Restriction_t>();
        if (restrictions.empty()) {
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
        }

        std::vector<pgrouting::trsp::Rule> ruleList;
        for (const auto &r : restrictions) {
            if (r.via) ruleList.push_back(pgrouting::trsp::Rule(r));
        }
        hint = nullptr;

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        if (!new_combinations.empty()) {
            pgrouting::trsp::Pgr_trspHandler gdef(
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
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
