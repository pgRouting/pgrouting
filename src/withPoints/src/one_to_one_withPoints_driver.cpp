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
#include "./one_to_one_withPoints_driver.h"

 #define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../common/src/memory_func.hpp"

// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


void
do_pgr_withPoints(
        pgr_edge_t  *edges,
        size_t total_edges,
        Point_on_edge_t  *points,
        size_t total_points,
        pgr_edge_t  *edges_of_points,
        size_t total_edges_of_points,
        int64_t start_pid,
        int64_t end_pid,
        bool directed,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg){
    std::ostringstream log;
    try {

#if 0
        if (total_tuples == 1) {
            log << "Requiered: more than one tuple\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }
#endif
        graphType gType = directed? DIRECTED: UNDIRECTED;
        const int initial_size = total_edges;

        Path path;

        log << "start_pid" << start_pid << "\n";
        log << "end_pid" << end_pid << "\n";

        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            digraph.graph_insert_data(edges, total_edges);
#ifdef DEBUG
            digraph.print_graph(log);
#endif
            pgr_dijkstra(digraph, path, -start_pid, -end_pid);
        } else {
            log << "Working with Undirected Graph\n";
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            undigraph.graph_insert_data(edges, total_edges);
#ifdef DEBUG
            undigraph.print_graph(log);
#endif
            pgr_dijkstra(undigraph, path, -start_pid, -end_pid);
        }

        size_t count(path.path.size());

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return;
        }

        (*return_tuples) = get_memory(count, (*return_tuples));
        size_t sequence = 0;
        path.generate_postgres_data(return_tuples, sequence);
        (*return_count) = sequence;


#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
    } catch ( ... ) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
    }
}


void
get_new_queries(
        char *edges_sql,
        char *points_sql,
        int64_t start_pid,
        int64_t end_pid,
        bool strict,
        char **edges_of_points_query,
        char **edges_no_points_query) {

    std::ostringstream edges_of_points_sql;
    std::ostringstream edges_no_points_sql;

    if (strict) {
        edges_of_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << "),"
            << " strict AS (SELECT edge_id FROM points WHERE pid IN (" << start_pid << ", " << end_pid << ")) "
            << " SELECT DISTINCT edges.* FROM edges JOIN strict ON (id = edge_id)";
        *edges_of_points_query = strdup(edges_of_points_sql.str().c_str());

        edges_no_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << "),"
            << " strict AS (SELECT edge_id FROM points WHERE pid IN (" << start_pid << ", " << end_pid << ")) "
            << " SELECT edges.* FROM edges WHERE NOT EXISTS (SELECT edge_id FROM strict WHERE id = edge_id)";
        *edges_no_points_query = strdup(edges_no_points_sql.str().c_str());
    } else {
        edges_of_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << ")"
            << " SELECT DISTINCT edges.* FROM edges JOIN points ON (id = edge_id)";
        *edges_of_points_query = strdup(edges_of_points_sql.str().c_str());

        edges_no_points_sql << "WITH "
            << " edges AS (" << edges_sql << "),"
            << " points AS (" << points_sql << ")"
            << " SELECT edges.* FROM edges WHERE NOT EXISTS (SELECT edge_id FROM points WHERE id = edge_id)";
        *edges_no_points_query = strdup(edges_no_points_sql.str().c_str());
    }
}



