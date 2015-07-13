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

#include "./boost_interface_drivedist.h"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include "unistd.h"
#include <sstream>
#include <deque>
#include <vector>

extern "C" {
#include "postgres.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
}






int  do_pgr_driving_many_to_dist(pgr_edge_t  *data_edges, int64_t total_tuples,
                       int64_t  *start_vertex, int s_len,
                       float8 distance,
                       bool directedFlag,
                       bool equiCostFlag,
                       pgr_path_element3_t **ret_path, int *path_count,
                       char ** err_msg) {
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
            digraph.dijkstra_dd(paths, start_vertices, distance);
        } else {
            undigraph.initialize_graph(data_edges, total_tuples);
            undigraph.dijkstra_dd(paths, start_vertices, distance);
        }


        if (equiCostFlag == false) {
            int count(count_tuples(paths));
            if (count == 0) {
              *err_msg = strdup("NOTICE: No return values was found");
              *ret_path = noPathFound3(-1, path_count, (*ret_path));
              return 0;
            }
            *ret_path = pgr_get_memory3(count, (*ret_path));
            int trueCount(collapse_paths(ret_path, paths));
            *path_count = trueCount;
            // assert (count == trueCount);

        } else {
            Path path = equi_cost(paths);
            size_t count(path.size());
            if (count == 0) {
              *err_msg = strdup("NOTICE: No return values was found");
              *ret_path = noPathFound3(-1, path_count, (*ret_path));
              return 0;
            }
            int trueCount = 0;
            *ret_path = pgr_get_memory3(count, (*ret_path));
            path.dpPrint(ret_path, trueCount);
            *path_count = trueCount;
            // assert (count == trueCount);
        }

      #if 1
        *err_msg = strdup("OK");
        #else
        *err_msg = strdup(log.str().c_str());
        #endif
        return EXIT_SUCCESS;
    } catch ( ... ) {
     *err_msg = strdup("Caught unknown expection!");
     return -1;
    }
}







int  do_pgr_driving_distance(pgr_edge_t  *data_edges, int64_t total_tuples,
                       int64_t  start_vertex, float8 distance,
                       bool directedFlag,
                       pgr_path_element3_t **ret_path, int *path_count,
                       char ** err_msg) {
    try {
        // in c code this should have been checked:
        //  1) start_vertex is in the data_edges  DONE
        //  2) end_vertex is in the data_edges    DONE
        //  3) start and end_vertex are different DONE
        std::ostringstream log;

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;
        const int initial_size = 1;

        Path paths;
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
            digraph.dijkstra_dd(paths, start_vertex, distance);
        } else {
            undigraph.initialize_graph(data_edges, total_tuples);
            undigraph.dijkstra_dd(paths, start_vertex, distance);
        }

        if (paths.path.size() == 0) {
            *err_msg = strdup(
                "NOTICE: No driving distance node found");
            *ret_path = noPathFound3(-1, path_count, (*ret_path));
            return 0;
        }

        log << "NOTICE: Calculating the number of tuples \n";
        int count = paths.path.size();

        log << "NOTICE Count: " << count << " tuples\n";

        // get the space required to store all the paths
        *ret_path = NULL;
        *ret_path = pgr_get_memory3(count, (*ret_path));

        int sequence = 0;
        paths.dpPrint(ret_path, sequence, 0);
        *path_count = count;

        #if 1
        *err_msg = strdup("OK");
        #else
        *err_msg = strdup(log.str().c_str());
        #endif

        return EXIT_SUCCESS;
    } catch ( ... ) {
     *err_msg = strdup("Caught unknown expection!");
     return -1;
    }
}

#if 0
// move around this lines to force a return with an empty path and the log msg
// cool for debugging
*err_msg = strdup(log.str().c_str());
(*path_count) = 1;
*path = noPathFound(start_vertex);
return -1;
#endif
