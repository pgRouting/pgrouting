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


#include <set>
#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <cassert>


#include "./pgr_dijkstra.hpp"
#include "./pgr_withPoints.hpp"
#include "./one_to_many_withPoints_driver.h"

#include "./../../common/src/basic_vertex.h"
#include "./../../common/src/pgr_alloc.hpp"
#include "./../../common/src/pgr_assert.h"

// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


void
do_pgr_one_to_many_withPoints(
        pgr_edge_t *edges, size_t total_edges,
        Point_on_edge_t *points_p, size_t total_points,
        pgr_edge_t *edges_of_points, size_t total_edges_of_points,
        int64_t start_vid,
        int64_t *end_pidsArr, size_t size_end_pidsArr,
        char driving_side,
        bool details,
        bool directed,
        bool only_cost,
        General_path_element_t **return_tuples, size_t *return_count,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    try {
        pgassert(!(*return_tuples));
        pgassert((*return_count) == 0);
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));

        /*
         * DOCUMENT:
         *   - Points are treated as the same point when the pid is the same
         *   therefore when two points have the same pid, but different edge/fraction
         *   an error is generated.
         */
        std::vector< Point_on_edge_t >
            points(points_p, points_p + total_points);

        int errcode = check_points(points, log);
        if (errcode) {
            *log_msg = strdup(log.str().c_str());
            err << "Unexpected point(s) with same pid but different edge/fraction/side combination found.";
            *err_msg = strdup(err.str().c_str());
            return;
        }


        std::vector< pgr_edge_t >
            edges_to_modify(edges_of_points, edges_of_points + total_edges_of_points);

        std::vector< pgr_edge_t > new_edges;

        log << "driving_side" << driving_side << "\n";
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                new_edges);

        log << "Inserting points into a c++ vector structure\n";
        /*
         * Eliminating duplicates
         * & ordering the points
         */
        std::set< int64_t > s_end_vertices(end_pidsArr, end_pidsArr + size_end_pidsArr);

        std::vector< int64_t > end_vertices(s_end_vertices.begin(), s_end_vertices.end());

        log << "start_vid" << start_vid << "\n";
        log << "end_vertices";

        for (const auto &vid : end_vertices) {
            log << vid << "\n";
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;


        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(
                    pgrouting::extract_vertices(
                        pgrouting::extract_vertices(edges, total_edges),
                        new_edges),
                    gType);
            digraph.graph_insert_data(edges, total_edges);
            digraph.graph_insert_data(new_edges);
            pgr_dijkstra(digraph, paths, start_vid, end_vertices, only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            auto vertices(pgrouting::extract_vertices(edges, total_edges));
            vertices = pgrouting::extract_vertices(vertices, new_edges);
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            vertices.clear();
            undigraph.graph_insert_data(edges, total_edges);
            undigraph.graph_insert_data(new_edges);
            pgr_dijkstra(undigraph, paths, start_vid, end_vertices, only_cost);
        }

        if (!details) {
            for (auto &path : paths) {
                eliminate_details(path, edges_to_modify);
            }
        }
        /*
         * order paths based on the end_pid
         */
        std::sort(paths.begin(), paths.end(), [](const Path &a, const Path &b) {
                return a.end_id() < b.end_id();
                });

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found between Starting and any of the Ending vertices\n";
            *log_msg = strdup(log.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));



        *log_msg = strdup(log.str().c_str());
        pgassert(!(*err_msg));
        return;
    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        *log_msg = strdup(log.str().c_str());
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        *log_msg = strdup(log.str().c_str());
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        *log_msg = strdup(log.str().c_str());
        err << "Caught unknown exception!\n";
        *err_msg = strdup(err.str().c_str());
    }
}

