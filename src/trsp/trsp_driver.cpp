/*PGR-GNU*****************************************************************
File: trsp_driver.cpp

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev
* Added combinations functionality

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

#include "drivers/trsp/trsp_driver.h"

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
#include <string>

#include "trsp/trspHandler.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/rule.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/restriction_t.hpp"
#include "c_types/ii_t_rt.h"
#include "withPoints/withPoints.hpp"

#include "dijkstra/dijkstra.hpp"

namespace {

void
post_process_trsp(std::deque<pgrouting::Path> &paths, bool sort) {
    using pgrouting::Path;
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p) {
                return p.size() == 0;
                }),
            paths.end());
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }

    if (sort) {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e1.start_id() < e2.start_id();
                });
    }
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
pgr_do_trsp(
        char *edges_sql,
        char *restrictions_sql,
        char *combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        bool directed,

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

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);
        hint = nullptr;

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = to_pg_msg(edges_sql);
            return;
        }



        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);

            paths = pgr_dijkstra(
                    digraph,
                    combinations);
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);

            paths = pgr_dijkstra(
                    undigraph,
                    combinations);
        }

        post_process_trsp(paths, false);
        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            notice << "No paths found";
            *log_msg = to_pg_msg(notice);
            return;
        }

        if (!restrictions_sql) {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            (*return_count) = (collapse_paths(return_tuples, paths));
            return;
        }

        /*
         * When there are turn restrictions
         */
        hint = restrictions_sql;
        auto restrictions = pgrouting::pgget::get_restrictions(std::string(restrictions_sql));
        if (restrictions.empty()) {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            (*return_count) = (collapse_paths(return_tuples, paths));
            return;
        }

        std::vector<pgrouting::trsp::Rule> ruleList;
        for (const auto &r : restrictions) {
            if (r.via) ruleList.push_back(pgrouting::trsp::Rule(r));
        }
        hint = nullptr;

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        if (!new_combinations.empty()) {
            pgrouting::trsp::TrspHandler gdef(
                    edges,
                    directed,
                    ruleList);
            auto new_paths = gdef.process(new_combinations);
            paths.insert(paths.end(), new_paths.begin(), new_paths.end());
        }
        post_process_trsp(paths, true);

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
