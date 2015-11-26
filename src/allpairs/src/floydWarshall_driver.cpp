/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#define DEBUG

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
    pgr_edge_t  *data_edges,  // array of id, source, target, cost, reverse_cost
    size_t total_tuples,     // size of data_edges
    bool directedFlag,        // to choose undirected or directed

    // return values
    Matrix_cell_t **postgres_rows,
    size_t *result_tuple_count,
    char ** err_msg) {
// function starts
  std::ostringstream log;
  try {

    // for logging (usefull when developing, but useless for production)
    log << "Starting the process\n";

    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples ;
    log << "gType:" << (gType==DIRECTED? "directed": "undirected") << "\n";


    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    

    //int64_t count = 0;

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
      return ;
    }
#ifndef DEBUG  // change to 0 to send the log to main code
    *err_msg = strdup("OK");
#else
    *err_msg = strdup(log.str().c_str());
#endif
    return;

  } catch ( ... ) {
    log << "Caught unknown expection!\n";
    *err_msg = strdup(log.str().c_str());
    *postgres_rows = NULL;
    *result_tuple_count = 0;
    return;
  }
}


