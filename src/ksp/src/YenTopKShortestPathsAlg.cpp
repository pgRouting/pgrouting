#include <deque>
#include <cassert>
#include <algorithm>
#include "GraphElements.h"
#include "DijkstraShortestPathAlg.h"
#include "YenTopKShortestPathsAlg.h"


void YenTopKShortestPathsAlg::clear() {
        DijkstraShortestPathAlg::clear();
        m_ResultList.clear();
        m_Heap.clear();
}

void YenTopKShortestPathsAlg::_init() {
        clear();
        BasePath shortestPath = Dijkstra(m_Source_id, m_Target_id);
        DijkstraShortestPathAlg::clear();
         if (!shortestPath.isEmpty()) { 
            m_ResultList.push_back(shortestPath);
            m_ResultSet.insert(shortestPath);
        }
}


std::deque<BasePath> YenTopKShortestPathsAlg::Yen(int  source, int  sink, int K) {
        clear();
        if (source !=sink && K > 0 && exist_vertex(source) && exist_vertex(sink)) {
             m_Source_id = source;
             m_Target_id = sink;
             sourceID = find_vertex(source);
             targetID = find_vertex(sink);
             get_shortest_paths(sourceID, targetID, K);
             //get_shortest_paths(find_vertex(source), find_vertex(sink), K);
        }
        m_ResultList.assign(m_ResultSet.begin(),m_ResultSet.end());
        return m_ResultList;
}


BasePath YenTopKShortestPathsAlg::get_shortest_path(UINT  sourcePos, UINT  targetPos) {
        int source_id = m_Vertices[ sourcePos ].getOriginalID();
        int target_id = m_Vertices[ targetPos ].getOriginalID();
        return Dijkstra(source_id,  target_id);
}


void YenTopKShortestPathsAlg::avoidDijkstra(UINT edgeToBeRemoved, UINT atPosOfPath, BasePath &workingPath) {
         //generate and store on the heap all paths that have parallel edges to edgeToBeRemoved
         UINT startId = m_Edges[edgeToBeRemoved].getStart();
         UINT endId = m_Edges[edgeToBeRemoved].getEnd();

         //std::deque<BaseEdge*> fanOut = m_Vertices[startId].getFanOut();
         std::deque<BaseEdge> fanOut;
         getFanOutActiveEdges(startId,fanOut);
         BasePath newPath; 
         for (UINT i = 0; i < fanOut.size(); i++) {
             if (edgeToBeRemoved == fanOut[i].ID()){
                remove_edge(edgeToBeRemoved);
                continue;
             }
             if ((fanOut[i].getStart() == startId) && (fanOut[i].getEnd() == endId)) {
                 // a new path can be formed with the parallel edge:
                 //  NewPath = rootpath + Paralleledgeof(edgeToBeRemoved) + rest of the curr_result path
                 workingPath.subPath(newPath, atPosOfPath); //copy the path up to the position
                 newPath.push_back(fanOut[i]);  //insert the paralel edge
                 for (UINT j = atPosOfPath+1; j < workingPath.size(); j++) {
		     newPath.push_back( workingPath[j] );
                 }
                 insertIntoHeap(newPath);
                 remove_edge(fanOut[i].ID());
             }
          }
};                 

void YenTopKShortestPathsAlg::next() {
        UINT currPathId =  m_ResultList.size()-1;
        BasePath curr_result_path = m_ResultList[ currPathId ];


        UINT spurNode;
        BasePath rootPath;
        for (UINT i = 0; i < curr_result_path.size(); ++i) {
            BaseEdge spurEdge (curr_result_path[i]);
            spurNode = spurEdge.getStart();

            curr_result_path.subPath(rootPath, i);

	    // when spur node has parallels to the end of the spur edge then 
            // no dijkstra has to be done because:
            //  NewPath = rootpath + Paralleledgeof(supredge) rest of the curr_result path
            // is the shortest path using the paralel edge
            
            UINT edgeToBeRemoved;
            for (UINT j=0; j < m_ResultList.size(); j++) {
               BasePath workingPath = m_ResultList[j];
               if (rootPath.isEqual(workingPath)) {
                   if ( i < workingPath.size()) { 
                      edgeToBeRemoved = workingPath[i].ID();
                      avoidDijkstra(edgeToBeRemoved, i, workingPath);
                      remove_edge(edgeToBeRemoved);
                   }
               }
            }

            removeVertices(rootPath);

            BasePath spurPath;
            spurPath = DijkstraShortestPathAlg::Dijkstra(spurNode , targetID, true);
            DijkstraShortestPathAlg::clear();

            if (spurPath.size() > 0) {
                // Entire path is made up of the root path and spur path.
                rootPath.append(spurPath);  // this also update the cost of the totalPath

                // Add the potential k-shortest path to the heap.
                if (rootPath.FromTo(sourceID, targetID)) {
                   insertIntoHeap(rootPath);
                }
           }
        }
}

void YenTopKShortestPathsAlg::insertIntoHeap(const BasePath &path) {
   if (m_ResultSet.find(path) != m_ResultSet.end()) return; //already is a solution
   m_Heap.insert(path);
}

void YenTopKShortestPathsAlg::get_shortest_paths(UINT source_id, UINT target_id, int K) {
          _init();  // get the best using Dijkstra
          if (m_ResultList.size() == 0) return; //no path found

          while ( m_ResultList.size() < (unsigned int) K ) {
                next();
                if ( m_Heap.size() == 0 ) break;

                m_ResultList.push_back(*m_Heap.begin());
                m_ResultSet.insert(*m_Heap.begin());
                m_Heap.erase(m_Heap.begin());
          }
}
