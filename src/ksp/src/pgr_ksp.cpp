/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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
*/

#include <deque>
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

     if (path.path.size() <= 1 ) return;
     curr_result_path = path;
     m_ResultSet.insert(curr_result_path);
}

template < class G>
std::deque<Path>
Pgr_ksp< G >::Yen(G &graph, 
  int64_t  start_vertex, int64_t end_vertex, int K) {
    std::deque<Path> l_ResultList;
    if ((start_vertex != end_vertex) && (K > 0)) {
        if   (!graph.get_gVertex(start_vertex, v_source)
           || !graph.get_gVertex(end_vertex, v_target)) {
             return l_ResultList;
        }
        m_start = start_vertex;
        m_end = end_vertex;
        executeYen(graph, K);
    }

    while (m_Heap.size()) {
         curr_result_path = *m_Heap.begin();
         m_ResultSet.insert(curr_result_path);
         m_Heap.erase(m_Heap.begin());
    }

    while (m_ResultSet.size()) {
         l_ResultList.push_back((*m_ResultSet.begin()));
         m_ResultSet.erase(m_ResultSet.begin());
    }
    return l_ResultList;
}


template < class G >
void Pgr_ksp< G >::removeVertices(G &graph, const Path &subpath) {
    for (unsigned int i = 0; i < subpath.path.size(); i++)
       graph.disconnect_vertex(subpath.path[i].vertex);
}

template < class G >
void Pgr_ksp< G >::doNextCycle(G &graph) {
    // REG_SIGINT


    int64_t spurNodeId;
    Path rootPath;
    Path spurPath;

    for (unsigned int i = 0; i < curr_result_path.path.size() ; ++i) {
        // int64_t  spurEdge = curr_result_path.path[i].edge;
        spurNodeId = curr_result_path.path[i].vertex;

        rootPath = curr_result_path.getSubpath(i);

        for (pIt = m_ResultSet.begin(); pIt != m_ResultSet.end(); ++pIt) {
           if ((*pIt).isEqual(rootPath)) {
              // edge to be removed = (*pIt).path[i].edge;
              graph.disconnect_edge((*pIt).path[i].vertex,     // from
                                    (*pIt).path[i+1].vertex);  // to
           }
        }
        removeVertices(graph, rootPath);

        // int spurPathSize;

        // THROW_ON_SIGINT
        Pgr_dijkstra< G > fn_dijkstra;
        fn_dijkstra.dijkstra(graph, spurPath, spurNodeId, m_end);
        //this->dijkstra(spurPath, spurNodeId , m_end);
        // THROW_ON_SIGINT

        if (spurPath.path.size() > 0) {
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
                if ( m_Heap.size() == 0 ) break;
                curr_result_path = *m_Heap.begin();
                m_ResultSet.insert(curr_result_path);
                m_Heap.erase(m_Heap.begin());
          }
}
