/*PGR-GNU*****************************************************************
File: bdDijkstra_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#include "./bdDijkstra_driver.h"


#include "./../../common/src/pgr_alloc.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_types.h"

#include "./pgr_bdDijkstra.hpp"
#include "./../../common/src/pgr_base_graph.hpp"




/************************************************************
  edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
 ***********************************************************/

template < class G >
static
Path
pgr_bdDijkstra(
        G &graph,
        int64_t source,
        int64_t target,
        std::ostream &log,
        bool only_cost = false) {
    log << "entering static function\n";
    Pgr_bdDijkstra<G> fn_bdDijkstra(graph);

    auto path = fn_bdDijkstra.pgr_bdDijkstra(graph, graph.get_V(source), graph.get_V(target), only_cost); 
    log << fn_bdDijkstra.log();

    return path;
}


void
do_pgr_bdDijkstra(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        int64_t start_vid,
        int64_t end_vid,
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
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        Path path;

        log << "starting process\n";
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            path = pgr_bdDijkstra(digraph,
                    start_vid,
                    end_vid,
                    log,
                    only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            path = pgr_bdDijkstra(
                    undigraph,
                    start_vid,
                    end_vid,
                    log,
                    only_cost);
        }

        auto count = path.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found between start_vid and end_vid vertices";
        } else {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            size_t sequence = 0;
            path.generate_postgres_data(return_tuples, sequence);
            (*return_count) = sequence;
        }

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
    } catch (std::exception& except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
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
