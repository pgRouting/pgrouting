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

#include "breadthFirstSearch/pgr_binaryBreadthFirstSearch.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

template < class G >
std::deque< Path >
pgr_binaryBreadthFirstSearch(
        G &graph,
        std::vector < int64_t > sources,
        std::vector < int64_t > targets) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    pgrouting::functions::Pgr_binaryBreadthFirstSearch< G > fn_binaryBreadthFirstSearch;
    auto paths = fn_binaryBreadthFirstSearch.binaryBreadthFirstSearch(
            graph,
            sources, targets);

    return paths;
}

const size_t MAX_UNIQUE_EDGE_COSTS = 2;
const char COST_ERR_MSG[] =  "Graph Condition Failed: Graph should have atmost two distinct non-negative edge costs! "
                             "If there are exactly two distinct edge costs, one of them must equal zero!";
template < class G >
bool
costCheck(G &graph)  {
    typedef typename G::E E;
    typedef typename G::E_i E_i;

    auto edges = boost::edges(graph.graph);
    E e;
    E_i out_i;
    E_i out_end;
    std::set<double> cost_set;
    for (boost::tie(out_i, out_end) = edges;
            out_i != out_end; ++out_i) {
        e = *out_i;
        cost_set.insert(graph[e].cost);

        if (cost_set.size() > MAX_UNIQUE_EDGE_COSTS) {
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



void
do_pgr_binaryBreadthFirstSearch(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        int64_t  *start_vidsArr,
        size_t size_start_vidsArr,
        int64_t  *end_vidsArr,
        size_t size_end_vidsArr,
        bool directed,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    try {
        pgassert(total_edges != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        // log << "Inserting vertices into a c++ vector structure";
        std::vector<int64_t>
            start_vertices(start_vidsArr, start_vidsArr + size_start_vidsArr);
        std::vector< int64_t >
            end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);


        std::deque< Path >paths;
        if (directed) {
            // log << "\nWorking with directed Graph";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);

            if (!(costCheck(digraph))) {
                err << COST_ERR_MSG;
                *err_msg = pgr_msg(err.str().c_str());
                return;
            }
            paths = pgr_binaryBreadthFirstSearch(
                digraph,
                start_vertices,
                end_vertices);

        } else {
            // log << "\nWorking with Undirected Graph";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);

            if (!(costCheck(undigraph))) {
                err << COST_ERR_MSG;
                *err_msg = pgr_msg(err.str().c_str());
                return;
            }

            paths = pgr_binaryBreadthFirstSearch(
                undigraph,
                start_vertices,
                end_vertices);
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
