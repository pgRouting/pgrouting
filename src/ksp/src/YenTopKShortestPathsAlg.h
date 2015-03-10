#ifndef SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_
#define SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_

#include <deque>
#include "DijkstraShortestPathAlg.h"

class YenTopKShortestPathsAlg: public DijkstraShortestPathAlg {
        std::set<BasePath, BasePath::compBasePath> m_ResultSet;
        std::deque<BasePath> m_ResultList;
        std::set<BasePath, BasePath::compBasePath> m_Heap;
        std::deque<BaseEdge*> removedEdges;

        bool m_DoIt;
        int m_Source_id;
        int m_Target_id;
        UINT sourceID;
        UINT targetID;


 public:
        explicit YenTopKShortestPathsAlg(const Graph& graph)
                : DijkstraShortestPathAlg(graph), m_DoIt(false),
                 m_Source_id(0), m_Target_id(0) {}

        ~YenTopKShortestPathsAlg(void) {clear();}
        std::deque<BasePath> Yen(int source, int target, int numb_paths);
        void clear();
 private:
        void insertIntoHeap(const BasePath &path);
        void executeYen(UINT pSource, UINT pTarget, int top_k);
        void avoidDijkstra(UINT edgeToBeRemoved, UINT atPosOfPath, BasePath &workingPath);
        void getParallelShort(const BasePath &solutionPath);
        void removeEdgeAndParallels(BaseEdge *edgePt);
        void restoreEdges();

        BasePath get_shortest_path(UINT pSource, UINT pTarget);
        void getFirstSolution();
        void next();
};
#endif  // SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_
