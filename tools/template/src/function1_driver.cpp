/*PGR-GNU*****************************************************************
File: MY_FUNCTION_NAME_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) YEAR DEVELOPER_NAME
Mail: DEVELOPER_EMAIL

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


#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif


#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>
#include "./pgr_dijkstra.hpp"
#include "./MY_FUNCTION_NAME_driver.h"

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"

/************************************************************
  MY_QUERY_LINE1
 ***********************************************************/
void
do_pgr_MY_FUNCTION_NAME(
        MY_EDGE_TYPE  *data_edges, size_t total_edges,
        int64_t start_vid,
        int64_t  *end_vidsArr, size_t size_end_vidsArr,
        bool directed,
        MY_RETURN_VALUE_TYPE **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg){
    std::ostringstream err;
    std::ostringstream log;
    std::ostringstream notice;
    try {
        
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        /* depending on the functionality some tests can be done here
         * For example */
        if (total_edges <= 1) {
            err << "Required: more than one edges\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(err.str().c_str());
            return;
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;
        // samll logs
        log << "Inserting vertices into a c++ vector structure\n";
        std::vector< int64_t > end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);
        std::sort(end_vertices.begin(),end_vertices.end());
#ifndef NDEBUG
        // big logs with cycles wrap them so on release they wont consume time
        log << "end vids: ";
        for (const auto &vid : end_vertices) log << vid << ",";
        log << "\nstart vid:" << start_vid << "\n";
#endif

        if (directed) {
            // very detailed logging
            log << "Working with directed Graph\n";
            pgRouting::DirectedGraph digraph(gType);
            log << "Working with directed Graph 1 \n";
            digraph.graph_insert_data(data_edges, total_edges);

#ifndef NDEBUG
            // a graph log is a big log
            log << digraph;
#endif
            
            log << "Working with directed Graph 2\n";
            pgr_dijkstra(digraph, paths, start_vid, end_vertices, false);
            log << "Working with directed Graph 3\n";
        } else {
            // maybe the code is working so cleaner logging
            log << "Working with Undirected Graph\n";
            pgRouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_edges);
            pgr_dijkstra(undigraph, paths, start_vid, end_vertices, false);
        }

        // use auto when possible
        auto count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log << 
                "No paths found between Starting and any of the Ending vertices\n";
            *log_msg = strdup(log.str().c_str());
            return;
        }

        // get the space required to store all the paths
        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));
#if 0
        // this is when its only one path
        path.generate_postgres_data(return_tuples, sequence);
#endif

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());
        *notice_msg = strdup(notice.str().c_str());

    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what();
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what();
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    }
}
