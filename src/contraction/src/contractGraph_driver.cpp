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


// #define DEBUG
#include <sstream>
#include <deque>
#include <vector>
#include <string.h>
#include "./contractGraph_driver.h"

#include "./pgr_contract.hpp"

extern "C" {
#include "./../../common/src/pgr_types.h"
#include "./structs.h"
}

#include "./../../common/src/pgr_alloc.hpp"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/identifiers.hpp"


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

		std::ostringstream debug;
		graphType gType = directed? DIRECTED: UNDIRECTED;
		std::vector< pgr_edge_t > edges(data_edges, data_edges + total_edges);
		std::vector < pgRouting::contraction::Vertex > vertices(pgRouting::contraction::extract_vertices(edges));
		Identifiers<int64_t> remaining_vertices;
		std::vector< pgRouting::contraction::Edge > shortcut_edges;

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
		const int64_t initial_size = (int64_t)total_edges;
		// Contraction_type contraction_type_count = Contraction_type::last;
		log << "size_contraction_order " << size_contraction_order << "\n";
		// log << "greatest contraction type " << static_cast<int>(contraction_type_count) << "\n";
		log << "contraction_order\n" <<   " { \n";
		for (size_t i = 0; i < size_contraction_order; ++i) {
			log << contraction_order[i] << ", ";
			// pgassert((contraction_order[i] >=0) && (contraction_order[i] < static_cast<int>(contraction_type_count)));
#if 0
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

		if (directed) {
			log << "Working with directed Graph\n";
			//#if 0
			pgRouting::CHDirectedGraph digraph(vertices, gType);
			digraph.graph_insert_data(data_edges, total_edges);
			log << "Checking for valid forbidden vertices\n";
			for (size_t i = 0; i < size_forbidden_vertices; ++i) {
				if (!digraph.has_vertex(forbidden_vertices[i]))
				{
					log << "Invalid forbidden vertex: " << forbidden_vertices[i] << "\n";
					*err_msg = strdup(log.str().c_str());
					return;
				}
			}
			Identifiers<int64_t> forbid_vertices(forbidden_vertices,
					size_forbidden_vertices);

			digraph.print_graph(log);
			//log << digraph;

			// digraph.print_graph(log);
			//#endif
#if 0
			*err_msg = strdup(log.str().c_str());
			return;
#endif
			/*
			   Function call to get the contracted graph
			 */
			//#if 0
			log << "Calling contraction\n";

			pgr_contractGraph(digraph,
					forbid_vertices, 
					contraction_order, size_contraction_order,
					max_cycles, remaining_vertices,
					shortcut_edges, debug);
			digraph.print_graph(log);
			log << debug.str().c_str() << "\n";
			(*return_tuples) = pgr_alloc(remaining_vertices.size()+shortcut_edges.size(), (*return_tuples));
			size_t sequence = 0;
			int i = 1;
			log << "Remaining Vertices:" << "\n";
			for (const auto vertex : remaining_vertices) {
				log << vertex << "\n";
			}
			char *type, *contracted_vertices;
			for (auto id : remaining_vertices) {
				type = strdup("v");
				std::ostringstream os;
				digraph.print_contracted_vertices(os, id);
				contracted_vertices = strdup(os.str().c_str());
				//char *contracted_vertices = strdup("--"); 
				(*return_tuples)[sequence] = {i, id, type, -1, -1, contracted_vertices};
				++sequence;
			}
			log << "Added Edges:" << "\n";
			for (const auto edge : shortcut_edges) {
				log << edge << "\n";
			}
			for (auto edge : shortcut_edges) {
				type = strdup("e");
				std::ostringstream os;
				digraph.get_ids(os, edge.contracted_vertices());
				contracted_vertices = strdup(os.str().c_str());
				//char *contracted_vertices = strdup("--"); 
				(*return_tuples)[sequence] = {i, edge.id, type, edge.source, edge.target, contracted_vertices};
				++sequence;
			}

			(*return_count) = sequence;
			//#endif
		} else {
			log << "Working with Undirected Graph\n";

			//#if 0
			pgRouting::CHUndirectedGraph undigraph(vertices, gType);
			undigraph.graph_insert_data(data_edges, total_edges);
			log << "Checking for valid forbidden vertices\n";
			for (size_t i = 0; i < size_forbidden_vertices; ++i) {
				if (!undigraph.has_vertex(forbidden_vertices[i]))
				{
					log << "Invalid forbidden vertex: " << forbidden_vertices[i] << "\n";
					*err_msg = strdup(log.str().c_str());
					return;
				}
			}
			Identifiers<int64_t> forbid_vertices(forbidden_vertices,
					size_forbidden_vertices);
			//log << undigraph;
			// undigraph.print_graph_c(log);
			/* Function call to get the contracted graph. */
			pgr_contractGraph(undigraph,
					forbid_vertices, 
					contraction_order, size_contraction_order,
					max_cycles, remaining_vertices,
					shortcut_edges, debug);
			log << "Size of remaining_vertices: " << remaining_vertices.size(); 
			log << debug.str().c_str() << "\n";
			//#endif
			(*return_tuples) = pgr_alloc(remaining_vertices.size(), (*return_tuples));
			size_t sequence = 0;
			int i = 1;
			log << "Remaining Vertices:" << "\n";
			for (const auto vertex : remaining_vertices) {
				log << vertex << "\n";
			}
			char *type, *contracted_vertices;
			for (auto id : remaining_vertices) {
				type = strdup("v");
				std::ostringstream os;
				undigraph.print_contracted_vertices(os, id);
				contracted_vertices = strdup(os.str().c_str());
				//char *contracted_vertices = strdup("--"); 
				(*return_tuples)[sequence] = {i, id, type, -1, -1, contracted_vertices};
				++sequence;
			}
			log << "Added Edges:" << "\n";
			for (const auto edge : shortcut_edges) {
				log << edge << "\n";
			}
			for (auto edge : shortcut_edges) {
				type = strdup("e");
				std::ostringstream os;
				undigraph.get_ids(os, edge.contracted_vertices());
				contracted_vertices = strdup(os.str().c_str());
				//char *contracted_vertices = strdup("--"); 
				(*return_tuples)[sequence] = {i, edge.id, type, edge.source, edge.target, contracted_vertices};
				++sequence;
			}
			(*return_count) = sequence;
		}

#ifndef DEBUG
		*err_msg = strdup("OK");
#else
		*err_msg = strdup(log.str().c_str());
#endif

	}
	catch (AssertFailedException &exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        //return false;
    } catch (std::exception& exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        //return false;
    } catch(...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
        //return false;
    }
}

int is_valid_contraction(int64_t number) {
    switch (number) {
        case -2:
        return -1;
        break;
        case -1:
        return -1;
        break;
        case 0:
        return 1;
        break;
        case 1:
        return 1;
        break;
        default:
        return -1;
        break;
    }
}






