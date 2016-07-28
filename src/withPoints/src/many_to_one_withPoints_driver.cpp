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


#if defined(__MINGW32__) ||  defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include <algorithm>
#include <set>
#include <sstream>
#include <deque>
#include <vector>
#include <cassert>

#include "./pgr_dijkstra.hpp"
#include "./pgr_withPoints.hpp"
#include "./many_to_one_withPoints_driver.h"
extern "C" {
#include "./../../common/src/pgr_types.h"
}
#include "./../../common/src/pgr_alloc.hpp"


// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid ANYARRAY,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


int
do_pgr_many_to_one_withPoints(
        pgr_edge_t *edges, size_t total_edges,
        Point_on_edge_t *points_p, size_t total_points,
        pgr_edge_t *edges_of_points, size_t total_edges_of_points,
        int64_t *start_pidsArr, size_t size_start_pidsArr,
        int64_t end_vid,
        char driving_side,
        bool details,
        bool directed,
        bool only_cost,
        General_path_element_t **return_tuples, size_t *return_count,
        char ** err_msg) {
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


        std::set< int64_t > s_start_vertices(start_pidsArr, start_pidsArr + size_start_pidsArr);
        std::vector< int64_t > start_vertices(s_start_vertices.begin(), s_start_vertices.end());

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;


        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            pgr_dijkstra(digraph, paths, start_vertices, end_vid, only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            pgr_dijkstra(undigraph, paths, start_vertices, end_vid, only_cost);
        }

#if 0
        for (auto &path : paths) {
            adjust_pids(points, path);
        }
#endif
        if (!details) {
            for (auto &path : paths) {
                eliminate_details(path, edges_to_modify);
            }
        }

        /*
         * order paths based on the start_pid
         */
        std::sort(paths.begin(), paths.end(), [](const Path &a, const Path &b) {
                return a.start_id() < b.start_id();
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

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

#ifndef NDEBUG
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
        return 1000;
    }
    return 0;
}

