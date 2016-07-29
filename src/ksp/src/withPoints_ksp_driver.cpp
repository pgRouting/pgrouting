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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <sstream>
#include <deque>
#include <vector>
#include "./pgr_ksp.hpp"
#include "./withPoints_ksp_driver.h"
#include "./../../withPoints/src/pgr_withPoints.hpp"
#include "./../../common/src/pgr_alloc.hpp"


// CREATE OR REPLACE FUNCTION pgr_withPointsKSP(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


int
do_pgr_withPointsKsp(
        pgr_edge_t  *edges,           size_t total_edges,
        Point_on_edge_t  *points_p,   size_t total_points,
        pgr_edge_t  *edges_of_points, size_t total_edges_of_points,
        int64_t start_vid,
        int64_t end_vid,
        int k,
        bool directed,
        bool heap_paths,
        char driving_side,
        bool details,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg) {
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


        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            Pgr_ksp< pgrouting::DirectedGraph  > fn_yen;
            paths = fn_yen.Yen(digraph, start_vid, end_vid, k, heap_paths);
        } else {
            log << "Working with undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            Pgr_ksp< pgrouting::UndirectedGraph > fn_yen;
            paths = fn_yen.Yen(undigraph, start_vid, end_vid, k, heap_paths);
        }

#if 0
        for (auto &path : paths) {
            path.print_path(log);
            adjust_pids(points, path);
            path.print_path(log);
        }
#endif

        if (!details) {
            for (auto &path : paths) {
                eliminate_details(path, edges_to_modify);
            }
        }

        auto count(count_tuples(paths));

        if (count == 0) {
            return 0;
        }


        *return_tuples = NULL;
        *return_tuples = pgr_alloc(count, (*return_tuples));

        size_t sequence = 0;
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
