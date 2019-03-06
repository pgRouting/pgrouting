/*PGR-GNU*****************************************************************
File: pgr_dijkstraVia.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo

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

#ifndef INCLUDE_DIJKSTRA_PGR_DIJKSTRAVIA_HPP_
#define INCLUDE_DIJKSTRA_PGR_DIJKSTRAVIA_HPP_
#pragma once

#include <sstream>
#include <deque>
#include <vector>

#include "dijkstra/pgr_dijkstra.hpp"

#include "cpp_common/pgr_assert.h"


namespace pgRouting {

template <class G>
void
pgr_dijkstraVia(
        G &graph,
        const std::vector< int64_t > &via_vertices,
        std::deque< Path > &paths,
        bool strict,
        bool U_turn_on_edge,
        std::ostringstream &log) {
    if (via_vertices.size() == 0) {
        return;
    }

    paths.clear();
    int64_t prev_vertex = via_vertices[0];
    Path path;

    int64_t i = 0;
    for (const auto &vertex : via_vertices) {
        if (i == 0) {
            prev_vertex = vertex; ++i;
            continue;
        }

        // Delete U Turn edges only valid for paths that are not the first path
        if (!U_turn_on_edge && i > 1) {
            /*
             * Can only delete if there was a path,
             * that is at least one edge size
             */
            if (path.size() > 1) {
                /*
                 * Delete from the graph the last edge if its outgoing also
                 * edge to be removed = second to last edge path[i].edge;
                 */
                int64_t edge_to_be_removed = path[path.size() - 2].edge;
                int64_t last_vertex_of_path = prev_vertex;

                // and the current vertex is not a dead end
                if (graph.out_degree(last_vertex_of_path) > 1) {
                    log << "\ndeparting from " << last_vertex_of_path
                        << " deleting edge " << edge_to_be_removed << "\n";
                    graph.disconnect_out_going_edge(
                            last_vertex_of_path,
                            edge_to_be_removed);
                }
            }
        }

        log << "\nfrom " << prev_vertex << " to " << vertex;
        path = pgr_dijkstra(graph, prev_vertex, vertex);

        if (!U_turn_on_edge && i > 1) {
            graph.restore_graph();
            if (path.empty()) {
                /*
                 *  no path was found with the deleted edge
                 *  try with the edge back in the graph
                 */
                log << "\nEmpty so again from "
                    << prev_vertex << " to " << vertex;
                path = pgr_dijkstra(graph, prev_vertex, vertex);
            }
        }

        if (strict && path.empty()) {
            paths.clear();
            return;
        }
        paths.push_back(path);

        /*
         * got to the next
         */
        prev_vertex = vertex; ++i;
    }
}


}  // namespace pgRouting

#endif  // INCLUDE_DIJKSTRA_PGR_DIJKSTRAVIA_HPP_
