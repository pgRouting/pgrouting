/*PGR-GNU*****************************************************************
File: max_flow_edmonds_karp_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "./max_flow_edmonds_karp_driver.h"

#include <sstream>
#include <vector>

#include "./pgr_maxflow.h"
#include "../../common/src/pgr_alloc.hpp"

// #define DEBUG

extern "C" {
}

void
do_pgr_max_flow_edmonds_karp(
    pgr_edge_t *data_edges,
    size_t total_tuples,
    int64_t source_vertex,
    int64_t sink_vertex,
    pgr_flow_t **return_tuples,
    size_t *return_count,
    char **err_msg) {
    std::ostringstream log;

    try {
        PgrFlowGraph<FlowGraph> G;
        G.create_flow_graph(data_edges, total_tuples, source_vertex, sink_vertex);

        int64_t flow = G.edmonds_karp();

        std::vector<pgr_flow_t> flow_edges = G.get_flow_edges();

        (*return_tuples) = pgr_alloc(flow_edges.size(), (*return_tuples));
        for (int i = 0; i < flow_edges.size(); ++i) {
            (*return_tuples)[i] = flow_edges[i];
        }
        *return_count = flow_edges.size();

#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif

        return;
    } catch (...) {
        log << "Caught unknown expection!\n";
        *err_msg = strdup(log.str().c_str());
        return;
    }
}





