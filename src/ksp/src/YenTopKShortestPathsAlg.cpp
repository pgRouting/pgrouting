#include <deque>
#include <algorithm>
#include "./signalhandler.h"
#include "./vrp_assert.h"
#include "./GraphElements.h"
#include "./DijkstraShortestPathAlg.h"
#include "./YenTopKShortestPathsAlg.h"


void YenTopKShortestPathsAlg::clear() {
        DijkstraShortestPathAlg::clear();
        m_Heap.clear();
}

void YenTopKShortestPathsAlg::getFirstSolution() {
        BasePath shortestPath = Dijkstra(sourceID, targetID);
        if (!shortestPath.isEmpty()) {
            getParallelShort(shortestPath);
            curr_result_path = shortestPath;
            m_ResultSet.insert(shortestPath);
        }
}


std::deque<BasePath> YenTopKShortestPathsAlg::Yen(int  source, int  sink, int K) {
        std::deque<BasePath> m_ResultList;
        if ((source !=sink) && (K > 0)) {
             BaseVertex* sourcePt = find_vertex(source);
             if (sourcePt == NULL) return m_ResultList;
             BaseVertex* sinkPt = find_vertex(sink);
             if (sinkPt == NULL) return m_ResultList;
             sourceID = sourcePt->ID();
             targetID = sinkPt->ID();
             executeYen(sourceID, targetID, K);
        }
#if 1  // 1  K paths + heap paths are to be returned
        while (m_Heap.size()){
             curr_result_path = *m_Heap.begin();
             m_ResultSet.insert(curr_result_path);
             m_Heap.erase(m_Heap.begin());
        }
#else  // set to 0 if NEVER the heap paths are to be returned
        if (K >= m_ResultList.size()) m_ResultList.resize(K);
#endif
        m_ResultList.assign(m_ResultSet.begin(), m_ResultSet.end());
        return m_ResultList;
}


void YenTopKShortestPathsAlg::getParallelShort(const BasePath &solutionPath) {
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
            if ((*fIt)->getStart() != startPt->ID()) assert(true == false);
            if ((*fIt)->getEnd() != endId) continue;
            if ((*fIt)->ID() == edgePt->ID()) continue;

            // copy the path up the the ith position
            // insert the parallel found
            // append the rest of the solution path
            newPath.clear();
            solutionPath.subPath(newPath, i);
            newPath.push_back(*fIt);
            for (UINT j = i + 1; j < solutionPath.size(); j++) {
                 newPath.push_back(solutionPath[j]);
            }
            // if has same (or less)  weight then insert as solution, otherwise insert into heap
            if (newPath.Weight() <= solutionPath.Weight()) {
                m_ResultSet.insert(newPath);
            } else {
                insertIntoHeap(newPath);
            }
        }
     }
}





void YenTopKShortestPathsAlg::doNextCycle() {
        REG_SIGINT

        //BasePath curr_result_path = m_ResultList.back();

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
            std::set<BasePath, BasePath::compBasePath>::iterator pIt;
            for (pIt = m_ResultSet.begin(); pIt != m_ResultSet.end(); ++pIt) {
               if ( m_DoIt && ((*pIt).path() != curr_result_path.path()) 
                    && curr_result_path.isEqual(*pIt)) {
                         continue;
               }
               if (rootPath.isEqual(*pIt)) {
                   if (i < (*pIt).size()) {
                      BaseEdge* edgeToBeRemoved = (*pIt)[i];
                      if (!m_DoIt) assert(edgeToBeRemoved->isMain());
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

#if 0
// correctness only edges in set are Main (minimal) others are parallel
// consumes hugh amount of time and its a little complicated
// so I am not removing it, only commenting it out
    std::set <BaseEdge*, BaseEdge::compBestEdge>::iterator edgeInSet;
    for (eListIt eIt = m_Edges.begin(); eIt != m_Edges.end(); ++eIt) {
        edgeInSet = m_BestEdgesPt.find(&(*eIt));
        if (edgeInSet == m_BestEdgesPt.end()) {
          assert (true == false);
        } else {
           if ((*edgeInSet)->ID() == eIt->ID()) {
                assert(eIt->isMain());
                assert(eIt->isActive());
                assert(isActive(&(*eIt)));
           } else {
                assert(eIt->isParallel());
                assert(eIt->isActive());
                assert(isActive(&(*eIt)));
           }
        }
    }
#endif



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
//    assert(m_ResultSet.find(path) == m_ResultSet.end()); // O(log n) assertion
    m_Heap.insert(path);
}

void YenTopKShortestPathsAlg::executeYen(UINT source_id, UINT target_id, int K) {
          assert(m_DoIt == false); // make sure we are not generating based on parallel edges
          clear();
          getFirstSolution();

          if (curr_result_path.size() == 0) return;  // no path found

          while ( m_ResultSet.size() < (unsigned int) K ) {
                doNextCycle();
                if ( m_Heap.size() == 0 ) break;
                curr_result_path = *m_Heap.begin();
                getParallelShort(*m_Heap.begin());  // in case for parallels
                m_ResultSet.insert(curr_result_path);
                m_Heap.erase(m_Heap.begin());
          }
}
