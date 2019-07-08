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
#include <algorithm>

#include "contraction/ch_graphs.hpp"
#include "contraction/pgr_contract.hpp"

#include "cpp_common/identifiers.hpp"
#include "cpp_common/pgr_alloc.hpp"

namespace {

/*! @brief vertices with at least one contracted vertex

  @result The vids Identifiers with at least one contracted vertex
*/
template <typename G>
Identifiers<int64_t> get_modified_vertices(const G& graph) {
    Identifiers<int64_t> vids;
    for (auto v : boost::make_iterator_range(boost::vertices(graph.graph))) {
        if (graph[v].has_contracted_vertices()) {
            vids += graph[v].id;
        }
    }
    return vids;
}

/*! @brief vertices with at least one contracted vertex

  @result The vids Identifiers with at least one contracted vertex
*/
template <typename G>
std::vector<typename G::E> get_shortcuts(const G& graph) {
    using E = typename G::E;
    Identifiers<E> eids;
    for (auto e : boost::make_iterator_range(boost::edges(graph.graph))) {
        if (graph[e].id < 0) {
            eids += e;
            pgassert(!graph[e].contracted_vertices().empty());
        } else {
            pgassert(graph[e].contracted_vertices().empty());
        }
    }
    std::vector<E> o_eids(eids.begin(), eids.end());
    std::sort(o_eids.begin(), o_eids.end(),
            [&](E lhs, E rhs) {return -graph[lhs].id < -graph[rhs].id;});
    return o_eids;
}


template <typename G>
void process_contraction(
        G &graph,
        const std::vector< pgr_edge_t > &edges,
        const std::vector< int64_t > &forbidden_vertices,
        const std::vector< int64_t > &contraction_order,
        int64_t max_cycles) {
    graph.insert_edges(edges);
    Identifiers<typename G::V> forbid_vertices;
    for (const auto &vertex : forbidden_vertices) {
        if (graph.has_vertex(vertex)) {
            forbid_vertices += graph.get_V(vertex);
        }
    }

    /*
     * Function call to get the contracted graph.
     */
    using Contract = pgrouting::contraction::Pgr_contract<G>;
    Contract result(
            graph,
            forbid_vertices,
            contraction_order,
            max_cycles);
}

template <typename G>
void get_postgres_result(
        G &graph,
        contracted_rt **return_tuples,
        size_t *count) {
    auto modified_vertices(get_modified_vertices(graph));
    auto shortcut_edges(get_shortcuts(graph));

    (*count) = modified_vertices.size() + shortcut_edges.size();
    (*return_tuples) = pgr_alloc((*count), (*return_tuples));
    size_t sequence = 0;

    for (const auto id : modified_vertices) {
        auto v = graph.get_V(id);
        int64_t* contracted_vertices = NULL;
        auto vids = graph[v].contracted_vertices();

        contracted_vertices = pgr_alloc(vids.size(), contracted_vertices);

        int count = 0;
        for (const auto id : vids) {
            contracted_vertices[count++] = id;
        }
        (*return_tuples)[sequence] = {
            id,
            const_cast<char*>("v"),
            -1, -1, -1.00,
            contracted_vertices,
            count};

        ++sequence;
    }

    int64_t eid = 0;
    for (auto e : shortcut_edges) {
        auto edge = graph[e];
        int64_t* contracted_vertices = NULL;

        const auto vids(edge.contracted_vertices());
        pgassert(!vids.empty());

        contracted_vertices = pgr_alloc(vids.size(), contracted_vertices);
        int count = 0;
        for (const auto vid : vids) {
            contracted_vertices[count++] = vid;
        }
        (*return_tuples)[sequence] = {
            --eid,
            const_cast<char*>("e"),
            edge.source, edge.target, edge.cost,
            contracted_vertices, count};
        ++sequence;
    }
}

}  // namespace



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

        for (const auto kind : ordering) {
            if (!pgrouting::contraction::is_valid_contraction(static_cast<int>(kind))) {
                *err_msg = pgr_msg("Invalid contraction type found");
                log << "Contraction type " << kind << " not valid";
                *log_msg = pgr_msg(log.str().c_str());
                return;
            }
        }


        graphType gType = directed? DIRECTED: UNDIRECTED;
        if (directed) {
            using DirectedGraph = pgrouting::graph::CHDirectedGraph;
            DirectedGraph digraph(gType);

            process_contraction(digraph, edges, forbid, ordering,
                    max_cycles);

            get_postgres_result(
                    digraph,
                    return_tuples,
                    return_count);
        } else {
            using UndirectedGraph = pgrouting::graph::CHUndirectedGraph;
            UndirectedGraph undigraph(gType);
            process_contraction(undigraph, edges, forbid, ordering,
                    max_cycles);

            get_postgres_result(
                    undigraph,
                    return_tuples,
                    return_count);
        }

        pgassert(err.str().empty());
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
