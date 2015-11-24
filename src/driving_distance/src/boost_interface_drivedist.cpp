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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include "unistd.h"
#include <sstream>
#include <deque>
#include <vector>

#include "../../common/src/memory_func.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./boost_interface_drivedist.h"

extern "C" {
#include "./../../common/src/pgr_types.h"
}



void
do_pgr_driving_many_to_dist(
        pgr_edge_t  *data_edges, int64_t total_tuples,
        int64_t  *start_vertex, int s_len,
        float8 distance,
        bool directedFlag,
        bool equiCostFlag,
        General_path_element_t **ret_path, size_t *path_count,
        char ** err_msg) {
    try {
        // in c code this should this must have been checked:
        //  1) end_vertex is in the data_edges

        #if 0  // set to 1 if needed
        std::ostringstream log;
        #endif

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
      digraph.graph_insert_data(data_edges, total_tuples);
      pgr_drivingDistance(digraph, paths, start_vertices, distance);
    } else {
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      undigraph.graph_insert_data(data_edges, total_tuples);
      pgr_drivingDistance(undigraph, paths, start_vertices, distance);
    }

        if (equiCostFlag == false) {
            size_t count(count_tuples(paths));
            if (count == 0) {
              *err_msg = strdup("NOTICE: No return values was found");
              *ret_path = noResult(path_count, (*ret_path));
              return;
            }
            *ret_path = get_memory(count, (*ret_path));
            int trueCount(collapse_paths(ret_path, paths));
            *path_count = trueCount;
            // assert (count == trueCount);

        } else {
            Path path = equi_cost(paths);
            size_t count(path.size());
            if (count == 0) {
              *err_msg = strdup("NOTICE: No return values was found");
              *ret_path = noResult(path_count, (*ret_path));
              return ;
            }
            size_t trueCount = 0;
            *ret_path = get_memory(count, (*ret_path));
            path.generate_postgres_data(ret_path, trueCount);
            *path_count = trueCount;
            // assert (count == trueCount);
        }

      #if 1
        *err_msg = strdup("OK");
        #else
        *err_msg = strdup(log.str().c_str());
        #endif
        return;

    } catch ( ... ) {
     *err_msg = strdup("Caught unknown expection!");
     *ret_path = noResult(path_count, (*ret_path));
     return;
    }
}







void
do_pgr_driving_distance(
  pgr_edge_t  *data_edges,
  int64_t     total_edges,
  int64_t     start_vertex,
  float8      distance,
  bool        directedFlag,
  General_path_element_t **ret_path,
  size_t                  *path_count,
  char                   **err_msg) {
    std::ostringstream log;
    try {
        // if it already has values there will be a leak
        // call with pointing to NULL
        *ret_path = NULL;

        log << "NOTICE: Started processing pgr_drivingDistance for 1 start_vid\n";
        // in c code this should have been checked:
        //  1) start_vertex is in the data_edges  DONE

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;
        const int initial_size = total_edges;

        Path path;
        typedef boost::adjacency_list < boost::vecS, boost::vecS,
            boost::undirectedS,
            boost_vertex_t, boost_edge_t > UndirectedGraph;
        typedef boost::adjacency_list < boost::vecS, boost::vecS,
            boost::bidirectionalS,
            boost_vertex_t, boost_edge_t > DirectedGraph;


        if (directedFlag) {
          log << "NOTICE: Processing Directed graph\n";
          Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
          digraph.graph_insert_data(data_edges, total_edges);
          pgr_drivingDistance(digraph, path, start_vertex, distance);
        } else {
          log << "NOTICE: Processing Undirected graph\n";
          Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
          undigraph.graph_insert_data(data_edges, total_edges);
          pgr_drivingDistance(undigraph, path, start_vertex, distance);
        }

        log << "Returning number of tuples" << path.path.size() << "\n";
        if (path.path.size() == 0) {
            log << "NOTICE: it shoud have at least the one for it self";
            *err_msg = strdup(log.str().c_str());
            *ret_path = noResult(path_count, (*ret_path));
            return;
        }

        log << "NOTICE: Calculating the number of tuples \n";
        int count = path.path.size();

        log << "NOTICE Count: " << count << " tuples\n";
    
        // get the space required to store all the paths
        *ret_path = get_memory(count, (*ret_path));

        int sequence = 0;
        path.ddPrint(ret_path, sequence, 0);
        *path_count = count;

        #if 0
            *err_msg = strdup("OK");
        #else
            *err_msg = strdup(log.str().c_str());
        #endif

        return;
    } catch ( ... ) {
      log << "NOTICE: unknown exception cought";
      *err_msg = strdup(log.str().c_str());
      *ret_path = noResult(path_count, (*ret_path));
      return;
    }
}

