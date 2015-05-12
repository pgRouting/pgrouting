/*PGR

file: KSPDriver.cpp

Copyright (c) 2013 Dave Potts
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

#include "./dijkstra_driver.h"

extern "C" {
#include "postgres.h"
// #include "./dijkstra.h"
}

#include <deque>
#include <sstream>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "./pgr_dijkstra.hpp"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"


static  void dpPrint(
                     const Path &thePath,
                     pgr_path_element3_t **path,
                     int &sequence, int route_id, std::ostream &log);

//static  pgr_path_element3_t * noPathFound(int64_t start_id);

int  do_pgr_dijkstra(pgr_edge_t  *data_edges, int64_t total_tuples,
                       int64_t  start_vertex, int64_t  end_vertex,
                       bool has_reverse_cost, bool directedFlag,
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
            digraph.process_dijkstra(paths, start_vertex, end_vertex);
            // digraph.clear();
        } else {
            undigraph.initialize_graph(data_edges, total_tuples);
            undigraph.process_dijkstra(paths, start_vertex, end_vertex);
            // undigraph.clear();
        }

        if (paths.path.size() == 0) {
            *err_msg = strdup("NOTICE: No path found between Starting and Ending vertices");
            (*path_count) = 1;
            *ret_path = noPathFound3(start_vertex);
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
        dpPrint(paths, ret_path, sequence, 0, log);

#if 0
// move around this lines to force a return with an empty path and the log msg
// cool for debugging
*err_msg = strdup(log.str().c_str());
(*path_count) = 1;
*path = noPathFound(start_vertex);
return -1;
#endif
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

static void dpPrint(
                     const Path &thePath,
                     pgr_path_element3_t **path,
                     int &sequence, int route_id, std::ostream &log ) {
        // the row data:  seq, route, nodeid, edgeId, cost
        int64_t nodeId, edgeId, lastNodeId;
        double cost;

        for (unsigned int i = 0; i < thePath.path.size(); i++) {
                edgeId = thePath.path[i].edge;
                nodeId = thePath.path[i].source;
                cost = thePath.path[i].cost;

               (*path)[sequence].route_id = route_id;
               (*path)[sequence].vertex_id = nodeId;
               (*path)[sequence].edge_id = edgeId;
               (*path)[sequence].cost = cost;
               sequence++;
        }
}

#if 0
static  pgr_path_element3_t * noPathFound(int64_t start_id) {
        pgr_path_element3_t *no_path;
        no_path = pgr_get_memory3(1, no_path);
        no_path[0].route_id  = 0;
        no_path[0].vertex_id = start_id;
        no_path[0].cost = 0;
        no_path[0].edge_id = -1;
        return no_path;
}
#endif

