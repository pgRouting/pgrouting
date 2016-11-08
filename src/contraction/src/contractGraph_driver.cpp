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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>

#ifdef unlink
#undef unlink
#endif

#endif


#include <string.h>
#include <sstream>
#include <deque>
#include <vector>
#include "./contractGraph_driver.h"

#include "./pgr_contractionGraph.hpp"
#include "./pgr_contract.hpp"

#include "./../../common/src/pgr_alloc.hpp"
#include "./../../common/src/pgr_types.h"


template <typename G>
static void process_contraction(
        G &graph,
        const std::vector< pgr_edge_t > &edges,
        const std::vector< int64_t > forbidden_vertices,
        const std::vector< int64_t > contraction_order,
        int64_t max_cycles,
        Identifiers<int64_t> &remaining_vertices,
        std::vector< pgrouting::CH_edge > &shortcut_edges,
        std::ostringstream &log,
        std::ostringstream &err) {
    graph.insert_edges(edges);
    /*
     * this check does not ignore vertices ids that do not belong to the graph
     */
    log << "Checking for valid forbidden vertices\n";
    for (const auto vertex : forbidden_vertices) {
        if (!graph.has_vertex(vertex)) {
            err << "Invalid forbidden vertex: " << vertex << "\n";
            return;
        }
    }

    Identifiers<typename G::V> forbid_vertices;
    for (const auto &vertex : forbidden_vertices) {
        if (graph.has_vertex(vertex)) {
            forbid_vertices.insert(graph.get_V(vertex));
        }
    }

#ifndef NDEBUG
    log << "Before contraction\n";
    graph.print_graph(log);
#endif

    /*
     * Function call to get the contracted graph. 
     */
    Pgr_contract<G> result(graph,
            forbid_vertices,
            contraction_order,
            max_cycles, remaining_vertices,
            shortcut_edges, log);

#ifndef NDEBUG
    log << "After contraction\n";
    log << graph;
    log << "Remaining Vertices:" << "\n";
    for (const auto vertex : remaining_vertices) {
        log << vertex << "\n";
    }
    log << "Added Edges:" << "\n";
    for (const auto edge : shortcut_edges) {
        log << edge << "\n";
    }
#endif
}

template <typename G>
void get_postgres_result(
        G &graph,
        const Identifiers<int64_t> remaining_vertices,
        const std::vector< pgrouting::CH_edge > shortcut_edges,
        pgr_contracted_blob *return_tuples) {
    size_t sequence = 0;
    int i = 1;
    char *type;
    for (auto id : remaining_vertices) {
        type = strdup("v");
        int64_t *contracted_vertices = NULL;
        int contracted_vertices_size = 0;
        graph.get_contracted_vertices(&contracted_vertices,
                contracted_vertices_size, id);
        return_tuples[sequence] = {i, id, type, -1, -1, -1.00,
            contracted_vertices, contracted_vertices_size};
        i++;
        ++sequence;
    }
    for (auto edge : shortcut_edges) {
        type = strdup("e");
        int64_t *contracted_vertices = NULL;
        int contracted_vertices_size = 0;
        graph.get_ids(&contracted_vertices,
                contracted_vertices_size, edge.contracted_vertices());
        return_tuples[sequence] = {i, edge.id, type,
            edge.source, edge.target, edge.cost,
            contracted_vertices, contracted_vertices_size};
        i++;
        ++sequence;
    }
}




/************************************************************
  edges_sql TEXT,
  contraction_order BIGINT[],
  forbidden_vertices BIGINT[] DEFAULT ARRAY[]::BIGINT[],
  max_cycles integer DEFAULT 1,
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
    std::ostringstream err;
    try {
        std::ostringstream debug;
        graphType gType = directed? DIRECTED: UNDIRECTED;
        /*
         * Converting to C++ structures
         */
        std::vector< pgr_edge_t > edges(data_edges, data_edges + total_edges);
        std::vector< int64_t > forbid(
                forbidden_vertices,
                forbidden_vertices + size_forbidden_vertices);
        std::vector< int64_t > ordering(
                contraction_order,
                contraction_order + size_contraction_order);

        /*
         * Extracting vertices of the graph
         */
        Identifiers<int64_t> remaining_vertices;
        std::vector< pgrouting::CH_edge > shortcut_edges;

#ifndef NDEBUG
        log << "Original Graph: \n" <<
            std::setprecision(32);
        for (const auto edge : edges) {
            log << "id = " << edge.id
                << "\tsource = " << edge.source
                << "\ttarget = " << edge.target
                << "\tcost = " << edge.cost
                << "\treverse_cost = " << edge.reverse_cost
                << ")\n";
        }
        log << "size_contraction_order " << ordering.size() << "\n";
        log << "contraction_order: " <<"{ ";
        for (const auto o : ordering) {
            log << o << ", ";
        }
        log << " }\n";

        log << "size_forbidden_vertices " << forbid.size() << "\n";
        log << "forbidden_vertices" << "{ ";
        for (const auto vertex : forbid) {
            log << vertex << ", ";
        }
        log << " }\n";
        log << "max_cycles " << max_cycles << "\n";
        log << "directed " << gType << "\n";
#endif

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::CHDirectedGraph digraph(gType);

            process_contraction(digraph, edges, forbid, ordering,
                    max_cycles,
                    remaining_vertices, shortcut_edges,
                    log, err);

            (*return_tuples) = pgr_alloc(
                    remaining_vertices.size()+shortcut_edges.size(),
                    (*return_tuples));
            (*return_count) = remaining_vertices.size()+shortcut_edges.size();

            get_postgres_result(
                    digraph,
                    remaining_vertices,
                    shortcut_edges,
                    *return_tuples);
        } else {
            log << "Working with Undirected Graph\n";

            pgrouting::CHUndirectedGraph undigraph(gType);
            process_contraction(undigraph, edges, forbid, ordering,
                    max_cycles,
                    remaining_vertices, shortcut_edges,
                    log, err);

            (*return_tuples) = pgr_alloc(
                    remaining_vertices.size()+shortcut_edges.size(),
                    (*return_tuples));
            (*return_count) = remaining_vertices.size()+shortcut_edges.size();

            get_postgres_result(
                    undigraph,
                    remaining_vertices,
                    shortcut_edges,
                    *return_tuples);
        }

#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
    }
    catch (AssertFailedException &except) {
        log << except.what() << "\n";
        *err_msg = strdup(log.str().c_str());
    } catch (std::exception& except) {
        log << except.what() << "\n";
        *err_msg = strdup(log.str().c_str());
    } catch(...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
    }
}

