/*PGR-GNU*****************************************************************
File: floydWarshall_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
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
#ifdef open
#undef open
#endif
#endif


#include <sstream>
#include <deque>
#include <vector>
#include "./pgr_allpairs.hpp"
#include "./floydWarshall_driver.h"
#include "./../../common/src/pgr_assert.h"

extern "C" {
#include "./../../common/src/pgr_types.h"
}


void
do_pgr_floydWarshall(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        bool directedFlag,

        // return values
        Matrix_cell_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg) {
    // function starts
    std::ostringstream log;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;


        if (directedFlag) {
            log << "Processing Directed graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(data_edges, total_tuples);
            log << digraph;
            pgr_floydWarshall(digraph, *return_count, return_tuples);
        } else {
            log << "Processing Undirected graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_tuples);
            log << undigraph;
            pgr_floydWarshall(undigraph, *return_count, return_tuples);
        }


        if (*return_count == 0) {
            log <<  "NOTICE: No Vertices found??? wiered error\n";
            *err_msg = strdup(log.str().c_str());
            *return_tuples = NULL;
            *return_count = 0;
            return;
        }

        *log_msg = strdup(log.str().c_str());
        return;
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
