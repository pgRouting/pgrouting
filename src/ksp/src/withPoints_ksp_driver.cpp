/*PGR-GNU*****************************************************************
File: withPoints_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: 

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


#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <deque>
#include <vector>

#include "./../../common/src/memory_func.hpp"
#include "./pgr_ksp.hpp"
#include "./../../withPoints/src/pgr_withPoints.h"
#include "./withPoints_ksp_driver.h"

// #define DEBUG

// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


int
do_pgr_withPointsKsp(
        pgr_edge_t  *edges,
        size_t total_edges,
        Point_on_edge_t  *points_p,
        size_t total_points,
        pgr_edge_t  *edges_of_points,
        size_t total_edges_of_points,
        int64_t start_pid,
        int64_t end_pid,
        int64_t k,
        bool directed,
        bool heap_paths,
        char driving_side,
        bool details,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg){
    std::ostringstream log;
    try {
        /*
         * This is the original state
         */
        if (*err_msg) free(err_msg);
        if (*return_tuples) free(return_tuples);
        (*return_count) = 0;

        std::vector< Point_on_edge_t >
            points(points_p, points_p + total_points);

        int errcode = check_points(points, log);
        if (errcode) {
            return errcode;
        }


        std::vector< pgr_edge_t >
            edges_to_modify(edges_of_points, edges_of_points + total_edges_of_points);

        std::vector< pgr_edge_t > new_edges;
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                new_edges,
                log);

        int64_t start_vid = 0;
        int64_t end_vid = 0;
        for (const auto point : points) {
            if (point.pid == start_pid) {
                start_vid = point.vertex_id;
            }
            if (point.pid == end_pid) {
                end_vid = point.vertex_id;
            }
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;
        const int initial_size = total_edges;

        std::deque< Path > paths;

        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            Pgr_ksp< Pgr_base_graph< DirectedGraph > > fn_yen;
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            paths = fn_yen.Yen(digraph, start_vid, end_vid, k, heap_paths);
        } else {
            log << "Working with undirected Graph\n";
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            Pgr_ksp< Pgr_base_graph< UndirectedGraph > > fn_yen;
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            paths = fn_yen.Yen(undigraph, start_vid, end_vid, k, heap_paths);
        }


        for (auto &path : paths) {
            adjust_pids(points, path);
        }

        if (!details) {
            for (auto &path : paths) {
                eliminate_details(path, edges_to_modify);
            }
        }

        int count(count_tuples(paths));

        if (count == 0) {
            return 0;
        }


        *return_tuples = NULL;
        *return_tuples = get_memory(count, (*return_tuples));

        int sequence = 0;
        int route_id = 0;
        for (const auto &path : paths) {
            if (path.size() > 0)
                path.get_pg_ksp_path(return_tuples, sequence, route_id);
            ++route_id;
        }

        if (count != sequence) {
            return 2;
        }
        (*return_count) = sequence;


#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
        return 0;
    } catch ( ... ) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
    }
    return 1000;
}
