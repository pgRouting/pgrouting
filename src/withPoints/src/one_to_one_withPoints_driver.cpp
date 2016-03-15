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
#include "./pgr_dijkstra.hpp"
#include "./pgr_withPoints.hpp"
#include "./one_to_one_withPoints_driver.h"

// #define DEBUG


#include "./../../common/src/memory_func.hpp"


// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


int
do_pgr_withPoints(
        pgr_edge_t  *edges,           size_t total_edges,
        Point_on_edge_t  *points_p,   size_t total_points,
        pgr_edge_t  *edges_of_points, size_t total_edges_of_points,
        int64_t start_pid,
        int64_t end_pid,
        bool directed,
        char driving_side,
        bool details,
        bool only_cost,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg){
    std::ostringstream log;
    try {
        log << "Entering do_pgr_withPoints\n";
        std::vector< Point_on_edge_t >
            points(points_p, points_p + total_points);
        
        log << "total points" << points.size() << "\n";

        /*
         * This test is easier in C++
         */
        int errcode = check_points(points, log);
        if (errcode) {
            log << "Point(s) with same pid but different edge/fraction/side combination found";
            *err_msg = strdup(log.str().c_str());
            return errcode;
        }

        std::vector< pgr_edge_t >
            edges_to_modify(edges_of_points, edges_of_points + total_edges_of_points);

        std::vector< pgr_edge_t > new_edges;
        log << "start_pid" << start_pid << "\n";
        log << "end_pid" << end_pid << "\n";
        log << "driving_side" << driving_side << "\n";
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                new_edges,
                log);

        int64_t start_vid(start_pid);
        int64_t end_vid(end_pid);
#if 0
        for (const auto point : points) {
            if (point.pid == start_pid) {
                start_vid = point.vertex_id;
            }
            if (point.pid == end_pid) {
                end_vid = point.vertex_id;
            }

        }
#endif
        log << "start_vid" << start_vid << "\n";
        log << "end_vid" << end_vid << "\n";
        graphType gType = directed? DIRECTED: UNDIRECTED;
        const auto initial_size = total_edges;

        Path path;


        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            pgr_dijkstra(digraph, path, start_vid, end_vid, only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            pgr_dijkstra(undigraph, path, start_vid, end_vid, only_cost);
        }


        size_t count(path.size());
        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return 0;
        }

        if (!details) {
            eliminate_details(path, edges_to_modify);
        }

        (*return_tuples) = get_memory(count, (*return_tuples));
        size_t sequence = 0;
        path.generate_postgres_data(return_tuples, sequence);
        (*return_count) = sequence;

#ifndef DEBUG
        {
            std::ostringstream log;
            log << "OK";
            *err_msg = strdup(log.str().c_str());
        }

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
