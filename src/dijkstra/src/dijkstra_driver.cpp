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
#include "./pgr_dijkstra.hpp"
#include "./dijkstra_driver.h"

extern "C" {
#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
}


// #include "./../../common/src/pgr_types.h"
// #include "./../../common/src/postgres_connection.h"


int do_pgr_dijkstra_many_to_many(pgr_edge_t  *data_edges, int64_t total_tuples,
    int64_t  *start_vertex, int s_len, int64_t  *end_vertex, int e_len,
    bool has_reverse_cost, bool directedFlag,
    General_path_element_t **ret_path, size_t *path_count,
    char ** err_msg) {
  try {
    // in c code this should this must have been checked:
    //  1) cant check anything
    if (total_tuples == 1) {
      *ret_path = noPathFound(path_count, (*ret_path));
      return 0;
    }

std::ostringstream log;
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    const int initial_size = total_tuples;

    std::vector< int64_t > start_vertices(start_vertex, start_vertex + s_len);
    std::vector< int64_t > end_vertices(end_vertex, end_vertex + e_len);
    if (directedFlag) {
      Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< DirectedGraph > > fn_dijkstra;
      digraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(digraph, paths, start_vertices, end_vertices);
    } else {
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< UndirectedGraph > > fn_dijkstra;
      undigraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(undigraph, paths, start_vertices, end_vertices);
    }

    int count(count_tuples(paths));

log << "count" << count;
    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between any of the starting vertices and any of the Ending vertices");
      *ret_path = noPathFound(path_count, (*ret_path));
      return 0;
    }


    *ret_path = get_memory(count, (*ret_path));
    int sequence(collapse_paths(ret_path, paths));

log << "sequence" << sequence;

#if 1
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


















int  do_pgr_dijkstra_many_to_1(pgr_edge_t *data_edges, int64_t total_tuples,
    int64_t *start_vertex, int s_len, int64_t end_vertex,
    bool has_reverse_cost, bool directedFlag,
    General_path_element_t **ret_path, size_t *path_count,
    char **err_msg) {
  try {
    // in c code this should this must have been checked:
    //  1) end_vertex is in the data_edges

    #if 0  // set to 1 if needed
    std::ostringstream log;
    #endif
    if (total_tuples == 1) {
      *ret_path = noPathFound(path_count, (*ret_path));
      *ret_path = NULL;
      return 0;
    }

    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    std::vector< int64_t > start_vertices(start_vertex, start_vertex + s_len);

    if (directedFlag) {
      Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< DirectedGraph > > fn_dijkstra;
      digraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(digraph, paths, start_vertices, end_vertex);
    } else {
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< UndirectedGraph > > fn_dijkstra;
      undigraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(undigraph, paths, start_vertices, end_vertex);
    }


    int count(count_tuples(paths));

    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between any of the starting vertices and the Ending vertex");
      *ret_path = noPathFound(path_count,  (*ret_path));
      return 0;
    }


    *ret_path = get_memory(count, (*ret_path));
    int sequence(collapse_paths(ret_path, paths));


    #if 1
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












int do_pgr_dijkstra_1_to_many(pgr_edge_t  *data_edges, int64_t total_tuples,
    int64_t start_vertex, int64_t *end_vertex, int e_len,
    bool has_reverse_cost, bool directedFlag,
    General_path_element_t **ret_path, size_t *path_count,
    char **err_msg) {
  try {
    // in c code this should this must have been checked:
    //  1) start_vertex is in the data_edges

    if (total_tuples == 1) {
      *ret_path = noPathFound(path_count, (*ret_path));
      return 0;
    }
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;



    std::vector< int64_t > end_vertices(end_vertex, end_vertex + e_len);

    if (directedFlag) {
      Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< DirectedGraph > > fn_dijkstra;
      digraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(digraph, paths, start_vertex, end_vertices);
    } else {
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< UndirectedGraph > > fn_dijkstra;
      undigraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(undigraph, paths, start_vertex, end_vertices);
    }

    int count(count_tuples(paths));

    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between Starting and any of the Ending vertices");
      *ret_path = noPathFound(path_count, (*ret_path));
      return 0;
    }


    // get the space required to store all the paths
    *ret_path = get_memory(count, (*ret_path));
    int sequence(collapse_paths(ret_path, paths));

    #if 1
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











int  do_pgr_dijkstra(pgr_edge_t  *data_edges, int64_t total_tuples,
    int64_t  start_vertex, int64_t  end_vertex,
    bool has_reverse_cost, bool directedFlag,
    General_path_element_t **ret_path, size_t *path_count,
    char **err_msg) {
  try {
    // in c code this should have been checked:
    //  1) start_vertex is in the data_edges  DONE
    //  2) end_vertex is in the data_edges  DONE
    //  3) start and end_vertex are different DONE

    if (total_tuples == 1) {
      *ret_path = noPathFound(path_count, (*ret_path));
      return 0;
    }
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = total_tuples;

    Path path;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;


    size_t count = 0;
    if (directedFlag) {
      Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
      digraph.graph_insert_data(data_edges, total_tuples);
      //Pgr_dijkstra< Pgr_base_graph< DirectedGraph > > fn_dijkstra;
      pgr_dijkstra(digraph, path, start_vertex, end_vertex);
    } else {
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      Pgr_dijkstra< Pgr_base_graph< UndirectedGraph > > fn_dijkstra;
      undigraph.graph_insert_data(data_edges, total_tuples);
      fn_dijkstra.dijkstra(undigraph, path, start_vertex, end_vertex);
    }

    count = path.path.size();

    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No path found between Starting and Ending vertices");
      *ret_path = noPathFound(path_count, (*ret_path));
      return 0;
    }


  
    // get the space required to store all the paths
    *ret_path = NULL;
    *ret_path = get_memory(count, (*ret_path));

    size_t sequence = 0;
    path.generate_postgres_data(ret_path, sequence);

    #if 1
    *err_msg = strdup("OK");
    #else
    *err_msg = strdup(log.str().c_str());
    #endif
    *path_count = count;
    return EXIT_SUCCESS;
  } catch ( ... ) {
    *err_msg = strdup("Caught unknown expection!");
    return -1;
  }
}

#if 0  // used for debugging
std::ostringstream log;
// move around this lines to force a return with an empty path and the log msg
// cool for debugging
if 0
*err_msg = strdup(log.str().c_str());
(*path_count) = 0;
*ret_path = noPathFound(start_vertex, (*ret_path));
return -1;
#endif

