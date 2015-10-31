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




#include <deque>
#include <sstream>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "./ksp_driver.h"
extern "C" {
#if 0
#include "postgres.h"
#include "./ksp.h"
#include "./../../common/src/pgr_types.h"
#endif
#include "./../../common/src/postgres_connection.h"
}



#include "./pgr_ksp.hpp"



int  do_pgr_ksp(pgr_edge_t  *data_edges, int64_t total_tuples,
                       int64_t  start_vertex, int64_t  end_vertex,
                       int no_paths, bool has_reverse_cost, bool directedFlag,
                       General_path_element_t **ksp_path, size_t *path_count,
                       char ** err_msg) {
    try {
        // in c code this should have been checked:
        //  1) start_vertex is in the data_edges  DONE
        //  2) end_vertex is in the data_edges    DONE
        //  3) start and end_vertex are different DONE
        std::ostringstream log;

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;
        const int initial_size = total_tuples;

        std::deque< Path > paths;
        typedef boost::adjacency_list < boost::vecS, boost::vecS,
            boost::undirectedS,
            boost_vertex_t, boost_edge_t > UndirectedGraph;
        typedef boost::adjacency_list < boost::vecS, boost::vecS,
            boost::bidirectionalS,
            boost_vertex_t, boost_edge_t > DirectedGraph;


    if (directedFlag) {
      Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
      Pgr_ksp< Pgr_base_graph< DirectedGraph > > fn_yen;
      digraph.graph_insert_data(data_edges, initial_size);
      paths = fn_yen.Yen(digraph, start_vertex, end_vertex, no_paths);
    } else {
      Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
      Pgr_ksp< Pgr_base_graph< UndirectedGraph > > fn_yen;
      undigraph.graph_insert_data(data_edges, initial_size);
      paths = fn_yen.Yen(undigraph, start_vertex, end_vertex, no_paths);
    }
#if 0
        if (directedFlag) {
            digraph.initialize_graph(data_edges, total_tuples);
            paths = digraph.Yen(start_vertex, end_vertex, no_paths);
            digraph.clear();
        } else {
            undigraph.initialize_graph(data_edges, total_tuples);
            paths = undigraph.Yen(start_vertex, end_vertex, no_paths);
        }
#endif

        int count(count_tuples(paths));

        if (count == 0) {
            *err_msg = strdup(
               "NOTICE: No path found between Starting and Ending vertices");
            *ksp_path = noPathFound(path_count, (*ksp_path));
            return 0;
        }

        // get the space required to store all the paths
        *ksp_path = NULL;
        *ksp_path = get_memory(count, (*ksp_path));

        int sequence = 0;
        int route_id = 0;
        for (const auto &path : paths) {
            if (path.path.size() > 0)
               path.dpPrint(ksp_path, sequence, route_id);
            ++route_id;
        }

        log << "NOTICE Sequence: " << sequence << "\n";
        if (count != sequence) {
            log << "ERROR: Internal count and real count are different. \n"
                << "ERROR: This should not happen: Please report in GitHub:"
                << " pgrouting issues.";
            *err_msg = strdup(log.str().c_str());
            return -1;
        }
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


#if 0
// move around this lines to force a return with an empty path and the log msg
// cool for debugging
*err_msg = strdup(log.str().c_str());
(*path_count) = 1;
*path = noPathFound(start_vertex);
return -1;
#endif
