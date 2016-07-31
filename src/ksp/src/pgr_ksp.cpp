/*PGR-GNU*****************************************************************
File: pgr_ksp.cpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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
#ifdef unlink
#undef unlink
#endif
#endif


#include <deque>
#include <set>
#include "./../../common/src/basePath_SSEC.hpp"

template < class G >
void Pgr_ksp< G >::clear() {
        m_Heap.clear();
}

template < class G >
void Pgr_ksp< G >::getFirstSolution(G &graph) {
     Path path;

     Pgr_dijkstra< G > fn_dijkstra;
     fn_dijkstra.dijkstra(graph, path, m_start, m_end);

     if (path.empty()) return;
     curr_result_path = path;
     m_ResultSet.insert(curr_result_path);
}

template < class G>
std::deque<Path>
Pgr_ksp< G >::Yen(G &graph,
  int64_t  start_vertex, int64_t end_vertex, int K, bool heap_paths) {
    m_ResultSet.clear();
    m_Heap.clear();

    if ((start_vertex != end_vertex) && (K > 0)) {
        if (!graph.has_vertex(start_vertex)
                || !graph.has_vertex(end_vertex)) {
            std::deque<Path> l_ResultList;
            return l_ResultList;
        }
        v_source = graph.get_V(start_vertex);
        v_target = graph.get_V(end_vertex);
        m_start = start_vertex;
        m_end = end_vertex;
        executeYen(graph, K);
    }

    while (!m_ResultSet.empty()) {
        m_Heap.insert(*m_ResultSet.begin());
        m_ResultSet.erase(m_ResultSet.begin());
    }
    std::deque<Path> l_ResultList(m_Heap.begin(), m_Heap.end());
    if (!heap_paths && l_ResultList.size() > (size_t) K)
        l_ResultList.resize(K);
    return l_ResultList;
}


template < class G >
void Pgr_ksp< G >::removeVertices(G &graph, const Path &subpath) {
    for (const auto &e : subpath)
        graph.disconnect_vertex(e.node);
}

template < class G >
void Pgr_ksp< G >::doNextCycle(G &graph) {
    // REG_SIGINT


    int64_t spurNodeId;
    Path rootPath;
    Path spurPath;

    for (unsigned int i = 0; i < curr_result_path.size() ; ++i) {
        spurNodeId = curr_result_path[i].node;

        rootPath = curr_result_path.getSubpath(i);

        for (const auto &path : m_ResultSet) {
            if (path.isEqual(rootPath)) {
                if (path.size() > i + 1) {
                    graph.disconnect_edge(path[i].node,     // from
                            path[i + 1].node);  // to
                }
            }
        }
        removeVertices(graph, rootPath);


        // THROW_ON_SIGINT
        Pgr_dijkstra< G > fn_dijkstra;
        fn_dijkstra.dijkstra(graph, spurPath, spurNodeId, m_end);
        // this->dijkstra(spurPath, spurNodeId , m_end);
        // THROW_ON_SIGINT

        if (spurPath.size() > 0) {
            rootPath.appendPath(spurPath);
            m_Heap.insert(rootPath);
        }

        graph.restore_graph();
        rootPath.clear();
        spurPath.clear();
    }
}

template < class G >
void Pgr_ksp< G >::executeYen(G &graph, int K) {
    clear();
    getFirstSolution(graph);

    if (m_ResultSet.size() == 0) return;  // no path found

    while ( m_ResultSet.size() < (unsigned int) K ) {
        doNextCycle(graph);
        if ( m_Heap.empty() ) break;
        curr_result_path = *m_Heap.begin();
        m_ResultSet.insert(curr_result_path);
        m_Heap.erase(m_Heap.begin());
    }
}
