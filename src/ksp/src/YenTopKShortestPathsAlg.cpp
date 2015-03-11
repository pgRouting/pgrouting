#include <deque>
#include <algorithm>
#include "./signalhandler.h"
#include "./vrp_assert.h"
#include "./GraphElements.h"
#include "./DijkstraShortestPathAlg.h"
#include "./YenTopKShortestPathsAlg.h"


void YenTopKShortestPathsAlg::clear() {
        DijkstraShortestPathAlg::clear();
        m_ResultList.clear();
        m_Heap.clear();
}

void YenTopKShortestPathsAlg::getFirstSolution() {
        BasePath shortestPath = Dijkstra(sourceID, targetID);
        if (!shortestPath.isEmpty()) { 
            // m_DoIt is set to false for the PgRouting Project
	    getParallelShort(shortestPath);
            m_ResultList.push_back(shortestPath);
            m_ResultSet.insert(shortestPath);
        }
}


std::deque<BasePath> YenTopKShortestPathsAlg::Yen(int  source, int  sink, int K) {
        clear();
        if ((source !=sink) && (K > 0)) {
             m_Source_id = source;
             m_Target_id = sink;

             BaseVertex* sourcePt = find_vertex(source);
             if (sourcePt == NULL) return m_ResultList;
             BaseVertex* sinkPt = find_vertex(sink);
             if (sinkPt == NULL) return m_ResultList;
             sourceID = sourcePt->ID();
             targetID = sinkPt->ID();
             executeYen(sourceID, targetID, K);
        }
        m_ResultList.assign(m_ResultSet.begin(),m_ResultSet.end());
        return m_ResultList;
}


void YenTopKShortestPathsAlg::removeEdgeAndParallels(BaseEdge *edgePt) {
    //BaseEdge* edgePt = &m_Edges[ edgeId ];
    BaseVertex *startPt = &m_Vertices[ edgePt->getStart() ];
    BaseVertex::eSetPt fanOutSet = startPt->getFanOut();
    BaseVertex::eSetPtIt fIt;
    for (fIt = fanOutSet.begin(); fIt !=fanOutSet.end(); ++fIt) {
        if ((*fIt)->getStart() != startPt->ID()) assert(true==false);  //somethingwent really wrong
        if ((*fIt)->getEnd() != edgePt->getEnd()) continue;  // not a parallel edge
        (*fIt)->remove();
        removedEdges.push_back((*fIt));
    }
};


void YenTopKShortestPathsAlg::restoreEdges() {
    while (removedEdges.size()){
        removedEdges.front()->reInsert();
        removedEdges.pop_front();
    }
}

 
void YenTopKShortestPathsAlg::getParallelShort(const BasePath &solutionPath){
    REG_SIGINT
    if (!m_DoIt) return;
    BaseEdge *edgePt;
    BaseVertex *startPt = NULL;
    BaseVertex::eSetPt fanOutSet;
    BaseVertex::eSetPtIt fIt;
    BasePath newPath;
    std::deque<BaseEdge>::iterator eIt;
    for (UINT i = 0; i < solutionPath.size(); ++i) {
        THROW_ON_SIGINT
        edgePt = solutionPath[i];
        startPt = &m_Vertices[ edgePt->getStart() ];
        fanOutSet = startPt->getFanOut();
        UINT endId = edgePt->getEnd();

        for (fIt = fanOutSet.begin(); fIt !=fanOutSet.end(); ++fIt) {
            THROW_ON_SIGINT
            // skipping:
            //  somethingwent really wrong
            //  not a parallel edge
            //  its the edge that is part of the solution
            if ((*fIt)->getStart() != startPt->ID()) assert(true==false);  
            if ((*fIt)->getEnd() != endId) continue;
            if ((*fIt)->ID() == edgePt->ID()) continue;

            // copy the path up the the ith position
            // insert the parallel found
            // append the rest of the solution path
            newPath.clear();
            solutionPath.subPath(newPath, i);
            newPath.push_back(*fIt);
            for (UINT j = i + 1; j < solutionPath.size(); j++) {
                 newPath.push_back( solutionPath[j] );
            }
            // if has same (or less)  weight then insert as solution, otherwise insert into heap 
            if (newPath.Weight() <= solutionPath.Weight()) {
                m_ResultList.push_back(newPath);
                m_ResultSet.insert(newPath);
            } else {
                insertIntoHeap(newPath);
            }
        }
     }
}
            




void YenTopKShortestPathsAlg::next() {
        REG_SIGINT
#if 1
        BasePath curr_result_path = m_ResultList.back();
#else
        UINT currPathId =  m_ResultList.size()-1;
        BasePath curr_result_path = m_ResultList[ currPathId ];
#endif

        UINT spurNodeId;
        BasePath rootPath;
        
        for (UINT i = 0; i < curr_result_path.size(); ++i) {
            BaseEdge* spurEdge = curr_result_path[i];
            spurNodeId = spurEdge->getStart();

            curr_result_path.subPath(rootPath, i);

	    // when spur node has parallels to the end of the spur edge then 
            // no dijkstra has to be done because:
            //  NewPath = rootpath + Paralleledgeof(supredge) rest of the curr_result path
            // is the shortest path using the paralel edge
            
#if 1
#else
            UINT edgeToBeRemoved;
#endif
            for (UINT j=0; j < m_ResultList.size(); j++) {
               // This next condition to skip parallel paths already as answer
               if ( m_DoIt && j < m_ResultList.size() - 1  
                    && curr_result_path.isEqual(m_ResultList[j])) {
                         continue;
               }
               //BasePath workingPath = m_ResultList[j];
               if (rootPath.isEqual(m_ResultList[j])) {
                   if ( i < m_ResultList[j].size()) { 
                      BaseEdge* edgeToBeRemoved = m_ResultList[j][i];
                      removeEdgeAndParallels(edgeToBeRemoved);
                   }
               }
            }

            removeVertices(rootPath);

            BasePath spurPath;
            THROW_ON_SIGINT
            spurPath = DijkstraShortestPathAlg::Dijkstra(spurNodeId , targetID);
            THROW_ON_SIGINT
            restoreEdges();
            restoreVertices(rootPath);
            //DijkstraShortestPathAlg::clear();
            THROW_ON_SIGINT

            if (spurPath.size() > 0) {
                // Entire path is made up of the root path and spur path.
                rootPath.append(spurPath);  

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

void YenTopKShortestPathsAlg::executeYen(UINT source_id, UINT target_id, int K) {
          clear();
          getFirstSolution();

          if (m_ResultList.size() == 0) return; //no path found

          while ( m_ResultList.size() < (unsigned int) K ) {
                next();
                if ( m_Heap.size() == 0 ) break;

	        getParallelShort(*m_Heap.begin()); // in case for parallels
                m_ResultList.push_back(*m_Heap.begin());
                m_ResultSet.insert(*m_Heap.begin());
                m_Heap.erase(m_Heap.begin());
          }
}
