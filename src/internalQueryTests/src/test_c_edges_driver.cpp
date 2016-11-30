/*PGR-GNU*****************************************************************
File: testXYedges_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Rohith Reddy
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

#include "./test_c_edges_driver.h"

#include <sstream>
#include <iomanip>
#include <vector>

#include "./../../contraction/src/pgr_contractionGraph.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_assert.h"

/************************************************************
  edges_sql TEXT
 ***********************************************************/
bool
do_pgr_test_c_edges(
        pgr_edge_t *data_edges,
        size_t total_edges,
        char ** log_msg,
        char ** err_msg){
    std::ostringstream log;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));

        std::vector<pgr_edge_t> edges(data_edges, data_edges + total_edges);
        auto vertices(pgrouting::extract_vertices(edges));

        log << "Original: \n" <<
            std::setprecision(9);
        for (const auto edge: edges) {
            log << "id = " << edge.id
                << "\tsource = " << edge.source
                << "\ttarget = " << edge.target
                << "\tcost = " << edge.cost
                << "\treverse_cost = " << edge.reverse_cost
                << ")\n";
        }

        {
            log << "Testing Directed ,  insertion using vector\n";
            pgrouting::CHDirectedGraph graph(DIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.insert_edges(edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            pgr_dijkstra(graph, 2, 3, true);

        }
        
        {
            log << "Testing Directed ,  insertion using C array\n";
            pgrouting::CHDirectedGraph graph(DIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.insert_edges(data_edges, total_edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            pgr_dijkstra(graph, 2, 3, true);

        }

        {
            log << "Testing Undirected ,  insertion using vector\n";
            pgrouting::CHUndirectedGraph graph(UNDIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.insert_edges(edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            pgr_dijkstra(graph, 2, 3, true);
        }

        {
            log << "Testing Undirected ,  insertion using C array\n";
            pgrouting::CHUndirectedGraph graph(UNDIRECTED);
            log << "  - Created graph:\n";
            log << graph;

            log << "  - Inserting Edges:\n";
            graph.insert_edges(data_edges, total_edges);
            log << graph;

            log << "  - Can do a dijKstra:\n";
            pgr_dijkstra(graph, 2, 3, true);
        }

        {
            log << "Testing Identifiers\n";
            pgrouting::CHUndirectedGraph graph(UNDIRECTED);
            graph.insert_edges(edges);

            Identifiers<int64_t> all_vertices, contracted_vertices, remaining_vertices;

            for (const auto vertex: vertices) {
                all_vertices.insert(graph.get_V(vertex.id));
            }
            /*
               Contracted vertices:
               1, 7, 8, 13, 14, 16
               */
            contracted_vertices.insert(graph.get_V(1));
            contracted_vertices.insert(graph.get_V(7));
            contracted_vertices.insert(graph.get_V(8));
            contracted_vertices.insert(graph.get_V(13));
            contracted_vertices.insert(graph.get_V(14));
            contracted_vertices.insert(graph.get_V(16));

            remaining_vertices = all_vertices - contracted_vertices;

            log << "  - All vertices: "
                << all_vertices
                << "\n   - Contracted vertices: "
                << contracted_vertices
                << "\n  - Remaining vertices: "
                << remaining_vertices
                << "\n";
        }

        {
            log << "Testing CH_vertex class\n";

            pgrouting::CHUndirectedGraph graph(DIRECTED);
            graph.insert_edges(edges);

            log << "id ----- V\n";
            for (int64_t i = 1; i < 18; ++i) {
            log << "  " << i << " ----- " <<  graph.get_V(i) << "\n";
            }
            auto vid1 = graph.get_V(1);
            auto vid2 = graph.get_V(2);

            auto v1 = graph[vid1];
            auto v2 = graph[vid2];


            log << "  - Dead end contraction:\n";
            // vertex 1 is contracted to vertex 2
            v2.add_contracted_vertex(v1, vid1);
            log << "Vertex 1 is contracted to Vertex 2:\n";
            log << vid1 << v1 << "\n";
            log << vid2 << v2 << "\n";

            log << "  - Linear contraction:\n";
            pgrouting::CH_edge e1;
            pgrouting::CH_edge e2;
            pgrouting::CH_edge e3;
            pgrouting::CH_edge e4;

            // vertex 2 is contracted to edge -1
            log << "Vertex 1 is contracted to edge -1:\n";
            e1.id = -1;
            e1.cost = 0;
            e1.source = 3;
            e1.target = 5;
            e1.add_contracted_vertex(v2, vid2);
            log << vid2 << v2 << "\n";
            log << e1 << "\n";
            log << vid2 << v2 << "\n";
        }

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());
        return true;

    } catch (AssertFailedException &except) {
        log << except.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    } catch (std::exception& except) {
        log << except.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    } catch(...) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
        return false;
    }
}
