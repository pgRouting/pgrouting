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
        BasePath shortestPath = Dijkstra(sourceID, targetID, true);
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

#if 0
void YenTopKShortestPathsAlg::avoidDijkstra(UINT edgeToBeRemoved, UINT atPosOfPath, BasePath &workingPath) {
         //generate and store on the heap all paths that have parallel edges to edgeToBeRemoved
         UINT startId = m_Edges[edgeToBeRemoved].getStart();
         UINT endId = m_Edges[edgeToBeRemoved].getEnd();

         //std::deque<BaseEdge*> fanOut = m_Vertices[startId].getFanOut();
         BaseVertex::eSetPt fanOut = m_Vertices[startId].getFanOut();
         BaseVertex::eSetPtIt fIt;
         //getFanOutActiveEdges(startId,fanOut);
         BasePath newPath; 
         for (fIt = fanOut.begin(); fIt !=fanOut.end(); ++fIt) {
         //for (UINT i = 0; i < fanOut.size(); i++) {
             //if (edgeToBeRemoved == fanOut[i].ID()){
             if (edgeToBeRemoved == (*fIt)->ID()){
                remove_edge(edgeToBeRemoved);
                continue;
             }
             //if ((fanOut[i].getStart() == startId) && (fanOut[i].getEnd() == endId)) {
             if (((*fIt)->getStart() == startId) && ((*fIt)->getEnd() == endId)) {
                 // a new path can be formed with the parallel edge:
                 //  NewPath = rootpath + Paralleledgeof(edgeToBeRemoved) + rest of the curr_result path
                 workingPath.subPath(newPath, atPosOfPath); //copy the path up to the position
                 newPath.push_back(*(*fIt));  //insert the paralel edge
                 for (UINT j = atPosOfPath+1; j < workingPath.size(); j++) {
		     newPath.push_back( workingPath[j] );
                 }
                 insertIntoHeap(newPath);
                 remove_edge((*fIt)->ID());
             }
         }
};                 
#endif

void YenTopKShortestPathsAlg::removeEdgeAndParallels(UINT edgeId) {
    BaseEdge* edgePt = &m_Edges[ edgeId ];
    BaseVertex* startPt = &m_Vertices[ edgePt->getStart() ];
    BaseVertex::eSetPt fanOutSet = startPt->getFanOut();
    BaseVertex::eSetPtIt fIt;
    for (fIt = fanOutSet.begin(); fIt !=fanOutSet.end(); ++fIt) {
        if ((*fIt)->getStart() != startPt->ID()) assert(true==false);  //somethingwent really wrong
        if ((*fIt)->getEnd() != edgePt->getEnd()) continue;  // not a parallel edge
        (*fIt)->remove();
        // maybe store so reconstructing the graph is faster???
    }
};



 
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
        edgePt = &m_Edges[ solutionPath[i].ID() ];
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
            newPath.push_back(*(*fIt));
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
               // This next condition to skip parallel paths already as answer
               if ( m_DoIt && j < m_ResultList.size() - 1  
                    && curr_result_path.isEqual(m_ResultList[j])) {
                         continue;
               }
               BasePath workingPath = m_ResultList[j];
               if (rootPath.isEqual(workingPath)) {
                   if ( i < workingPath.size()) { 
                      edgeToBeRemoved = workingPath[i].ID();
                      removeEdgeAndParallels(edgeToBeRemoved);
                   }
               }
            }

            removeVertices(rootPath);

            BasePath spurPath;
            THROW_ON_SIGINT
            spurPath = DijkstraShortestPathAlg::Dijkstra(spurNode , targetID, true);
            THROW_ON_SIGINT
            DijkstraShortestPathAlg::clear();
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

                m_ResultList.push_back(*m_Heap.begin());
	        getParallelShort(*m_Heap.begin()); // in case for parallels
                m_ResultSet.insert(*m_Heap.begin());
                m_Heap.erase(m_Heap.begin());
          }
}
