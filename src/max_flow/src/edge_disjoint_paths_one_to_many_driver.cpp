/*PGR-GNU*****************************************************************
File: edge_disjoint_paths_one_to_many_driver.cpp

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

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <vector>
#include <set>

#include "pgr_edgedisjointpaths.hpp"
#include "./edge_disjoint_paths_one_to_many_driver.h"
#include "../../common/src/pgr_alloc.hpp"

// #define DEBUG
extern "C" {
#include "./../../common/src/pgr_types.h"
}


void
do_pgr_edge_disjoint_paths_one_to_many(
    pgr_basic_edge_t *data_edges,
    size_t total_tuples,
    int64_t source_vertex,
    int64_t *sink_vertices,
    size_t size_sink_verticesArr,
    bool directed,
    General_path_element_t **return_tuples,
    size_t *return_count,
    char **err_msg) {
    std::ostringstream log;

    try {
        std::vector<General_path_element_t> path_elements;
        std::set<int64_t> set_source_vertices;
        set_source_vertices.insert(source_vertex);
        std::set<int64_t> set_sink_vertices;
        for (size_t i = 0; i < size_sink_verticesArr; ++i) {
            set_sink_vertices.insert(sink_vertices[i]);
        }
        PgrEdgeDisjointPathsGraph<FlowGraph> G;

        // I use a directed graph since I'm dependant on directed graphs

        G.create_edge_disjoint_paths_graph(data_edges, total_tuples,
                                           set_source_vertices,
                                           set_sink_vertices, directed);
        int64_t flow = G.boykov_kolmogorov();
        G.get_edge_disjoint_paths(path_elements, flow);

        (*return_tuples) = pgr_alloc(path_elements.size(), (*return_tuples));
        for (size_t i = 0; i < path_elements.size(); ++i) {
            (*return_tuples)[i] = path_elements[i];
        }
        *return_count = path_elements.size();

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





