///////////////////////////////////////////////////////////////////////////////
///  DijkstraShortestPathAlg.h
///  The implementation of Dijkstra algorithm to get the shortest path of
///  a pair of vertices in a graph.
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 5/30/2010
///
///  $Id: DijkstraShortestPathAlg.h 65 2010-09-08 06:48:36Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////
#ifndef SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
#define SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_

#include <set>
#include "Graph.h"

class DijkstraShortestPathAlg: public Graph {
 private:
       std::set<BaseVertex*> m_CandidateVertices;

 public:
       BasePath Dijkstra(int source, int sink);
       BasePath Dijkstra(UINT source, UINT sink , bool localids);
       explicit DijkstraShortestPathAlg(const Graph &graph)
        :Graph(graph) {}
       void clear();

 protected:
       BasePath get_shortest_path(UINT source_id, UINT sink_id);
       void determine_shortest_paths(UINT source_id, UINT sink_id);
       void improve2vertex(UINT sink_id);
       BaseEdge* bestEdge(UINT sink_id);
       UINT selectBestCandidate();
       void InsertIntoCandidate(UINT node_id);
};

#endif  // SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
