/*PGR-GNU*****************************************************************
File: one_to_many_withPoints_driver.cpp

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
#include <cassert>

#define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../common/src/memory_func.hpp"

#include "./pgr_dijkstra.hpp"
#include "./pgr_withPoints.h"
#include "./many_to_many_withPoints_driver.h"


// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid ANYARRAY,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


int
do_pgr_many_to_many_withPoints(
        pgr_edge_t  *edges,
        size_t total_edges,
        Point_on_edge_t  *points_p,
        size_t total_points,
        pgr_edge_t  *edges_of_points,
        size_t total_edges_of_points,
        int64_t  *start_pidsArr,
        size_t size_start_pidsArr,
        int64_t  *end_pidsArr,
        size_t size_end_pidsArr,

        char driving_side,
        bool details,
        bool directed,
        bool only_cost,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg){
    std::ostringstream log;
    try {
        std::vector< Point_on_edge_t >
            points(points_p, points_p + total_points);

        int errcode = check_points(points, log);
        if (errcode) {
            /* Point(s) with same pid but different edge/fraction/side combination found */
            *err_msg = strdup(log.str().c_str());
            return errcode;
        }

        std::vector< pgr_edge_t >
            edges_to_modify(edges_of_points, edges_of_points + total_edges_of_points);

        std::vector< pgr_edge_t > new_edges;
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                new_edges);


        std::set< int64_t > start_points(start_pidsArr, start_pidsArr + size_start_pidsArr);
        std::set< int64_t > start_vertices;
        std::set< int64_t > end_points(end_pidsArr, end_pidsArr + size_end_pidsArr);
        std::set< int64_t > end_vertices;

        for (const auto &start_pid : start_points) {
            for (const auto point : points) {
                if (point.pid == start_pid) {
                    start_vertices.insert(point.vertex_id);
                    break;
                }
            }
        }
        for (const auto &end_pid : end_points) {
            for (const auto point : points) {
                if (point.pid == end_pid) {
                    end_vertices.insert(point.vertex_id);
                    break;
                }
            }
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;
        const int initial_size = total_edges;

        std::deque< Path > paths;


        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            pgr_dijkstra(digraph, paths, start_vertices, end_vertices, only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            pgr_dijkstra(undigraph, paths, start_vertices, end_vertices, only_cost);
        }


        for (auto &path :paths) {
            adjust_pids(points, path);
        }

        if (!details) {
            for (auto &path :paths) {
                eliminate_details(path);
            }
        }

        /*
         *  order paths based on the start_pid, end_pid
         */
        std::sort(paths.begin(), paths.end(),
                [](const Path &a,const  Path &b) {
                if (b.start_id() != a.start_id()) {
                return b.start_id() < a.start_id();
                }
                return b.end_id() < a.end_id();  
                });

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return 0;
        }

        (*return_tuples) = get_memory(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

#ifndef DEBUG
        *err_msg = "OK";
#else
        *err_msg = strdup(log.str().c_str());
#endif
        return 0;
    } catch ( ... ) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
        return 1000;
    }
    return 0;
}

