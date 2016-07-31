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
#ifdef open
#undef open
#endif
#endif

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <set>

#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./../../withPoints/src/pgr_withPoints.hpp"
#include "./withPoints_dd_driver.h"


#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_alloc.hpp"


/*******************************************************************************/
// CREATE OR REPLACE FUNCTION pgr_withPointsDD(
// edges_sql TEXT,
// points_sql TEXT,
// start_pids anyarray,
// distance FLOAT,
//
// driving_side CHAR -- DEFAULT 'b',
// details BOOLEAN -- DEFAULT false,
// directed BOOLEAN -- DEFAULT true,
// equicost BOOLEAN -- DEFAULT false,


int
do_pgr_many_withPointsDD(
        pgr_edge_t      *edges,             size_t total_edges,
        Point_on_edge_t *points_p,          size_t total_points,
        pgr_edge_t      *edges_of_points,   size_t total_edges_of_points,

        int64_t  *start_pids_arr,    size_t s_len,
        double distance,

        bool directed,
        char driving_side,
        bool details,
        bool equiCost,

        General_path_element_t **return_tuples, size_t *return_count,
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

        /*
         * checking here is easier than on the C code
         */
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

        std::set< int64_t > s_start_vids(start_pids_arr, start_pids_arr + s_len);
        std::vector< int64_t > start_vids(s_start_vids.begin(), s_start_vids.end());
#if 0
        std::set< int64_t > start_vids;

        for (const auto start_pid : start_pids) {
            for (const auto point : points) {
                if (point.pid == start_pid) {
                    start_vids.insert(point.vertex_id);
                    break;
                }
            }
        }
#endif


        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            pgr_drivingDistance(digraph, paths, start_vids, distance, equiCost);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            pgr_drivingDistance(undigraph, paths, start_vids, distance, equiCost);
        }

        for (auto &path : paths) {
            log << path;

            if (!details) {
                eliminate_details_dd(path);
            }
            log << path;
            std::sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.node < r.node;});
            std::stable_sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.agg_cost < r.agg_cost;});
            log << path;
        }

        size_t count(count_tuples(paths));


        if (count == 0) {
            *err_msg = strdup("NOTICE: No return values was found");
            return 0;
        }
        *return_tuples = pgr_alloc(count, (*return_tuples));
        *return_count = collapse_paths(return_tuples, paths);

#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
        return 0;
    } catch ( ... ) {
        *err_msg = strdup("Caught unknown expection!");
        return 1000;
    }
}


// CREATE OR REPLACE FUNCTION pgr_withPointsDD(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
//
// driving_side CHAR -- DEFAULT 'b',
// details BOOLEAN -- DEFAULT false,
// directed BOOLEAN -- DEFAULT true,
// equicost BOOLEAN -- DEFAULT false,

int
do_pgr_withPointsDD(
        pgr_edge_t  *edges, size_t total_edges,
        Point_on_edge_t  *points_p, size_t total_points,
        pgr_edge_t  *edges_of_points, size_t total_edges_of_points,

        int64_t start_vid,
        double      distance,

        char driving_side,
        bool details,
        bool directed,

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

        /*
         * checking here is easier than on the C code
         */
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

#if 0
        int64_t start_vid = 0;
        for (const auto point : points) {
            if (point.pid == start_pid) {
                start_vid = point.vertex_id;
            }
        }
#endif

        graphType gType = directed? DIRECTED: UNDIRECTED;

        Path path;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            pgr_drivingDistance(digraph, path, start_vid, distance);
        } else {
            log << "Working with undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            pgr_drivingDistance(undigraph, path, start_vid, distance);
        }


#if 0
        path.print_path(log);
        adjust_pids(points, path);
        path.print_path(log);
#endif

        if (!details) {
            eliminate_details_dd(path);
        }
        std::sort(path.begin(), path.end(),
                [](const Path_t &l, const  Path_t &r)
                {return l.node < r.node;});
        std::stable_sort(path.begin(), path.end(),
                [](const Path_t &l, const  Path_t &r)
                {return l.agg_cost < r.agg_cost;});


        auto count(path.size());

        if (count == 0) {
            return 0;
        }


        *return_tuples = NULL;
        *return_tuples = pgr_alloc(count, (*return_tuples));

        size_t sequence = 0;
        path.get_pg_dd_path(return_tuples, sequence);

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
