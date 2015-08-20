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
#include "./warshall_driver.h"
#include "./pgr_warshall.hpp"

extern "C" {
#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
}


int do_pgr_warshall(
    pgr_edge_t  *data_edges,  // array of id, source, target, cost, reverse_cost
    int64_t total_tuples,     // size of data_edges
    bool directedFlag,        // to choose undirected or directed

    // return values
    path_element_t **ret_matrix,
    int *path_count,
    char ** err_msg) {
// function starts
  std::ostringstream log;
  try {

    // for logging (usefull when developing, but useless for production)
    log << "My log message 1";

    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples;
    log << "My log message 2";

    // where we store the results
    //std::vector< path_element_t > Dmatrix;
    ////////

    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    // example using dijkstra  (must have something like src/dijkstra/src/pgr_dijkstra.hpp)
    Pgr_warshall < DirectedGraph > digraph(gType, initial_size);
    Pgr_warshall < UndirectedGraph > undigraph(gType, initial_size);
    

    int64_t count = 0;
#if 1
    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      log << "directed graph initialized";
//      digraph.warshall(ret_matrix, count);  // not working yet
      log << "directed finished";
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      log << "undirected graph initialized";
//      undigraph.warshall(ret_matrix, count);
      log << "directed finished";
    }
#endif


    if (count == 0) {
      //*err_msg = strdup( "NOTICE: No Vertices found??? wiered error");
      *err_msg = strdup(log.str().c_str());
      *ret_matrix = NULL;
      return -1;
    }





#if 0  // change to 0 to send the log to main code
    *err_msg = strdup("OK");
#else
    *err_msg = strdup(log.str().c_str());
#endif

    *path_count = count;
    return EXIT_SUCCESS;
  } catch ( ... ) {
    //*err_msg = strdup("Caught unknown expection!");
    *err_msg = strdup(log.str().c_str());
    return -1;
  }
}


