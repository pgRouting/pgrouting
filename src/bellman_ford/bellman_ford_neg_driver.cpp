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

#include "drivers/bellman_ford/bellman_ford_neg_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>

#include "bellman_ford/pgr_bellman_ford.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"


/* Bellman Ford Shortest Path */
/************************************************************
    EDGE_SQL,
    ANYARRAY,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,
 ***********************************************************/

template < class G >
std::deque< Path >
pgr_bellman_ford(
        G &graph,
        std::vector < int64_t > sources,
        std::vector < int64_t > targets,
        std::string &log,
        bool only_cost = false) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    Pgr_bellman_ford< G > fn_bellman_ford;
    auto paths = fn_bellman_ford.bellman_ford(
            graph, sources, targets, only_cost);
    log += fn_bellman_ford.get_log();
    return paths;
}

void
do_pgr_bellman_ford_neg(
                pgr_edge_t  *positive_edges,
                size_t total_positive_edges,
                pgr_edge_t  *negative_edges,
                size_t total_negative_edges,
                int64_t  *start_vidsArr,
                size_t size_start_vidsArr,
                int64_t  *end_vidsArr,
                size_t size_end_vidsArr,
                bool directed,
                bool only_cost,

                General_path_element_t **return_tuples,
                size_t *return_count,
                char ** log_msg,
                char ** notice_msg,
                char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    size_t total_edges =  total_positive_edges + total_negative_edges;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        log << "Inserting vertices into a c++ vector structure";

        std::vector<int64_t>
            start_vertices(start_vidsArr, start_vidsArr + size_start_vidsArr);
        std::vector< int64_t >
            end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);

        std::deque< Path >paths;
        std::string logstr;
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(positive_edges, total_positive_edges);
            digraph.insert_negative_edges(negative_edges, total_negative_edges);
            log << digraph;
            paths = pgr_bellman_ford(digraph,
                    start_vertices,
                    end_vertices,
                    logstr,
                    only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(positive_edges, total_positive_edges);
            undigraph.insert_negative_edges(
                    negative_edges,
                    total_negative_edges);
            log << undigraph;
            paths = pgr_bellman_ford(
                    undigraph,
                    start_vertices,
                    end_vertices,
                    logstr,
                    only_cost);
        }
        log<< logstr;
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
