/*PGR-GNU*****************************************************************
File: johnson_driver.cpp

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
#include "./johnson_driver.h"

// #define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

// CREATE OR REPLACE FUNCTION pgr_johnson(edges_sql TEXT, directed BOOLEAN,
void
do_pgr_johnson(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        bool directed,
        Matrix_cell_t **return_tuples,
        size_t *return_count,
        char **err_msg) {
    std::ostringstream log;
    try {
        if (total_tuples == 1) {
            log << "Required: more than one tuple\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(data_edges, total_tuples);
            pgr_johnson(digraph, *return_count, return_tuples);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_tuples);
            pgr_johnson(undigraph, *return_count, return_tuples);
        }

        if (*return_count == 0) {
            log <<  "NOTICE: No Vertices found??? wiered error\n";
            *err_msg = strdup(log.str().c_str());
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
    } catch ( ... ) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
    }
}
