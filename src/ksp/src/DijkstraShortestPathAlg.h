#ifndef SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
#define SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_

#include <set>
#include <deque>
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
       std::deque<BaseEdge> Dijkstra(long source, long sink);
       //! Dijkstra using the graph's IDs
       BasePath Dijkstra(UINT source, UINT sink);
       ///@}
       explicit DijkstraShortestPathAlg(const Graph &graph)
        :Graph(graph) {}
       

 protected:
    /** @name Routines used in the algorithm*/
    ///@{

    BasePath boostDijkstra(UINT source_id, UINT sink_id);

    ///@}
    /** @name Routines to modify the graph*/
    ///@{

    //! logically removes the vertices from the graph that are starting vertices of the edges in path
    void removeVertices(const BasePath &path);
    //! logically restores the vertices from the graph that are starting vertices of the edges in path
    void restoreVertices(const BasePath &path);
    //! ture when edge is active and both of its vertices are active
    bool isActive(const BaseEdge *edgePt) const;
    //! logically removes the edge and all its parallels
    void removeEdgeAndParallels(BaseEdge *edgePt);
    //! logically restores the edge kept in the book-keeping
    void restoreEdges();
    //! logically restores all edges & vertices
    void clear();
    ///@}

 protected:
    std::deque<BaseEdge*> removedEdges;
 private:
    /* TODO(vicky):
       put as member the boost graph
       once its a member, the routines that modify the graph should modify this graph also
    */
};

#endif  // SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
