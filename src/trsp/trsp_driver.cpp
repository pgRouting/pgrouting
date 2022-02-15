/*PGR-GNU*****************************************************************
File: trsp_driver.cpp

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Celia Virginia Vergara Castillo
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

#include "trsp/pgr_trspHandler.h"
#include "cpp_common/rule.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/combinations.h"
#include "c_types/restriction_t.h"
#include "c_types/ii_t_rt.h"
#include "dijkstra/pgr_dijkstra.hpp"
#include "withPoints/pgr_withPoints.hpp"


namespace {

void
post_process_trsp(std::deque<Path> &paths, bool sort) {
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
std::deque<Path>
pgr_dijkstra(
        G &graph,
        std::map<int64_t, std::set<int64_t>> &combinations
        ) {
    pgrouting::Pgr_dijkstra<G> fn_dijkstra;
    auto paths = fn_dijkstra.dijkstra(
            graph,
            combinations,
            false, (std::numeric_limits<size_t>::max)());

    return paths;
}

}  // namespace

void
do_trsp(
        Edge_t *edges, size_t total_edges,
        Restriction_t *restrictions, size_t restrictions_size,

        II_t_rt *combinations_arr, size_t total_combinations,
        int64_t *starts_arr, size_t size_starts_arr,
        int64_t *ends_arr, size_t size_ends_arr,

        bool directed,

        Path_rt **return_tuples, size_t *return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(edges);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        auto vertices(pgrouting::extract_vertices(edges, total_edges));

        auto combinations = total_combinations?
            pgrouting::utilities::get_combinations(combinations_arr, total_combinations)
            : pgrouting::utilities::get_combinations(starts_arr, size_starts_arr, ends_arr, size_ends_arr);

        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges, total_edges);

            paths = pgr_dijkstra(
                    digraph,
                    combinations);
        } else {
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            undigraph.insert_edges(edges, total_edges);

            paths = pgr_dijkstra(
                    undigraph,
                    combinations);
        }

        post_process_trsp(paths, false);
        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            notice << "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        if (restrictions_size == 0) {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            (*return_count) = (collapse_paths(return_tuples, paths));
            return;
        }

        /*
         * When there are turn restrictions
         */
        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < restrictions_size; ++i) {
            if (restrictions[i].via_size == 0) continue;
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        if (!new_combinations.empty()) {
            pgrouting::trsp::Pgr_trspHandler gdef(
                    edges,
                    total_edges,
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
