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

// #define DEBUG

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <vector>
#include "./function1_driver.h"

extern "C" {
#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
}


static int do_pgr_function1(
    pgr_edge_t  *data_edges,  // array of id, source, target, cost, reverse_cost
	int64_t total_tuples,     // size of data_edges

    int64_t  *start_vertex,   // array of id
    int s_len,                // size of array

    int64_t  start_vertex,    // id

    int64_t  *end_vertex,     // array of id
    int e_len,                // size of array

    int64_t  end_vertex,      // id

    bool directedFlag,        // to choose undirected or directed

    // return values
    pgr_path_element3_t **ret_path,
    int *path_count,
    char ** err_msg
                    ) {
  try {

    // maybe there is a minimum requirement to execute the algorithm
    if (total_tuples == 1) {
      *ret_path = noPathFound3(-1, path_count, (*ret_path));
      *ret_path = NULL;
      return 0;
    }

    // for logging (usefull when developing, but useless for production)
    std::ostringstream log;
    log << "My log message 1";

    log << "My log message 2";
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    // example using dijkstra  (must have something like src/dijkstra/src/pgr_dijkstra.hpp)
    Pgr_dijkstra < DirectedGraph > digraph(gType, initial_size);
    Pgr_dijkstra < UndirectedGraph > undigraph(gType, initial_size);
    
#if 0    // if the start_vertex and end vertex are arrays use this:
    std::vector< int64_t > start_vertices(start_vertex, start_vertex + s_len);
    std::vector< int64_t > end_vertices(end_vertex, end_vertex + e_len);

    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      digraph.dijkstra(paths, start_vertices, end_vertices);
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      undigraph.dijkstra(paths, start_vertices, end_vertices);
    }
#else
    // this shows a combination
    std::vector< int64_t > start_vertices(start_vertex, start_vertex + s_len);

    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      digraph.dijkstra(paths, start_vertices, end_vertex);
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      undigraph.dijkstra(paths, start_vertices, end_vertex);
    }
#endif

    int count(count_tuples(paths));


    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between any of the starting vertices and any of the Ending vertices");
      *ret_path = noPathFound3(-1, path_count, (*ret_path));
      return 0;
    }


    *ret_path = pgr_get_memory3(count, (*ret_path)); // getting memory to store the result
    int sequence(collapse_paths(ret_path, paths));   // store the result


#if 1  // change to 0 to send the log to main code
    *err_msg = strdup("OK");
#else
    *err_msg = strdup(log.str().c_str());
#endif
    *path_count = sequence;
    return EXIT_SUCCESS;
  } catch ( ... ) {
    *err_msg = strdup("Caught unknown expection!");
    return -1;
  }
}


