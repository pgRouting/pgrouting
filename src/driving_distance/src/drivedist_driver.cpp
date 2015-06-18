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

#include "./drivedist_driver.h"
#include <sstream>

extern "C" {
#include "postgres.h"
}


#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"


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
            (*path_count) = 1;
            *ret_path = noPathFound3(start_vertex, (*ret_path));
            return 0;
        }

        log << "NOTICE: Calculating the number of tuples \n";
        int count = paths.path.size();
        int seq = 0;

        log << "NOTICE Count: " << count << " tuples\n";

        // get the space required to store all the paths
        *ret_path = NULL;
        *ret_path = pgr_get_memory3(count, (*ret_path));

        int sequence = 0;
        paths.dpPrint(ret_path, sequence, 0);

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
