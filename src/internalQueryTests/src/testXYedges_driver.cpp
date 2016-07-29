/*PGR-GNU*****************************************************************
File: testXYedges_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
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
#endif

#include <sstream>
#include <iomanip>
#include <vector>
#include "./testXYedges_driver.h"


#define DEBUG
#include "./../../common/src/pgr_types.h"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./../../common/src/pgr_base_graph.hpp"
#include "./../../common/src/pgr_assert.h"

/************************************************************
  edges_sql TEXT
 ***********************************************************/
bool
do_pgr_testXYedges(
        Pgr_edge_xy_t *data_edges,
        size_t total_edges,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));

        std::vector< Pgr_edge_xy_t > edges(data_edges, data_edges + total_edges);
        auto vertices(pgrouting::extract_vertices(edges));


        log << "Original: \n" <<
            std::setprecision(32);
        for (const auto edge : edges) {
            log << "id = " << edge.id
                << "\tsource = " << edge.source
                << "\ttarget = " << edge.target
                << "\tcost = " << edge.cost
                << "\treverse_cost = " << edge.reverse_cost
                << "\n\t(x1,y1) = (" << edge.x1 << "," << edge.y1 << ")"
                << "\t(x2,y2) = (" << edge.x2 << "," << edge.y2 << ")\n";
        }

        {
            log << "Testing Directed ,  insertion using vector\n";
            pgrouting::xyDirectedGraph graph(DIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }
        {
            log << "Testing Directed ,  insertion using C array\n";
            pgrouting::xyDirectedGraph graph(DIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(data_edges, total_edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }
        {
            log << "Testing Directed ,  creating with vertices, insertion using vector\n";
            pgrouting::xyDirectedGraph graph(vertices, DIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }
        {
            log << "Testing Directed ,  creating with vertices, insertion using C array\n";
            pgrouting::xyDirectedGraph graph(vertices, DIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(data_edges, total_edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }

        {
            log << "Testing Undirected ,  insertion using vector\n";
            pgrouting::xyUndirectedGraph graph(UNDIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }
        {
            log << "Testing Directed ,  insertion using C array\n";
            pgrouting::xyUndirectedGraph graph(UNDIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(data_edges, total_edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }
        {
            log << "Testing Undirected ,  insertion using C array\n";
            pgrouting::xyUndirectedGraph graph(UNDIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(data_edges, total_edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }
        {
            log << "Testing Undirected ,  creating with vertices, insertion using vector\n";
            pgrouting::xyUndirectedGraph graph(vertices, UNDIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            Path path;
            pgr_dijkstra(graph, path, 2, 3, true);
        }

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());
        return true;
    } catch (AssertFailedException &exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    } catch (std::exception& exept) {
        log << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    } catch(...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    }
}
