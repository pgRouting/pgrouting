/*PGR-GNU*****************************************************************
File: astarOneToOne_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: 

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
#include <algorithm>
#include <vector>
#include "./astarOneToOne_driver.h"

#define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

// #include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./pgr_astar.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"

template < class G >
void
pgr_astar(
        G &graph,
        Path &path,
        int64_t source,
        int64_t target,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost = false) {
    Pgr_astar< G > fn_astar;
    fn_astar.astar(graph, path, source, target,
            heuristic, factor, epsilon, only_cost);
}


/************************************************************
  edges_sql TEXT,
  vertex_table TEXT,
  start_vid BIGINT,
  end_vid BIGINT  directed BOOLEAN DEFAULT true,
 ***********************************************************/
void do_pgr_astarOneToOne(
        Pgr_edge_xy_t  *data_edges,
        size_t total_edges,
        int64_t start_vid,
        int64_t end_vid,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream err;
    std::ostringstream log;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        if (total_edges <= 1) {
            log << "Required: more than one edge\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }


        graphType gType = directed? DIRECTED: UNDIRECTED;

        Path path;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::xyDirectedGraph digraph(gType);
            log << "Working with directed Graph 1 \n";
            digraph.graph_insert_data(data_edges, total_edges);
#ifdef DEBUG
            log << digraph;
#endif
            log << "Working with directed Graph 2\n";
            pgr_astar(digraph, path, start_vid, end_vid,
                    heuristic, factor, epsilon, only_cost);
            log << "Working with directed Graph 3\n";
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::xyUndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_edges);
#ifdef DEBUG
            log << undigraph;
#endif
            pgr_astar(undigraph, path, start_vid, end_vid,
                    heuristic, factor, epsilon, only_cost);
        }

        auto count = path.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found\n";
            *log_msg = strdup(log.str().c_str());
            return;
        }

        size_t sequence = 0;
        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        path.generate_postgres_data(return_tuples, sequence);
        (*return_count) = sequence;

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());
    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    }
}
