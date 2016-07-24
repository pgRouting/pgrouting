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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include <sstream>
#include <iomanip>
#include <vector>
#include "./test_c_edges_driver.h"

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./../../common/src/pgr_base_graph.hpp"
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

        std::vector< pgr_edge_t > edges(data_edges, data_edges + total_edges);
        std::vector < pgrouting::contraction::Vertex > vertices(pgrouting::contraction::extract_vertices(edges));


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
            pgrouting::CDirectedGraph graph(DIRECTED);
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
            pgrouting::CDirectedGraph graph(DIRECTED);
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
            pgrouting::CDirectedGraph graph(vertices, DIRECTED);
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
            pgrouting::CDirectedGraph graph(vertices, DIRECTED);
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
            pgrouting::CDirectedGraph graph(UNDIRECTED);
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
            pgrouting::CDirectedGraph graph(UNDIRECTED);
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
            pgrouting::CDirectedGraph graph(UNDIRECTED);
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
            pgrouting::CDirectedGraph graph(vertices, UNDIRECTED);
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
            log << "Testing Identifiers, creating with vertices, insertion using vector\n";
            log << "  - Created graph:\n";
            pgrouting::CDirectedGraph graph(vertices, UNDIRECTED);
            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(edges);
            log << "  - All vertices:\n";
            Identifiers<int64_t> all_vertices, contracted_vertices, remaining_vertices;
            for (const auto vertex: vertices) {
                all_vertices.insert(graph.get_V(vertex.id));
                //log << vertex;
            }
            log << "    " << all_vertices;
            log << "\n";
            log << "  - Contracted vertices:\n";
            /*
             1, 7, 8, 13, 14, 16
            */
            contracted_vertices.insert(graph.get_V(1));
            contracted_vertices.insert(graph.get_V(7));
            contracted_vertices.insert(graph.get_V(8));
            contracted_vertices.insert(graph.get_V(13));
            contracted_vertices.insert(graph.get_V(14));
            contracted_vertices.insert(graph.get_V(16));
            log << "    " << contracted_vertices;
            log << "\n";
            log << "  - Remaining vertices:\n";
            remaining_vertices = all_vertices - contracted_vertices;
            log << "    " << remaining_vertices;
            log << "\n";
            #if 0
            
            Pgr_contract<pgrouting::CDirectedGraph>  contractor;
            contractor.getDeadEndSet(graph);
            log << "  - Dead end vertices:\n";
            contractor.print_dead_end_vertices(log);

            #endif
        }

        {
            log << "Testing Vertex class, creating graph with vertices, insertion using vector\n";
            log << "  - Created graph:\n";
            pgrouting::CDirectedGraph graph(vertices, UNDIRECTED);
            log << "  - Inserting Edges:\n";
            graph.graph_insert_data(edges);
            int64_t vid1 = graph.get_V(1);
            int64_t vid2 = graph.get_V(2);
            int64_t vid5 = graph.get_V(5);
            int64_t vid4 = graph.get_V(4);
            int64_t vid7 = graph.get_V(7);
            int64_t vid8 = graph.get_V(8);
            int64_t vid10 = graph.get_V(10);
            int64_t vid12 = graph.get_V(12);
            int64_t vid13 = graph.get_V(13);
            int64_t vid14 = graph.get_V(14);
            int64_t vid15 = graph.get_V(15);
            int64_t vid16 = graph.get_V(16);
            int64_t vid17 = graph.get_V(17);

            log << "  - id ----- V:\n";
            log << "  " << 1 << " ----- " << vid1 << "\n";
            log << "  " << 2 << " ----- " << vid2 << "\n";
            log << "  " << 4 << " ----- " << vid4 << "\n";
            log << "  " << 5 << " ----- " << vid5 << "\n";
            log << "  " << 7 << " ----- " << vid7 << "\n";
            log << "  " << 8 << " ----- " << vid8 << "\n";
            log << "  " << 10 << " ----- " << vid10 << "\n";
            log << "  " << 12 << " ----- " << vid12 << "\n";
            log << "  " << 13 << " ----- " << vid13 << "\n";
            log << "  " << 14 << " ----- " << vid14 << "\n";
            log << "  " << 15 << " ----- " << vid15 << "\n";
            log << "  " << 16 << " ----- " << vid16 << "\n";
            log << "  " << 17 << " ----- " << vid17 << "\n";
            pgrouting::contraction::Vertex v1 = graph[vid1];
            pgrouting::contraction::Vertex v2 = graph[vid2];
            pgrouting::contraction::Vertex v4 = graph[vid4];
            pgrouting::contraction::Vertex v5 = graph[vid5];
            pgrouting::contraction::Vertex v7 = graph[vid7];
            pgrouting::contraction::Vertex v8 = graph[vid8];
            pgrouting::contraction::Vertex v10 = graph[vid10];
            pgrouting::contraction::Vertex v12 = graph[vid12];
            pgrouting::contraction::Vertex v13 = graph[vid13];
            pgrouting::contraction::Vertex v14 = graph[vid14];
            pgrouting::contraction::Vertex v15 = graph[vid15];
            pgrouting::contraction::Vertex v16 = graph[vid16];
            pgrouting::contraction::Vertex v17 = graph[vid17];

            log << "  - Dead end contraction:\n";
            // vertex 1 is contracted to vertex 2
            v2.add_contracted_vertex(v1, vid1);
            log << "Vertex 1 is contracted to Vertex 2:\n";
            log << v1;
            log << v2;
            // vertex 7 is contracted to vertex 8
            v8.add_contracted_vertex(v7, vid7);
            log << "Vertex 7 is contracted to Vertex 8:\n";
            log << v7;
            log << v8;
            // vertex 8 is contracted to vertex 5
            v5.add_contracted_vertex(v8, vid8);
            log << "Vertex 8 is contracted to Vertex 5:\n";
            log << v8;
            log << v5;
            // vertex 13 is contracted to vertex 10
            v10.add_contracted_vertex(v13, vid13);
            log << "Vertex 13 is contracted to Vertex 10:\n";
            log << v13;
            log << v10;
            // vertex 14 is contracted to vertex 15
            v15.add_contracted_vertex(v14, vid14);
            log << "Vertex 14 is contracted to Vertex 15:\n";
            log << v14;
            log << v15;
            // vertex 16 is contracted to vertex 17
            v17.add_contracted_vertex(v16, vid16);
            log << "Vertex 16 is contracted to Vertex 17:\n";
            log << v16;
            log << v17;


            log << "  - Linear contraction:\n";
            pgrouting::contraction::Edge e1;
            pgrouting::contraction::Edge e2;
            pgrouting::contraction::Edge e3;
            pgrouting::contraction::Edge e4;
            // vertex 2 is contracted to edge -1
            e1.id = -1;
            e1.source = 3;
            e1.target = 5;
            log << "Vertex 1 is contracted to edge -1:\n";
            e1.add_contracted_vertex(v2, vid2);
            log << e1;
            // vertex 4 is contracted to edge -2
            e2.id = -2;
            e2.source = 3;
            e2.target = 9;
            log << "Vertex 4 is contracted to edge -2:\n";
            e2.add_contracted_vertex(v4, vid4);
            log << e2;
            // vertex 10 is contracted to edge -3
            e3.id = -3;
            e3.source = 5;
            e3.target = 11;
            log << "Vertex 10 is contracted to edge -3:\n";
            e3.add_contracted_vertex(v10, vid10);
            log << e3;
            // vertex 12 is contracted to edge -4
            e4.id = -4;
            e4.source = 9;
            e4.target = 11;
            log << "Vertex 12 is contracted to edge -4:\n";
            e4.add_contracted_vertex(v12, vid12);
            log << e4;
            #if 0
            
            Pgr_contract<pgrouting::CDirectedGraph>  contractor;
            contractor.getDeadEndSet(graph);
            log << "  - Dead end vertices:\n";
            contractor.print_dead_end_vertices(log);

            #endif
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
