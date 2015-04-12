#include "./vrp_assert.h"
#include "./signalhandler.h"
#include "./basePath_SSEC.hpp"

template < class G >
void Pgr_ksp< G >::clear() {
        m_Heap.clear();
}

template < class G >
void Pgr_ksp< G >::getFirstSolution() {
     int result_size;
     Path path;

     this->process_dijkstra(path, m_start, m_end);

     if (path.path.size() <= 1 ) return;
     curr_result_path = path;
     m_ResultSet.insert(curr_result_path);
}

template < class G>
std::deque<Path> Pgr_ksp< G >::Yen(int64_t  start_vertex, int64_t  end_vertex, int K) {

    std::deque<Path> l_ResultList;
    if ((start_vertex != end_vertex) && (K > 0)) {
        if (!this->get_gVertex(start_vertex, v_source) or !this->get_gVertex(end_vertex, v_target)){
             return l_ResultList;
        }
        m_start = start_vertex;
        m_end = end_vertex;
        executeYen(K);

        }

        while (m_Heap.size()){
             curr_result_path = *m_Heap.begin();
             m_ResultSet.insert(curr_result_path);
             m_Heap.erase(m_Heap.begin());
        }

        while (m_ResultSet.size()){
             l_ResultList.push_back((*m_ResultSet.begin()));
             m_ResultSet.erase(m_ResultSet.begin());
        }
        return l_ResultList;
}


template < class G >
void Pgr_ksp< G >::removeVertices(const Path &subpath) {
    for (int i = 0; i < subpath.path.size(); i++) 
       this->disconnect_vertex(subpath.path[i].source);
}

template < class G >
void Pgr_ksp< G >::doNextCycle() {
        REG_SIGINT


        int64_t spurNodeId;
        Path rootPath;
        Path spurPath;

        for (unsigned int i = 0; i < curr_result_path.path.size() ; ++i) {

            int64_t  spurEdge = curr_result_path.path[i].edge;
            spurNodeId = curr_result_path.path[i].source;
             
            rootPath = curr_result_path.getSubpath(i);

            // when spur node has parallels to the end of the spur edge then
            // no dijkstra has to be done because:
            //  NewPath = rootpath + Paralleledgeof(supredge) rest of the curr_result path
            // is the shortest path using the paralel edge
            for (pIt = m_ResultSet.begin(); pIt != m_ResultSet.end(); ++pIt) {
               if ((*pIt).isEqual(rootPath)) {
                   // edge to be removed = (*pIt).path[i].edge;
                  this->disconnect_edge((*pIt).path[i].source, (*pIt).path[i+1].source);
               }
            }
            removeVertices(rootPath);

            int spurPathSize;

            THROW_ON_SIGINT
            this->process_dijkstra(spurPath, spurNodeId , m_end);
            THROW_ON_SIGINT

            if (spurPath.path.size() > 0) {
                rootPath.appendPath(spurPath);
                m_Heap.insert(rootPath);
            }

            this->restore_graph();
            rootPath.clear();
            spurPath.clear();
        }
}

template < class G >
void Pgr_ksp< G >::executeYen(int K) {
          clear();
          getFirstSolution();

          if (m_ResultSet.size() == 0) return;  // no path found

          while ( m_ResultSet.size() < (unsigned int) K ) {
                doNextCycle();
                if ( m_Heap.size() == 0 ) break;
                curr_result_path = *m_Heap.begin();
                m_ResultSet.insert(curr_result_path);
                m_Heap.erase(m_Heap.begin());
          }
}
