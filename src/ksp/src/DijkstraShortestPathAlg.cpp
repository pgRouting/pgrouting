///////////////////////////////////////////////////////////////////////////////
///  DijkstraShortestPathAlg.cpp
///  The implementation of Dijkstra algorithm to get the shortest path of 
///  a pair of vertices in a graph. 
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 5/30/2010
/// 
/// $Id: DijkstraShortestPathAlg.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////

#include <set>
#include <map>
#include <vector>
#include <cassert>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"

BasePath DijkstraShortestPathAlg::Dijkstra( int source, int sink ) {
        if (  source !=sink  && exist_vertex(source) && exist_vertex(sink) )
             return get_shortest_path( find_vertex(source), find_vertex(sink) );
        return BasePath();
}
 
BasePath DijkstraShortestPathAlg::Dijkstra( POS source, POS sink, bool localids ) {
        if (  source < m_vtVertices.size() and sink < m_vtVertices.size() &&  source != sink)
             return get_shortest_path( source, sink );
        return BasePath();
}

BasePath DijkstraShortestPathAlg::get_shortest_path( POS source_id, POS sink_id ) // original ids
{
#if 0
cout<<"Entering DijkstraShortestPathAlg::get_shortest_path\n";
cout<<"source_id"<<source_id<<"\n";
m_vtVertices[source_id].PrintOut(std::cout, m_vtEdges);
cout<<"\n";
cout<<"sink_id"<<sink_id<<"\n";
m_vtVertices[sink_id].PrintOut(std::cout, m_vtEdges);
cout<<"\n";
#endif
        BasePath path;
	m_CandidateVertices.clear();

	determine_shortest_paths(source_id, sink_id);

	std::deque<BaseEdge*> edges_list;

        double weight = NodeWeight(sink_id);
        

	POS curr_vertex;
	if (weight < Graph::DISCONNECT)
	{
		curr_vertex = sink_id;
		BaseEdge* edge_pt;
		do {
                   edge_pt = bestEdge(curr_vertex);
                   if (!edge_pt) break;
		   path.push_front( edge_pt ) ;
                   curr_vertex =  edge_pt->getStart(); 
		} while (curr_vertex != source_id);
	}
	return path;
}

BaseEdge* DijkstraShortestPathAlg::bestEdge(POS sink_id) {
#if 0
cout<<"***** Entering DijkstraShortestPathAlg::bestEdge\n";
#endif
	std::deque<BaseEdge*> incomming_edges_list;
        double curr_dist = NodeWeight(sink_id);
	get_precedent_edges(sink_id, incomming_edges_list);

        BaseEdge *edge_pt; 
        double prev_dist;
	for(POS i = 0; i < incomming_edges_list.size(); i++) {
                edge_pt = incomming_edges_list[i];
#if 0
cout<<"\n* checking edge\n";
edge_pt->PrintOut(std::cout);
std::cout<<"current distance"<<curr_dist<<"\n";
std::cout<<"NodeWeight at " << edge_pt->getStart()<<" = "<<NodeWeight(edge_pt->getStart())<<"\n";
#endif
                prev_dist = NodeWeight(edge_pt->getStart()) ;
                if (curr_dist == prev_dist + edge_pt->Weight()) 
                    return edge_pt;
        };
        return NULL;
}




void DijkstraShortestPathAlg::determine_shortest_paths( POS source_id, POS sink_id )
{
#if 0
cout<<"Entering DijkstraShortestPathAlg::determine_shortest_paths\n";
cout<<"source_id"<<source_id<<"\n";
m_vtVertices[source_id].PrintOut(std::cout, m_vtEdges);
cout<<"\n";
cout<<"sink_id"<<sink_id<<"\n";
m_vtVertices[sink_id].PrintOut(std::cout, m_vtEdges);
cout<<"\n";
#endif
        //the source starts with 0 as cost
	m_vtVertices[source_id].Weight(0);
        
        // m_pDirectGraph
	m_CandidateVertices.push_back(source_id);

	//3. start searching for the shortest path
        improve2vertex( sink_id );
#if 0
cout<<"Final State\n";
PrintOut(std::cout);
#endif
}

void DijkstraShortestPathAlg::improve2vertex(POS sink_id )
{
#if 0
cout<<"Entering DijkstraShortestPathAlg::improve2vertex\n";
#endif
     while (!m_CandidateVertices.empty()) {
        int current_id = selectBestCandidate();
#if 0
cout<<"BestCandidate: current id"<<current_id<<"\n";
m_vtVertices[current_id].PrintOut(std::cout, m_vtEdges);
cout<<"\n";
#endif
        //m_CandidateVertices.erase( m_CandidateVertices.begin() );

	// 1. get the neighboring edges (vertices are stiored on the vertices) 
	std::deque<POS> neighbor_edges_list;

	get_adjacent_edges(current_id, neighbor_edges_list);
#if 0 
std::cout<<"!!!!!!!!!  adjacent edges size"<< neighbor_edges_list.size()<<"\n";
#endif

	// 2. update the distance passing on the current vertex
	BaseEdge *edge_pt;
        BaseVertex *curr_vertex_pt;
        BaseVertex *next_vertex_pt;
        double distance;
	for(POS i = 0; i < neighbor_edges_list.size(); i++)
	{
                edge_pt = &m_vtEdges[ neighbor_edges_list[i] ];
#if 0
cout<<"current_edge\n";
edge_pt->PrintOut(std::cout);
cout<<"\n";
#endif

                curr_vertex_pt =  get_vertex(current_id);
                next_vertex_pt =  get_vertex(edge_pt->getEnd());

		//2.1 skip if it has been visited before
		if( next_vertex_pt->visited()) continue;

	        InsertIntoCandidate(next_vertex_pt->getNID());
                
		//2.2 calculate the distance
		distance = curr_vertex_pt->Weight() + edge_pt->Weight();

		//2.3 update the distance if necessary (comparison and assignment in BaseVertex)
                next_vertex_pt->Weight(distance);

                curr_vertex_pt->setAsVisited();  //mark as Visited

                if (curr_vertex_pt->getNID()==sink_id) return;
	}
        neighbor_edges_list.clear();
      } 
}

POS DijkstraShortestPathAlg::selectBestCandidate() {
    assert( m_CandidateVertices.size() );
    POS best = 0;
    POS best_id;
    double bestDistance = Graph::DISCONNECT;
    POS current_id;
    for(POS i = 0; i < m_CandidateVertices.size(); i++)
    {
       current_id = m_CandidateVertices[i];
       if ( bestDistance > NodeWeight(current_id)) {
	  best = i;
	  best_id = current_id;
          bestDistance = NodeWeight(current_id);
      }
    }
    m_CandidateVertices.erase(m_CandidateVertices.begin()+best);
    return best_id;
}

void DijkstraShortestPathAlg::InsertIntoCandidate( POS node_id ) {
    for(POS i = 0; i < m_CandidateVertices.size(); i++)
      if (m_CandidateVertices[i]== node_id) return;
    m_CandidateVertices.push_back( node_id );
}

void DijkstraShortestPathAlg::clear()
{
     // this restores all visits to unvisited and all node weights to DISCONNECTED
     for (POS i = 0; i < m_vtVertices.size() ;i++) 
          m_vtVertices[i].restore();
     for (POS i = 0; i < m_vtEdges.size() ;i++) 
          m_vtEdges[i].reInsert();
}

