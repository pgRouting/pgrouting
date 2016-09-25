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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <vector>
#include "./pgr_allpairs.hpp"
#include "./floydWarshall_driver.h"

extern "C" {
#include "./../../common/src/pgr_types.h"
}


void
do_pgr_floydWarshall(
    pgr_edge_t  *data_edges,
    size_t total_tuples,
    bool directedFlag,

    // return values
    Matrix_cell_t **postgres_rows,
    size_t *result_tuple_count,
    char ** err_msg) {
// function starts
  std::ostringstream log;
  try {
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const size_t initial_size = total_tuples;


    if (directedFlag) {
      log << "Processing Directed graph\n";
      Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
      digraph.graph_insert_data(data_edges, total_tuples);
      pgr_floydWarshall(digraph, *result_tuple_count, postgres_rows);
    } else {
      log << "Processing Undirected graph\n";
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      undigraph.graph_insert_data(data_edges, total_tuples);
      pgr_floydWarshall(undigraph, *result_tuple_count, postgres_rows);
    }


    if (*result_tuple_count == 0) {
      log <<  "NOTICE: No Vertices found??? wiered error\n";
      *err_msg = strdup(log.str().c_str());
      *postgres_rows = NULL;
      *result_tuple_count = 0;
      return;
    }
#ifndef DEBUG
    *err_msg = strdup("OK");
#else
    *err_msg = strdup(log.str().c_str());
#endif
    return;
  } catch ( ... ) {
    log << "Caught unknown exception!\n";
    *err_msg = strdup(log.str().c_str());
    *postgres_rows = NULL;
    *result_tuple_count = 0;
    return;
  }
}


