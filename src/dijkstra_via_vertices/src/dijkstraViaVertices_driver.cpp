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
#include "./dijkstraViaVertices_driver.h"
#include "../../dijkstra/src/pgr_dijkstra.hpp"

extern "C" {
#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
}


int  do_pgr_dijkstra_via_vertices(

    pgr_edge_t  *data_edges,  //<! array of id, source, target, cost, reverse_cost
    int64_t total_tuples,     //<! size of data_edges

    int64_t  *via_vertex,     //<! array of vertex id
    int v_len,                //<! size of array

    bool directedFlag,        //<! to choose undirected or directed

    // return values
    pgr_path_element3_t **ret_path,
    int *path_count,
    char ** err_msg) {
  // function starts
  try {
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    // maybe there is a minimum requirement to execute the algorithm (that coulnt be checked in the c code)
    if (total_tuples == 1) {
      *ret_path = noPathFound3(-1, path_count, (*ret_path));
      *ret_path = NULL;
      return 0;
    }

    std::vector< int64_t > via_vertices(via_vertex, via_vertex + v_len);

    // for logging (usefull when developing, but useless for production)
    std::ostringstream log;
    log << "Created log variable\n";

    log << "Getting the type of graph\n";
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples;

    log << "Creating the container for results\n";
    std::deque< Path >paths;

    if (directedFlag) {
      log << "Working with directed graph";

      Pgr_dijkstra < DirectedGraph > digraph(gType, initial_size);
      log << "   Directed graph created";
      digraph.initialize_graph(data_edges, total_tuples);
      log << "   Inserted edges";
      digraph.dijkstra(paths, via_vertices);
      log << "   Obtained paths";
    } else {
      log << "Working with undirected graph";

      Pgr_dijkstra < UndirectedGraph > undigraph(gType, initial_size);
      log << "   Directed graph created";
      undigraph.initialize_graph(data_edges, total_tuples);
      log << "   Inserted edges";
      undigraph.dijkstra(paths, via_vertices);
      log << "   Obtained paths";
    }

    int count(count_tuples(paths));
    if (count == 0 || (paths.size() != size_t(v_len - 1)) ) {
      *err_msg = strdup("NOTICE: No Route found");
      *ret_path = noPathFound3(-1, path_count, (*ret_path));
      return 0;
    }


    *ret_path = pgr_get_memory3(count, (*ret_path)); // getting memory to store the result
    int sequence(collapse_paths(ret_path, paths));   // store the result

    //update tot_cost, and seq
    for (int i = 1; i < sequence; ++i) {
	(*ret_path)[i].tot_cost = (*ret_path)[i-1].tot_cost + (*ret_path)[i-1].cost;
	(*ret_path)[i].seq = i+1;
    }



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


