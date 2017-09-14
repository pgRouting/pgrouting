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

#include "drivers/contraction/contractGraph_driver.h"

#include <string.h>
#include <sstream>
#include <deque>
#include <vector>

#include "contraction/pgr_contractionGraph.hpp"
#include "contraction/pgr_contract.hpp"

#include "cpp_common/pgr_alloc.hpp"

static
bool
is_valid_contraction(int64_t number) {
    switch (number) {
        case 1:
        case 2:
            return true;
            break;
        default:
            return false;
            break;
    }
}


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
            forbid_vertices += graph.get_V(vertex);
        }
    }

#ifndef NDEBUG
    log << "Before contraction\n";
    graph.print_graph(log);
#endif

    /*
     * Function call to get the contracted graph.
     */
    pgrouting::contraction::Pgr_contract<G> result(graph,
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
static
void get_postgres_result(
        G &graph,
        const Identifiers<int64_t> remaining_vertices,
        const std::vector< pgrouting::CH_edge > shortcut_edges,
        contracted_rt **return_tuples) {
    (*return_tuples) = pgr_alloc(
            remaining_vertices.size() + shortcut_edges.size(),
            (*return_tuples));

    size_t sequence = 0;

    for (auto id : remaining_vertices) {
        int64_t* contracted_vertices = NULL;
        auto ids = graph.get_contracted_vertices(id);
        contracted_vertices = pgr_alloc(
                   ids.size(), contracted_vertices);
        int count = 0;
        for (const auto id : ids) {
            contracted_vertices[count++] = id;
        }
        (*return_tuples)[sequence] = {id, const_cast<char*>("v"), -1, -1, -1.00,
            contracted_vertices, count};

        ++sequence;
    }

    for (auto edge : shortcut_edges) {
        int64_t* contracted_vertices = NULL;
        auto ids = graph.get_ids(edge.contracted_vertices());

        contracted_vertices = pgr_alloc(
                   ids.size(), contracted_vertices);
        int count = 0;
        for (const auto id : ids) {
            contracted_vertices[count++] = id;
        }
        (*return_tuples)[sequence] = {edge.id, const_cast<char*>("e"),
            edge.source, edge.target, edge.cost,
            contracted_vertices, count};

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
        contracted_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(total_edges != 0);
        pgassert(size_contraction_order != 0);
        pgassert(max_cycles != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        std::ostringstream debug;
        /*
         * Converting to C++ structures
         */
        std::vector<pgr_edge_t> edges(data_edges, data_edges + total_edges);
        std::vector<int64_t> forbid(
                forbidden_vertices,
                forbidden_vertices + size_forbidden_vertices);
        std::vector<int64_t> ordering(
                contraction_order,
                contraction_order + size_contraction_order);

        for (const auto o : ordering) {
            if (!is_valid_contraction(o)) {
                *err_msg = pgr_msg("Invalid Contraction Type found");
                log << "Contraction type " << o << " not valid";
                *log_msg = pgr_msg(log.str().c_str());
                return;
            }
        }


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
        log << "directed " << directed << "\n";
#endif

        graphType gType = directed? DIRECTED: UNDIRECTED;
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::CHDirectedGraph digraph(gType);

            process_contraction(digraph, edges, forbid, ordering,
                    max_cycles,
                    remaining_vertices, shortcut_edges,
                    log, err);

            get_postgres_result(
                    digraph,
                    remaining_vertices,
                    shortcut_edges,
                    return_tuples);
        } else {
            log << "Working with Undirected Graph\n";

            pgrouting::CHUndirectedGraph undigraph(gType);
            process_contraction(undigraph, edges, forbid, ordering,
                    max_cycles,
                    remaining_vertices, shortcut_edges,
                    log, err);

            get_postgres_result(
                    undigraph,
                    remaining_vertices,
                    shortcut_edges,
                    return_tuples);
        }

        (*return_count) = remaining_vertices.size()+shortcut_edges.size();


        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
