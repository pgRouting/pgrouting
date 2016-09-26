/*PGR-GNU*****************************************************************
File: maximum_cardinality_matching_driver.cpp

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

#include "./pgr_maximumcardinalitymatching.hpp"
#include "./maximum_cardinality_matching_driver.h"
#include "../../common/src/pgr_alloc.hpp"

// #define DEBUG
extern "C" {
#include "./../../common/src/pgr_types.h"
}

void
do_pgr_maximum_cardinality_matching(
    pgr_basic_edge_t *data_edges,
    bool directed,
    size_t total_tuples,
    pgr_basic_edge_t **return_tuples,
    size_t *return_count,
    char **err_msg) {
    std::ostringstream log;

    try {
        std::vector<pgr_basic_edge_t> matched_vertices;

        if (directed) {
            PgrCardinalityGraph<BasicDirectedGraph> G;
            G.create_max_cardinality_graph(data_edges, total_tuples);
            std::vector<int64_t> mate_map(boost::num_vertices(G.boost_graph));
            G.maximum_cardinality_matching(mate_map);
            G.get_matched_vertices(matched_vertices, mate_map);
        } else {
            PgrCardinalityGraph<BasicUndirectedGraph> G;
            G.create_max_cardinality_graph(data_edges, total_tuples);
            std::vector<int64_t> mate_map(boost::num_vertices(G.boost_graph));
            G.maximum_cardinality_matching(mate_map);
            G.get_matched_vertices(matched_vertices, mate_map);
        }

        (*return_tuples) = pgr_alloc(matched_vertices.size(), (*return_tuples));
        for (size_t i = 0; i < matched_vertices.size(); ++i) {
            (*return_tuples)[i] = matched_vertices[i];
        }
        *return_count = matched_vertices.size();

#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif

        return;
    } catch (...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
        return;
    }
}





