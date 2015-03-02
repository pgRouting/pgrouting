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
       BasePath Dijkstra(POS source, POS sink , bool localids);
       explicit DijkstraShortestPathAlg(const Graph &graph)
        :Graph(graph) {}
       void clear();

 protected:
       BasePath get_shortest_path(POS source_id, POS sink_id);
       void determine_shortest_paths(POS source_id, POS sink_id);
       void improve2vertex(POS sink_id);
       BaseEdge* bestEdge(POS sink_id);
       POS selectBestCandidate();
       void InsertIntoCandidate(POS node_id);
};

#endif  // SRC_KSP_SRC_DIJKSTRASHORTESTPATHALG_H_
