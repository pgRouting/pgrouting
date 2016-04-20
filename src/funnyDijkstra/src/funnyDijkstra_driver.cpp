/*PGR-GNU*****************************************************************
File: funnyDijkstra_driver.cpp

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


#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>
#include "./pgr_dijkstra.hpp"
#include "./funnyDijkstra_driver.h"

#define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"

/************************************************************
  edges_sql TEXT,
    start_pid BIGINT,
    end_pids ANYARRAY,
    directed BOOLEAN DEFAULT true,
 ***********************************************************/
void
do_pgr_funnyDijkstra(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        int64_t start_vid,
        int64_t  *end_vidsArr,
        size_t size_end_vidsArr,
        bool directed,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg){
    std::ostringstream log;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        if (total_tuples <= 1) {
            log << "Required: more than one tuple\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;
        log << "Inserting vertices into a c++ vector structure\n";
        std::vector< int64_t > end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);
        std::sort(end_vertices.begin(),end_vertices.end());
#ifdef DEBUG
        log << "end vids: ";
        for (const auto &vid : end_vertices) log << vid << ",";
        log << "\nstart vid:" << start_vid << "\n";
#endif
        if (directed) {
            log << "Working with directed Graph\n";
            pgRouting::graph::Pgr_base_graph< DirectedGraph > digraph(gType);
            log << "Working with directed Graph 1 \n";
            digraph.graph_insert_data(data_edges, total_tuples);
#ifdef DEBUG
            log << digraph;
#endif
            log << "Working with directed Graph 2\n";
            pgr_dijkstra(digraph, paths, start_vid, end_vertices, false);
            log << "Working with directed Graph 3\n";
        } else {
            log << "Working with Undirected Graph 4\n";
            pgRouting::graph::Pgr_base_graph< UndirectedGraph > undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_tuples);
#ifdef DEBUG
            log << undigraph;
#endif
            pgr_dijkstra(undigraph, paths, start_vid, end_vertices, false);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log << 
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return;
        }

        // get the space required to store all the paths
        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());

    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
    }
}
