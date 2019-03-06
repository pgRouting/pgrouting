/*PGR-GNU*****************************************************************
File: lineGraph_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

#include "drivers/lineGraph/lineGraph_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <utility>

#include "dijkstra/pgr_dijkstra.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "lineGraph/pgr_lineGraph.hpp"
#include "cpp_common/linear_directed_graph.h"


void get_postgres_result(
        std::vector< Line_graph_rt > edge_result,
        Line_graph_rt **return_tuples,
        size_t &sequence) {
    (*return_tuples) = pgr_alloc(
            static_cast<int>(edge_result.size()),
            (*return_tuples));

    for (const auto &edge : edge_result) {
        (*return_tuples)[sequence] = {edge.id, edge.source, edge.target,
             edge.cost, edge.reverse_cost};
        sequence++;
    }
}

void
do_pgr_lineGraph(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        bool directed,
        Line_graph_rt **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        pgrouting::DirectedGraph digraph(gType);
        digraph.insert_edges_neg(data_edges, total_edges);

        log << digraph << "\n";
        pgrouting::graph::Pgr_lineGraph<
            pgrouting::LinearDirectedGraph,
            pgrouting::Line_vertex,
            pgrouting::Basic_edge> line(digraph);
        std::vector< Line_graph_rt > line_graph_edges;
        line_graph_edges = line.get_postgres_results_directed();
        auto count = line_graph_edges.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "Only vertices graph";
        } else {
            size_t sequence = 0;

            get_postgres_result(
                line_graph_edges,
                return_tuples,
                sequence);
            (*return_count) = sequence;
        }

        pgassert(*err_msg == NULL);
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
