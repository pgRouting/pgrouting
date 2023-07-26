/*PGR-GNU*****************************************************************
File: bellman_ford_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Sourabh Garg
Mail: sourabh.garg.mat@gmail.com

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

#include "drivers/bellman_ford/bellman_ford_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>


#include "c_types/ii_t_rt.h"
#include "cpp_common/combinations.h"
#include "cpp_common/pggetdata.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "bellman_ford/pgr_bellman_ford.hpp"

namespace {

template <class G>
std::deque<pgrouting::Path>
pgr_bellman_ford(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        bool only_cost = false) {
    pgrouting::Pgr_bellman_ford<G> fn_bellman_ford;
    auto paths = fn_bellman_ford.bellman_ford(graph, combinations, only_cost);
    for (auto &p : paths) p.recalculate_agg_cost();
    return paths;
}

}  // namespace

void
pgr_do_bellman_ford(
        char *edges_sql,
        char *combinations_sql,

        int64_t *start_vidsArr, size_t size_start_vidsArr,
        int64_t *end_vidsArr, size_t size_end_vidsArr,

        bool directed,
        bool only_cost,

        Path_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char *hint;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.size() == 0) {
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }
        hint = nullptr;

        hint = combinations_sql;
        auto combinationsArr = combinations_sql?
            pgrouting::pgget::get_combinations(std::string(combinations_sql)) : std::vector<II_t_rt>();
        hint = nullptr;

        auto combinations = combinationsArr.empty()?
            pgrouting::utilities::get_combinations(start_vidsArr, size_start_vidsArr, end_vidsArr, size_end_vidsArr)
            : pgrouting::utilities::get_combinations(combinationsArr);

        std::deque<Path> paths;

        if (directed) {
            pgrouting::DirectedGraph graph(gType);
            graph.insert_edges(edges);
            paths = pgr_bellman_ford(graph,
                    combinations,
                    only_cost);
        } else {
            pgrouting::UndirectedGraph graph(gType);
            graph.insert_edges(edges);
            paths = pgr_bellman_ford(graph, combinations, only_cost);
        }

        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = (collapse_paths(return_tuples, paths));

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
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    }
}
