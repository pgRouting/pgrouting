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


#define DEBUG
#include <sstream>
#include <deque>
#include <vector>
#include <string.h>
#include "./contractGraph_driver.h"

#ifdef DEBUG
#include "./pgr_contract.hpp"
#endif

extern "C" {
#include "./../../common/src/pgr_types.h"
#include "./structs.h"
}

#include "./../../common/src/pgr_alloc.hpp"
#include "./../../common/src/debug_macro.h"


/************************************************************
  edges_sql TEXT,
    level BIGINT,
    directed BOOLEAN DEFAULT true
 ***********************************************************/
void
do_pgr_contractGraph(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        int64_t *forbidden_vertices,
        size_t size_forbidden_vertices,
        int64_t *contraction_order,
        size_t size_contraction_order,
        int64_t max_cycles,
        bool directed,
        pgr_contracted_blob **return_tuples,
        size_t *return_count,
        char ** err_msg) {
    std::ostringstream log;
    try {
        std::ostringstream contracted_graph_name;
        std::ostringstream contracted_graph_blob;
        std::ostringstream removedEdges;
        std::ostringstream removedVertices;
        std::ostringstream psuedoEdges;
        std::ostringstream dmap;
        std::ostringstream debug;
        graphType gType = directed? DIRECTED: UNDIRECTED;
        std::vector< pgr_edge_t > edges(data_edges, data_edges + total_edges);
        std::vector < pgRouting::contraction::Vertex > vertices(pgRouting::contraction::extract_vertices(edges));
        log << "Original: \n" <<
            std::setprecision(32);
        for (const auto edge: edges) {
            log << "id = " << edge.id
                << "\tsource = " << edge.source
                << "\ttarget = " << edge.target
                << "\tcost = " << edge.cost
                << "\treverse_cost = " << edge.reverse_cost
                << ")\n";
        }
        const int initial_size = total_edges;
        // Contraction_type contraction_type_count = Contraction_type::last;
        log << "size_contraction_order " << size_contraction_order << "\n";
        // log << "greatest contraction type " << static_cast<int>(contraction_type_count) << "\n";
        log << "contraction_order\n" <<   " { \n";
        for (size_t i = 0; i < size_contraction_order; ++i) {
            log << contraction_order[i] << ", ";
            // pgassert((contraction_order[i] >=0) && (contraction_order[i] < static_cast<int>(contraction_type_count)));
            #ifdef DEBUG
            if (!is_valid_contraction_number(contraction_order[i])) {
                log << "Error: Enter a valid Contraction Type\n";
                (*return_tuples) = NULL;
                (*return_count) = 0;
                *err_msg = strdup(log.str().c_str());
                return;
            }
            log << "\n";
            #endif
        }
        log << " }\n";


        log << "size_forbidden_vertices " << size_forbidden_vertices << "\n";
        // log << "greatest contraction type " << static_cast<int>(contraction_type_count) << "\n";
        log << "forbidden_vertices\n" <<   " { \n";
        for (size_t i = 0; i < size_forbidden_vertices; ++i) {
            log << forbidden_vertices[i] << ", ";
            log << "\n";
        }
        log << " }\n";        
        log << "max_cycles " << max_cycles << "\n";
        log << "directed " << directed << "\n";
        log << "gType " << gType << "\n";
        log << "total_tuples " << initial_size << "\n";
        if (total_edges == 1) {
            log << "Requiered: more than one tuple\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }
        if (max_cycles < 1) {
            log << "Required: atleast one cycle\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }
        if (directed) {
            log << "Working with directed Graph\n";
 #ifdef DEBUG
//#if 0
            pgRouting::CHDirectedGraph digraph(vertices, gType);
            digraph.graph_insert_data(data_edges, total_edges);
            //log << digraph;
           // digraph.print_graph(log);
#endif
#if 0
            *err_msg = strdup(log.str().c_str());
            return;
#endif
            /*
            Function call to get the contracted graph
            */
#ifdef DEBUG
//#if 0
            pgr_contractGraph(digraph,
                forbidden_vertices, size_forbidden_vertices, 
                contraction_order, size_contraction_order,
                max_cycles, contracted_graph_name, contracted_graph_blob,
                removedEdges, removedVertices, psuedoEdges, debug);
            log << debug.str().c_str() << "\n";
#endif
        } else {
            log << "Working with Undirected Graph\n";
            
#ifdef DEBUG
//#if 0
            pgRouting::CHUndirectedGraph undigraph(vertices, gType);
            undigraph.graph_insert_data(data_edges, total_edges);

            //log << undigraph;
            // undigraph.print_graph_c(log);
            /* Function call to get the contracted graph. */
            pgr_contractGraph(undigraph,
                forbidden_vertices, size_forbidden_vertices, 
                contraction_order, size_contraction_order,
                max_cycles, contracted_graph_name, contracted_graph_blob,
                removedEdges, removedVertices, psuedoEdges, debug);
            log << debug.str().c_str() << "\n";
#endif
        }
#ifdef DEBUG
        (*return_tuples) = pgr_alloc(1, (*return_tuples));
        (*return_tuples)->contracted_graph_name = strdup(contracted_graph_name.str().c_str());
        (*return_tuples)->contracted_graph_blob = strdup(contracted_graph_blob.str().c_str());
        (*return_tuples)->removedVertices = strdup(removedVertices.str().c_str());
        (*return_tuples)->removedEdges = strdup(removedEdges.str().c_str());
        (*return_tuples)->psuedoEdges = strdup(psuedoEdges.str().c_str());
        (*return_count) = 1;
        // get the space required to store all the paths
        //(*return_tuples) = get_memory(count, (*return_tuples));
        //log << "Converting a set of paths into the tuples\n";
        //(*return_count) = (collapse_paths(return_tuples, paths));
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





