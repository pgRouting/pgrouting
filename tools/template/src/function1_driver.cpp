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

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <sstream>
#include <deque>
#include <vector>

#include "./MY_FUNCTION_NAME_driver.h"


#include "../../common/src/pgr_alloc.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_types.h"

#include "./../../dijkstra/src/pgr_dijkstra.hpp"




/************************************************************
  MY_QUERY_LINE1
 ***********************************************************/

template < class G >
static
Path
pgr_MY_FUNCTION_NAME(
        G &graph,
        int64_t source,
        int64_t target,
        bool only_cost = false) {
    Path path;
    Pgr_dijkstra< G > fn_dijkstra;
    fn_dijkstra.dijkstra(graph, path, source, target, only_cost);
    return path;
}


void
do_pgr_MY_FUNCTION_NAME(
        MY_EDGE_TYPE  *data_edges,
        size_t total_edges,
        int64_t start_vid,
        int64_t end_vid,
        bool directed,
        bool only_cost,
        MY_RETURN_VALUE_TYPE **return_tuples,
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

        Path path;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(data_edges, total_edges);
            path = pgr_MY_FUNCTION_NAME(digraph,
                    start_vid,
                    end_vid,
                    only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_edges);
            path = pgr_MY_FUNCTION_NAME(
                    undigraph,
                    start_vid,
                    end_vid,
                    only_cost);
        }

        auto count = path.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found between start_vid and end_vid vertices";
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        size_t sequence = 0;
        path.generate_postgres_data(return_tuples, sequence);
        (*return_count) = sequence;

        pgassert(*err_msg == NULL);
        *log_msg = log.str().empty()?
            nullptr :
            strdup(log.str().c_str());
        *notice_msg = notice.str().empty()?
            nullptr :
            strdup(notice.str().c_str());
    } catch (AssertFailedException &except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception &except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << execpt.what();
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
