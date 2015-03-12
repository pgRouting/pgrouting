#ifndef SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_
#define SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_

#include <deque>
#include <set>
#include "DijkstraShortestPathAlg.h"

class YenTopKShortestPathsAlg: public DijkstraShortestPathAlg {
 public:
        explicit YenTopKShortestPathsAlg(const Graph& graph)
                : DijkstraShortestPathAlg(graph),
                 m_DoIt(false) {}  // in case of wanted parallels set to true
//                 m_Source_id(0), m_Target_id(0) {}

        ~YenTopKShortestPathsAlg(void) {clear();}
        std::deque<BasePath> Yen(int source, int target, int K);
        void clear();

 private:

        //! the actual algorithm 
        void executeYen(UINT pSource, UINT pTarget, int top_k);
        /** @name Auxiliary function for yen's algorithm */
        ///@{

        //! Inserts a path found in the heap
        void insertIntoHeap(const BasePath &path);
        //! Performs the first Dijkstra of the algorithm
        void getFirstSolution();
        //! Performs the next cycle of the algorithm
        void doNextCycle();
        ///@}

 private:
        UINT sourceID;  //!< local ID of the source
        UINT targetID;  //!< local ID of the target
        //! ordered set of paths found
        std::set<BasePath, BasePath::compBasePath> m_ResultSet;
        BasePath curr_result_path; //!< latest path found
        //std::deque<BasePath> m_ResultList;
        std::set<BasePath, BasePath::compBasePath> m_Heap;

 private:
        /** @name For parallel paths */
        ///@{

        bool m_DoIt;  //!< manually set to true in the code if (some) parallels are wanted
        //!  when _DoIt == ture, creates a "small" set of paths with parallel edges
        void getParallelShort(const BasePath &solutionPath);
        ///@}
};
#endif  // SRC_KSP_SRC_YENTOPKSHORTESTPATHSALG_H_
