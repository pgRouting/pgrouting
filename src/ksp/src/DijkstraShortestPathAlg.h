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
#ifndef _DIJKSTRASHORTESTPATHALG_H
#define  _DIJKSTRASHORTESTPATHALG_H


using namespace std;
#include "GraphElements.h"

class DijkstraShortestPathAlg: public Graph
{
private: 
        //int from_id;
        //int to_id;
	std::deque<POS> m_CandidateVertices;
	
public:
        BasePath Dijkstra( int source, int sink ) ;
        BasePath Dijkstra( POS source, POS sink , bool localids ) ;
	DijkstraShortestPathAlg(const Graph &graph): Graph(graph){}
	void clear();
protected:

	BasePath get_shortest_path(POS source_id, POS sink_id );
	void determine_shortest_paths(POS source_id, POS sink_id) ; 
        void improve2vertex( POS sink_id );
        BaseEdge* bestEdge(POS sink_id);
        POS selectBestCandidate( ) ;
        void InsertIntoCandidate(POS node_id);
};

#endif
