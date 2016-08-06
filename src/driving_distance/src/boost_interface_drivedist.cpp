/*PGR-GNU*****************************************************************
File: boost_interface_drivedist.cpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

------

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

 ********************************************************************PGR-GNU*/

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif

#include "./boost_interface_drivedist.h"
#include <sstream>
#include <deque>
#include <vector>
#include <set>

#include "./../../common/src/pgr_types.h"
#include "../../common/src/pgr_alloc.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"




void
do_pgr_driving_many_to_dist(
        pgr_edge_t  *data_edges, size_t total_tuples,
        int64_t  *start_vertex, size_t s_len,
        double distance,
        bool directedFlag,
        bool equiCostFlag,
        General_path_element_t **ret_path, size_t *path_count,
        char ** err_msg) {
    try {
        *ret_path = NULL;
        *path_count = 0;

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;
        std::set< int64_t > s_start_vertices(start_vertex, start_vertex + s_len);
        std::vector< int64_t > start_vertices(s_start_vertices.begin(), s_start_vertices.end());

        if (directedFlag) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(data_edges, total_tuples);
            pgr_drivingDistance(digraph, paths, start_vertices, distance, equiCostFlag);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_tuples);
            pgr_drivingDistance(undigraph, paths, start_vertices, distance, equiCostFlag);
        }

        size_t count(count_tuples(paths));


        if (count == 0) {
            *err_msg = strdup("NOTICE: No return values was found");
            return;
        }
        *ret_path = pgr_alloc(count, (*ret_path));
        auto trueCount(collapse_paths(ret_path, paths));
        *path_count = trueCount;


#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
        return;
    } catch ( ... ) {
        *err_msg = strdup("Caught unknown expection!");
        if (ret_path) free(ret_path);
        *path_count = 0;
        return;
    }
}




void
do_pgr_driving_distance(
        pgr_edge_t  *data_edges, size_t total_edges,
        int64_t     start_vertex,
        double      distance,
        bool        directedFlag,
        General_path_element_t **ret_path, size_t *path_count,
        char                   **err_msg) {
    std::ostringstream log;
    try {
        // if it already has values there will be a leak
        // call with pointing to NULL
        *ret_path = NULL;
        *path_count = 0;

        log << "NOTICE: Started processing pgr_drivingDistance for 1 start_vid\n";
        // in c code this should have been checked:
        //  1) start_vertex is in the data_edges  DONE

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;

        Path path;


        if (directedFlag) {
            log << "NOTICE: Processing Directed graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(data_edges, total_edges);
            pgr_drivingDistance(digraph, path, start_vertex, distance);
        } else {
            log << "NOTICE: Processing Undirected graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(data_edges, total_edges);
            pgr_drivingDistance(undigraph, path, start_vertex, distance);
        }

        log << "Returning number of tuples" << path.size() << "\n";
        if (path.empty()) {
            log << "NOTICE: it should have at least the one for it self";
            *err_msg = strdup(log.str().c_str());
            return;
        }

        log << "NOTICE: Calculating the number of tuples \n";
        auto count = path.size();

        log << "NOTICE Count: " << count << " tuples\n";

        *ret_path = pgr_alloc(count, (*ret_path));

        size_t sequence = 0;
        path.get_pg_dd_path(ret_path, sequence);
        *path_count = count;

#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif

        return;
    } catch ( ... ) {
        log << "NOTICE: unknown exception cought";
        *err_msg = strdup(log.str().c_str());
        if (ret_path) free(ret_path);
        *path_count = 0;
        return;
    }
}

