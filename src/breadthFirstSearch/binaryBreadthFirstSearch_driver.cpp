/*PGR-GNU*****************************************************************
File: binaryBreadthFirstSearch_driver.cpp

Generated with Template by:
Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Gudesa Venkata Sai Akhil
Mail: gvs.akhil1997@gmail.com


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

#include "drivers/breadthFirstSearch/binaryBreadthFirstSearch_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

#include "cpp_common/combinations.h"
#include "cpp_common/pgget.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "breadthFirstSearch/pgr_binaryBreadthFirstSearch.hpp"

namespace {

template < class G >
std::deque<pgrouting::Path>
pgr_binaryBreadthFirstSearch(
        G &graph,
        std::map<int64_t, std::set<int64_t>> &combinations) {
    pgrouting::functions::Pgr_binaryBreadthFirstSearch< G > fn_binaryBreadthFirstSearch;
    auto paths = fn_binaryBreadthFirstSearch.binaryBreadthFirstSearch(graph, combinations);

    return paths;
}

template < class G >
bool
costCheck(G &graph)  {
    typedef typename G::E E;
    typedef typename G::E_i E_i;

    const size_t max_unique_edge_costs = 2;
    auto edges = boost::edges(graph.graph);
    E e;
    E_i out_i;
    E_i out_end;
    std::set<double> cost_set;
    for (boost::tie(out_i, out_end) = edges;
            out_i != out_end; ++out_i) {
        e = *out_i;
        cost_set.insert(graph[e].cost);

        if (cost_set.size() > max_unique_edge_costs) {
            return false;
        }
    }

    if (cost_set.size() == 2) {
        if (*cost_set.begin() != 0.0) {
            return false;
        }
    }

    return true;
}

}  // namespace


void
pgr_do_binaryBreadthFirstSearch(
        char *edges_sql,
        char *combinations_sql,
        int64_t *start_vidsArr, size_t size_start_vidsArr,
        int64_t *end_vidsArr, size_t size_end_vidsArr,

        bool directed,

        Path_rt **return_tuples, size_t *return_count,
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
    const char c_err_msg[] = "Graph Condition Failed: Graph should have atmost two distinct non-negative edge costs! "
                             "If there are exactly two distinct edge costs, one of them must equal zero!";

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
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

        std::deque< Path >paths;
        if (directed) {
            // log << "\nWorking with directed Graph";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(edges);

            if (!(costCheck(digraph))) {
                err << c_err_msg;
                *err_msg = pgr_msg(err.str().c_str());
                return;
            }
            paths = pgr_binaryBreadthFirstSearch(digraph, combinations);

        } else {
            // log << "\nWorking with Undirected Graph";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges);

            if (!(costCheck(undigraph))) {
                err << c_err_msg;
                *err_msg = pgr_msg(err.str().c_str());
                return;
            }

            paths = pgr_binaryBreadthFirstSearch(undigraph, combinations);
        }

        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "\nConverting a set of paths into the tuples";
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
        *log_msg = pgr_msg(log.str().c_str());
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
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
