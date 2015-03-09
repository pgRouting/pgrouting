#ifndef SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
#define SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_

#include <set>
#include "Graph.h"

/**************************************************************************/
//! DijkstraShortestPathAlg: public Graph edge oriented implementation
/*!
\author Vicky Vergara
\date Feb/2015
\copyright GNU General Public License, version 2
\details  Class to store an edge of a graph.
\sa Graph 
********************************************************************** */
class DijkstraShortestPathAlg: public Graph {
 private:
       //! Ordered by weight set of pointers to vertices used in the algorithm
       //std::set<BaseVertex*, BaseVertex::compBaseVertex> m_CandidateVertices;
       std::deque<BaseVertex*> m_CandidateVertices;
       std::set<UINT> m_CandidateVerticesId;

 public:
       /** @name Dijkstra execution
       \Returns empty path: when:
         - source is the same as sink
         - source is is not found in any edge
         - sink is is not found in any edge
         - the edges structure don't allow to go from source to sink
       \Returns the shortest path from source to sink

       Path A is shorter than path B when:
          - A's weight < B's weight
          - when the weights are the same then
            - A's length < B's length
          - when the weights and lenth are the same then on the first edge that is different
            - A's edge(i)'s Id < B's edge(i)'s Id
       */
       ///@{

       //! Dijkstra using the original ids
       BasePath Dijkstra(int source, int sink);
       //! Dijkstra using the graph's IDs 
       BasePath Dijkstra(UINT source, UINT sink , bool localids);
       ///@}
       explicit DijkstraShortestPathAlg(const Graph &graph)
        :Graph(graph) {}
       void clear();

 protected:
       /** @name Routines used in the algorithm*/
       ///@{
       BasePath boostDijkstra(UINT source_id, UINT sink_id);
       ///@}
};

#endif  // SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
