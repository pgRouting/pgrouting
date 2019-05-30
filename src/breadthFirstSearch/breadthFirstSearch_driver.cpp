/*PGR-GNU*****************************************************************
File: breadthFirstSearch_driver.cpp

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

#include "drivers/breadthFirstSearch/breadthFirstSearch_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>

#if 0
#include "breadthFirstSearch/pgr_breadthFirstSearch.hpp"
#endif

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

//TODO(vicennial) : Remove below headers once pgr_breadthFirstSearch.hpp is implemented.
#include "cpp_common/pgr_messages.h"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
//TODO(vicennial) : Complete file once pgr_breadthFirstSearch.hpp is implemented. 
//TODO(vicennial) : Use parameter max_depth.

/* Breadth First Search Shortest Path */
/************************************************************
    EDGE_SQL,
    ANYARRAY,
    ANYARRAY,
    directed BOOLEAN DEFAULT true,
 ***********************************************************/

#if 0
template < class G >
std::deque< Path >
pgr_breadthFirstSearch(
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

    Pgr_breadthFirstSearch< G > fn_breadthFirstSearch;
    auto paths = fn_breadthFirstSearch.breadthFirstSearch(
            graph, sources, targets, only_cost);
    log += fn_breadthFirstSearch.get_log();
    return paths;
}
#endif

void
do_pgr_breadthFirstSearch(
        pgr_edge_t  *data_edges,
                size_t total_edges,
                int64_t  *start_vidsArr,
                size_t size_start_vidsArr,
                int64_t  max_depth,
                bool directed,

                pgr_mst_rt **return_tuples,
                size_t *return_count,
                char ** log_msg,
                char ** notice_msg,
                char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
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

        std::deque< Path >paths;
        std::string logstr;
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            
            #if 0
            paths = pgr_breadthFirstSearch(digraph,
                    start_vertices,
                    end_vertices,
                    logstr,
                    only_cost);
            #endif

        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);

            #if 0
            paths = pgr_breadthFirstSearch(
                    undigraph,
                    start_vertices,
                    end_vertices,
                    logstr,
                    only_cost);
            #endif

        }
        #if 0 //TODO (vicennial): Figure out how to process return tuples
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
        #endif
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
