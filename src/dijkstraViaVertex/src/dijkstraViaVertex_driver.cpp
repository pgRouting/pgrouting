/*PGR-GNU*****************************************************************
File: dijkstraViaVertex_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo

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
#include "./dijkstraViaVertex_driver.h"

#define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}


template <class G>
void
pgr_dijkstraViaVertex(
        G &graph,
        const std::vector< int64_t > via_vertices,
        std::deque< Path > &paths,
        bool strict,
        bool U_turn_on_edge,  //! true = u turns are allowed between paths
        std::ostringstream &log) {    

    if (via_vertices.size() == 0) {
        return;
    }

    paths.clear();
    int64_t prev_vertex = via_vertices[0];
    Path path;
    
    //for (size_t i = 0; i < via_vertices.size() - 1; ++i) {
    int64_t i = 0;
    for (const auto &vertex : via_vertices) {
        if (i == 0) {
            prev_vertex = vertex; ++i;
            continue;
        }

        // Delete uTurn edges only valid for paths that are not the first path
        if (!U_turn_on_edge && i > 1) {

            // we can only delete if there is was a path, that is at least one edge size
            if (path.path.size() > 1) {
                // Delete from the graph the last edge if its outgoing also
                // edge to be removed = second to last edge path[i].edge;
                int64_t edge_to_be_removed = path.path[path.path.size() - 2].edge;
                int64_t last_vertex_of_path = prev_vertex;
               // path.path[path.path.size() - 1].vertex;

                // and the current vertex is not a dead end
                if (graph.out_degree(last_vertex_of_path) > 1) {
                log << "departing from " << last_vertex_of_path
                    << " deleting edge " << edge_to_be_removed << "\n";
                    graph.disconnect_out_going_edge(last_vertex_of_path, edge_to_be_removed);
                }
            }
        }

        path.clear();

        log << "from " << prev_vertex << " to " << vertex << "\n";
        pgr_dijkstra(graph, path, prev_vertex, vertex);

        if (!U_turn_on_edge && i > 1) {
            graph.restore_graph();
            if (path.path.empty()) { 
                /*
                 *  no path was found with the deleted edge
                 *  try with the edge back in the graph
                 */
                log << "WAS empty so again from " << prev_vertex << " to " << vertex << "\n";
                pgr_dijkstra(graph, path, prev_vertex, vertex);
            }
        }

        if (strict && path.path.empty()) {
            paths.clear();
            return;
        }
        paths.push_back(path);

        /*
         * got to the next
         */
        prev_vertex = vertex; ++i;
    }
}

static
void
get_path(
        int64_t route_id,
        int64_t path_id,
        const Path &path,
        Routes_t **postgres_data,
        double &route_cost,
        size_t &sequence) {
    for (unsigned int i = 0; i < path.size(); i++) {
        (*postgres_data)[sequence] = {
            route_id,
            path_id,
            i,
            path.path[i].from,
            path.path[i].to,
            path.path[i].vertex,
            path.path[i].edge,
            path.path[i].cost,
            path.path[i].tot_cost,
            route_cost};
        route_cost += path.path[i].cost;
        sequence++;
    }
}


static
size_t
get_route(
        Routes_t **ret_path,
        const std::deque< Path > &paths) {
    size_t sequence = 0;    //arrys index
    int64_t path_id = 1;    // id's in posgresql start with 1
    int64_t route_id = 1;   
    double route_cost = 0;  // routes_agg_cost
    for (const Path &path : paths) {
        if (path.path.size() > 0)
            get_path(route_id, path_id, path, ret_path, route_cost, sequence);
        ++path_id;
    }
    return sequence;
}

static
Routes_t *
get_memory(int size, Routes_t *path){
    if( path ==0  ) {
        path = (Routes_t *) malloc(size * sizeof(Routes_t));
    } else {
        path = (Routes_t *)realloc(path,size * sizeof(Routes_t));
    }
    return path;
}


// CREATE OR REPLACE FUNCTION pgr_dijkstraViaVertices(sql text, vertices anyarray, directed boolean default true,
void
do_pgr_dijkstraViaVertex(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        int64_t  *via_vidsArr,
        int size_via_vidsArr,
        bool directed,
        bool strict,
        bool U_turn_on_edge,
        Routes_t **return_tuples,
        size_t *return_count,
        char ** err_msg){
    std::ostringstream log;
    try {

        if (total_tuples == 1) {
            log << "Requiered: more than one tuple\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }

        graphType gType = directed? DIRECTED: UNDIRECTED;
        const int initial_size = total_tuples;

        std::deque< Path >paths;
        log << "Inserting vertices into a c++ vector structure\n";
        std::vector< int64_t > via_vertices(via_vidsArr, via_vidsArr + size_via_vidsArr);

        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            digraph.graph_insert_data(data_edges, total_tuples);
            pgr_dijkstraViaVertex(digraph, via_vertices, paths, strict, U_turn_on_edge, log);
        } else {
            log << "Working with Undirected Graph\n";
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            undigraph.graph_insert_data(data_edges, total_tuples);
            pgr_dijkstraViaVertex(undigraph, via_vertices, paths, strict, U_turn_on_edge, log);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log << 
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return;
        }

        // get the space required to store all the paths
        (*return_tuples) = get_memory(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (get_route(return_tuples, paths));
        (*return_tuples)[count - 1].edge = -2;

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





