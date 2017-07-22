/*PGR-GNU*****************************************************************
File: lineGraph_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307.com

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

#include "dijkstra/pgr_dijkstra.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "lineGraph/pgr_lineGraph.hpp"

void get_postgres_result(
        std::vector< Line_graph_rt > edge_result,
        Line_graph_rt **return_tuples) {
    (*return_tuples) = pgr_alloc(
            edge_result.size(),
            (*return_tuples));

    size_t seq = 0;
    for (auto edge: edge_result) {
        (*return_tuples)[seq] = edge;
        ++seq;
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

        std::vector< Line_graph_rt > results;
        graphType gType = directed?DIRECTED:UNDIRECTED;
        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            digraph.m_num_vertices = 1000;
            log << digraph << "\n";

            pgrouting::LinearDirectedGraph line(gType);
            line.insert_vertices(data_edges, total_edges);
            auto line_graph_edges = line.transform(digraph);
            /*line.insert_edges(line_graph_edges);


            get_postgres_result(
                line_graph_edges,
                return_tuples
            );
            (*return_count) = line_graph_edges.size();*/
            log << line.log.str().c_str();
        }

    #if 0
        if (directed) {
            //log << "\nNum of vertices: " << digraph.num_vertices() << "\n";
            log << "\nDirected Graph: \n" << digraph;
            line.process(digraph, log);
            /*log << "\nNum of vertices: " << digraph.num_vertices() << "\n";
            //log << "\nDirected Graph :\n" << digraph;
            for (auto it: digraph.vertices_map) {
                log << (it.first) << " " << (it.second) << "\n";
            }
            int64_t c = 0;
            log << "\nNum vertices = " << digraph.m_num_vertices << "\n";
            for (auto vi = boost::vertices(digraph.graph).first;
                    vi != boost::vertices(digraph.graph).second; ++vi) {
                log << (*vi) << ": " << " out_edges_of(" << digraph.graph[(*vi)] << "):\n";
                c++;
            }
            log << "\n" << c;*/
        }
        std::vector<pgr_edge_t> edges(data_edges, data_edges + total_edges);
        if (!directed) {
            for (int64_t i = 0; i < total_edges;i++) {
                if (data_edges[i].reverse_cost >= 0) {
                    std::swap(data_edges[i].cost, data_edges[i].reverse_cost);
                }
                std::swap(data_edges[i].source, data_edges[i].target);
                edges.push_back( data_edges[i] );
            }
        }
        digraph.insert_edges(edges);

        log << "\nDirected Graph :\n" << digraph;

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found between start_vid and end_vid vertices";
            return;
        }
        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        size_t sequence = 0;
        path.generate_postgres_data(return_tuples, sequence);
        (*return_count) = sequence;
#endif
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
