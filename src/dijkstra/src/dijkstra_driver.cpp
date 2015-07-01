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
#include "./dijkstra_driver.h"
#include <sstream>
#include <deque>
#include <vector>

extern "C" {
#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
}


// #include "./../../common/src/pgr_types.h"
// #include "./../../common/src/postgres_connection.h"
#include "./pgr_dijkstra.hpp"

int do_pgr_dijkstra_many_to_many(pgr_edge_t  *data_edges, int64_t total_tuples,
    int64_t  *start_vertex, int s_len, int64_t  *end_vertex, int e_len,
    bool has_reverse_cost, bool directedFlag,
    pgr_path_element3_t **ret_path, int *path_count,
    char ** err_msg) {
  try {
    // in c code this should this must have been checked:
    //  1) cant check anything

std::ostringstream log;
    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = 1;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    Pgr_dijkstra < DirectedGraph > digraph(gType, initial_size);
    Pgr_dijkstra < UndirectedGraph > undigraph(gType, initial_size);

    std::vector< int64_t > start_vertices(start_vertex, start_vertex + s_len);
    std::vector< int64_t > end_vertices(end_vertex, end_vertex + e_len);

    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      digraph.dijkstra(paths, start_vertices, end_vertices);
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      undigraph.dijkstra(paths, start_vertices, end_vertices);
    }

    int count(count_tuples(paths));

log << "count" << count;
    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between any of the starting vertices and any of the Ending vertices");
      (*path_count) = 1;
      *ret_path = noPathFound3(-1, (*ret_path));
      return 0;
    }


    *ret_path = pgr_get_memory3(count, (*ret_path));
    int sequence(collapse_paths(ret_path, paths));

log << "sequence" << sequence;

#if 0
    *err_msg = strdup("OK");
#else
    *err_msg = strdup(log.str().c_str());
    return -1;
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
    pgr_path_element3_t **ret_path, int *path_count,
    char **err_msg) {
  try {
    // in c code this should this must have been checked:
    //  1) end_vertex is in the data_edges

    #if 0  // set to 1 if needed
    std::ostringstream log;
    #endif

    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = 1;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    Pgr_dijkstra < DirectedGraph > digraph(gType, initial_size);
    Pgr_dijkstra < UndirectedGraph > undigraph(gType, initial_size);

    std::vector< int64_t > start_vertices(start_vertex, start_vertex + s_len);

    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      digraph.dijkstra(paths, start_vertices, end_vertex);
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      undigraph.dijkstra(paths, start_vertices, end_vertex);
    }


    int count(count_tuples(paths));

    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between any of the starting vertices and the Ending vertex");
      (*path_count) = 1;
      *ret_path = noPathFound3(-1, (*ret_path));
      return 0;
    }


    *ret_path = pgr_get_memory3(count, (*ret_path));
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
    pgr_path_element3_t **ret_path, int *path_count,
    char **err_msg) {
  try {
    // in c code this should this must have been checked:
    //  1) start_vertex is in the data_edges

    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = 1;

    std::deque< Path >paths;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    Pgr_dijkstra < DirectedGraph > digraph(gType, initial_size);
    Pgr_dijkstra < UndirectedGraph > undigraph(gType, initial_size);


    std::vector< int64_t > end_vertices(end_vertex, end_vertex + e_len);

    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      digraph.dijkstra(paths, start_vertex, end_vertices);
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      undigraph.dijkstra(paths, start_vertex, end_vertices);
    }

    int count(count_tuples(paths));

    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No paths found between Starting and any of the Ending vertices");
      (*path_count) = 1;
      *ret_path = noPathFound3(-1, (*ret_path));
      return 0;
    }


    // get the space required to store all the paths
    *ret_path = pgr_get_memory3(count, (*ret_path));
    int sequence(collapse_paths(ret_path, paths));

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











int  do_pgr_dijkstra(pgr_edge_t  *data_edges, int64_t total_tuples,
    int64_t  start_vertex, int64_t  end_vertex,
    bool has_reverse_cost, bool directedFlag,
    pgr_path_element3_t **ret_path, int *path_count,
    char **err_msg) {
  try {
    // in c code this should have been checked:
    //  1) start_vertex is in the data_edges  DONE
    //  2) end_vertex is in the data_edges  DONE
    //  3) start and end_vertex are different DONE


    graphType gType = directedFlag? DIRECTED: UNDIRECTED;
    const int initial_size = 1;

    Path path;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::undirectedS,
      boost_vertex_t, boost_edge_t > UndirectedGraph;
    typedef boost::adjacency_list < boost::vecS, boost::vecS,
      boost::bidirectionalS,
      boost_vertex_t, boost_edge_t > DirectedGraph;

    Pgr_dijkstra < DirectedGraph > digraph(gType, initial_size);
    Pgr_dijkstra < UndirectedGraph > undigraph(gType, initial_size);

    if (directedFlag) {
      digraph.initialize_graph(data_edges, total_tuples);
      digraph.dijkstra(path, start_vertex, end_vertex);
    } else {
      undigraph.initialize_graph(data_edges, total_tuples);
      undigraph.dijkstra(path, start_vertex, end_vertex);
    }

    int count(path.path.size());

    if (count == 0) {
      *err_msg = strdup(
        "NOTICE: No path found between Starting and Ending vertices");
      (*path_count) = 1;
      *ret_path = noPathFound3(-1, (*ret_path));
      return 0;
    }


    int seq = 0;


    // get the space required to store all the paths
    *ret_path = NULL;
    *ret_path = pgr_get_memory3(count, (*ret_path));

    int sequence = 0;
    path.dpPrint(ret_path, sequence);

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
(*path_count) = 1;
*ret_path = noPathFound3(start_vertex, (*ret_path));
return -1;
#endif

