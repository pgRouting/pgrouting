/*PGR-GNU*****************************************************************
File: contractGraph_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
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
#include "./contractGraph_driver.h"
#include "./pgr_contract.hpp"


extern "C" {
#include "./../../common/src/pgr_types.h"
#include "./structs.h"
}

#include "./../../common/src/memory_func.hpp"
#define DEBUG
#include "./../../common/src/debug_macro.h"


/************************************************************
  edges_sql TEXT,
    level BIGINT,
    directed BOOLEAN DEFAULT true
 ***********************************************************/
void
do_pgr_contractGraph(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        int64_t level,
        bool directed,
        pgr_contracted_blob **return_tuples,
        size_t *return_count,
        char **err_msg) {
    std::ostringstream log;
    try {
        std::ostringstream contracted_graph_name;
        std::ostringstream contracted_graph_blob;
        std::ostringstream removedEdges;
        std::ostringstream removedVertices;
        std::ostringstream psuedoEdges;

        

        graphType gType = directed? DIRECTED: UNDIRECTED;
        const int initial_size = total_tuples;

        log << "Level" << level << "\n";
        log << "Directed" << directed << "\n";
        if (total_tuples == 1) {
            log << "Requiered: more than one tuple\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }
        if (directed) {
            log << "Working with directed Graph\n";
            Pgr_contractionGraph< CDirectedGraph > digraph(gType, initial_size);
            digraph.graph_insert_data_c(data_edges, total_tuples);
#ifdef DEBUG
            digraph.print_graph(log);
#endif
#if 0
            *err_msg = strdup(log.str().c_str());
            return;
#endif
            /*
            Function call to get the contracted graph
            */
            pgr_contractGraph(digraph,level,
              contracted_graph_name,contracted_graph_blob,removedEdges,
              removedVertices,psuedoEdges);
        } else {
            log << "Working with Undirected Graph\n";
 
            Pgr_contractionGraph< CUndirectedGraph > undigraph(gType, initial_size);
            undigraph.graph_insert_data_c(data_edges, total_tuples);
#ifdef DEBUG
            undigraph.print_graph(log);
#endif

            /* Function call to get the contracted graph. */
            pgr_contractGraph(undigraph, level,
                    contracted_graph_name, contracted_graph_blob, removedEdges,
                    removedVertices, psuedoEdges);

        }
        (*return_tuples) = get_memory(1, (*return_tuples));
        (*return_tuples)->contracted_graph_name = strdup(contracted_graph_name.str().c_str());
        (*return_tuples)->contracted_graph_blob = strdup(contracted_graph_blob.str().c_str());
        (*return_tuples)->removedVertices = strdup(removedVertices.str().c_str());
        (*return_tuples)->removedEdges = strdup(removedEdges.str().c_str());
        (*return_tuples)->psuedoEdges = strdup(psuedoEdges.str().c_str());
        (*return_count) = 1;
#if 0
        // get the space required to store all the paths
        (*return_tuples) = get_memory(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));
#endif

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





