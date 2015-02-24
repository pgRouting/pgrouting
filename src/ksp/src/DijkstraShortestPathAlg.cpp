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
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"

BasePath* DijkstraShortestPathAlg::get_shortest_path( BaseVertex* source, BaseVertex* sink )
{
        bool is_source2sink=true;
	BaseVertex* start_vertex = is_source2sink ? source : sink;
	BaseVertex* end_vertex = is_source2sink ? sink : source;

	determine_shortest_paths(source, sink, is_source2sink);
        POS start_id = start_vertex->getNID();
        POS sink_id = end_vertex->getNID();

	std::deque<BaseEdge*> edges_list;

        double weight = m_pDirectGraph.Weight(sink_id);
        

	POS curr_vertex;
	BaseVertex *prev_vertex;
	if (weight < Graph::DISCONNECT)
	{
		curr_vertex = sink_id;
		BaseEdge* edge;
		do {
                   edge = bestEdge(curr_vertex, is_source2sink);
		   edges_list.push_front( edge ) ;
                   prev_vertex =  is_source2sink ?  m_pDirectGraph.get_vertex(edge->getStart()): 
                                                    m_pDirectGraph.get_vertex(edge->getEnd());
                   curr_vertex = prev_vertex->getNID(); 
		} while (curr_vertex != start_id);
	}
	return new BasePath(edges_list, weight);
}

BaseEdge* DijkstraShortestPathAlg::bestEdge(POS sink_id, bool is_source2sink) const {
	std::deque<BaseEdge*> incomming_edges_list;
        double curr_dist = m_pDirectGraph.Weight(sink_id);

	if(is_source2sink)
		m_pDirectGraph.get_precedent_edges(sink_id, incomming_edges_list);
	else
		m_pDirectGraph.get_adjacent_edges(sink_id, incomming_edges_list);

        BaseEdge *edge; 
        double prev_dist;
	for(POS pos = 0; pos < incomming_edges_list.size(); pos++) {
                edge = incomming_edges_list[pos];
                prev_dist =  is_source2sink ? m_pDirectGraph.Weight(edge->getStart()) 
                                            : m_pDirectGraph.Weight(edge->getEnd());
                if (curr_dist == prev_dist + edge->Weight()) 
                    return edge;
        };
        return NULL;
}




void DijkstraShortestPathAlg::determine_shortest_paths( BaseVertex* source, BaseVertex* sink, bool is_source2sink )
{
	//1. clear the intermediate variables
	clear();

	//2. initiate the local variables
	BaseVertex* start_vertex = is_source2sink ? source : sink;
	BaseVertex* sink_vertex = is_source2sink ? sink : source;
	//m_mpStartDistanceIndex[start_vertex] = 0;
	start_vertex->Weight(0);
        POS start_id = start_vertex->getNID();
        POS sink_id = sink_vertex->getNID();
        
        // m_pDirectGraph
	m_CandidateVertices.push_back(start_id);

	//3. start searching for the shortest path
	improve2vertex( m_CandidateVertices, sink_id, is_source2sink);
}

void DijkstraShortestPathAlg::improve2vertex(std::deque<POS> &m_CandidateVertices, POS sink_id, bool is_source2sink )
{
    while (!m_CandidateVertices.empty()) {
        int current_id = selectBest(m_CandidateVertices);
        m_CandidateVertices.erase( m_CandidateVertices.begin() );

	// 1. get the neighboring edges (vertices are sotred on the edges) 
	std::deque<BaseEdge*> neighbor_edges_list;

	if(is_source2sink)
		m_pDirectGraph.get_adjacent_edges(current_id, neighbor_edges_list);
	else
		m_pDirectGraph.get_precedent_edges(current_id, neighbor_edges_list);

	// 2. update the distance passing on the current vertex
	BaseEdge *edge;
        BaseVertex *curr_vertex;
        BaseVertex *next_vertex;
        double distance;
	for(POS pos = 0; pos < neighbor_edges_list.size(); pos++)
	{
                edge = neighbor_edges_list[pos];
                curr_vertex =  m_pDirectGraph.get_vertex(current_id);
                next_vertex =  is_source2sink ? m_pDirectGraph.get_vertex(edge->getEnd())
                                              : m_pDirectGraph.get_vertex(edge->getStart());

		//2.1 skip if it has been visited before
		if( next_vertex->visited()) continue;
	        m_CandidateVertices.push_back(next_vertex->getNID());
                
		//2.2 calculate the distance
		distance = curr_vertex->Weight() + edge->Weight();

		//2.3 update the distance if necessary (comparison and assignment in BaseVertex)
                next_vertex->Weight(distance);
	}
        neighbor_edges_list.clear();
        curr_vertex->visit();  //mark as Visited
        if (curr_vertex->getNID()==sink_id) return;
    }
    return;
}

POS DijkstraShortestPathAlg::selectBest( std::deque<POS> &m_CandidateVertices ) {
    POS best = 0;
    double bestDistance = Graph::DISCONNECT;
    POS current_id;
    for(POS pos = 0; pos < m_CandidateVertices.size(); pos++)
    {
       current_id = m_CandidateVertices[pos];
       if ( bestDistance > m_pDirectGraph.Weight(current_id)) {
	  best= pos;
          bestDistance = m_pDirectGraph.Weight(current_id);
      }
    }
    return best;
}


void DijkstraShortestPathAlg::clear()
{
	//m_stDeterminedVertices.clear();
	//m_mpPredecessorVertex.clear();
	//m_mpStartDistanceIndex.clear();
	//m_quCandidateVertices.clear();
}
#if 0
BasePath* DijkstraShortestPathAlg::update_cost_forward( BaseVertex* vertex )
{
	double cost = Graph::DISCONNECT;

 	// 1. get the set of successors of the input vertex
	set<BaseVertex*>* adj_vertex_set = new set<BaseVertex*>();
	m_pDirectGraph.get_adjacent_vertices(vertex, *adj_vertex_set);
 
 	// 2. make sure the input vertex exists in the index
	map<BaseVertex*, double>::iterator pos4vertexInStartDistIndex = m_mpStartDistanceIndex.find(vertex);
	if(pos4vertexInStartDistIndex == m_mpStartDistanceIndex.end())
 	{
		pos4vertexInStartDistIndex = 
			(m_mpStartDistanceIndex.insert(make_pair(vertex, Graph::DISCONNECT))).first;
 	}

 	// 3. update the distance from the root to the input vertex if necessary
 	for(set<BaseVertex*>::const_iterator pos=adj_vertex_set->begin(); pos!=adj_vertex_set->end();++pos)
 	{
 		// 3.1 get the distance from the root to one successor of the input vertex
		map<BaseVertex*, double>::const_iterator cur_vertex_pos = m_mpStartDistanceIndex.find(*pos);
		double distance = cur_vertex_pos == m_mpStartDistanceIndex.end() ?
			Graph::DISCONNECT : cur_vertex_pos->second;
 
 		// 3.2 calculate the distance from the root to the input vertex
		distance += m_pDirectGraph->get_edge_weight(vertex, *pos);
	
 		// 3.3 update the distance if necessary 
		double cost_of_vertex = pos4vertexInStartDistIndex->second;
 		if(cost_of_vertex > distance)
 		{
			m_mpStartDistanceIndex[vertex] = distance;
			m_mpPredecessorVertex[vertex] = cur_vertex_pos->first;
 			cost = distance;
 		}
 	}

 	// 4. create the sub_path if exists
	BasePath* sub_path = NULL;
	if(cost < Graph::DISCONNECT) 
 	{
		deque<BaseEdge*> edges_list;
		edges_list.push_back(vertex);

		map<BaseVertex*, BaseVertex*>::const_iterator pos4PredVertexMap =
			m_mpPredecessorVertex.find(vertex);
		
		while(pos4PredVertexMap != m_mpPredecessorVertex.end())
		{
			BaseVertex* pred_vertex_pt = pos4PredVertexMap->second;
			vertex_list.push_back(pred_vertex_pt);
			pos4PredVertexMap = m_mpPredecessorVertex.find(pred_vertex_pt);
		}

		sub_path = new BasePath(vertex_list, cost);
 	}
 	return sub_path;
}
#endif

#if 0
void DijkstraShortestPathAlg::correct_cost_backward( BaseVertex* vertex )
{
 	// 1. initialize the list of vertex to be updated
	vector<BaseVertex*> vertex_pt_list;
	vertex_pt_list.push_back(vertex);

	// 2. update the cost of relevant precedents of the input vertex
	while(!vertex_pt_list.empty())
 	{
		BaseVertex* cur_vertex_pt = *(vertex_pt_list.begin());
		vertex_pt_list.erase(vertex_pt_list.begin());

 		double cost_of_cur_vertex = m_mpStartDistanceIndex[cur_vertex_pt];

		set<BaseVertex*> pre_vertex_set;
		m_pDirectGraph.get_precedent_vertices(cur_vertex_pt, pre_vertex_set);
		for(set<BaseVertex*>::const_iterator pos=pre_vertex_set.begin(); pos!=pre_vertex_set.end();++pos)
		{
			map<BaseVertex*,double>::const_iterator pos4StartDistIndexMap = 
				m_mpStartDistanceIndex.find(*pos);
			double cost_of_pre_vertex = m_mpStartDistanceIndex.end() == pos4StartDistIndexMap ?
				Graph::DISCONNECT : pos4StartDistIndexMap->second;

			double fresh_cost = cost_of_cur_vertex + m_pDirectGraph->get_edge_weight(*pos, cur_vertex_pt);
			if(cost_of_pre_vertex > fresh_cost)
			{
				m_mpStartDistanceIndex[*pos] = fresh_cost;
				m_mpPredecessorVertex[*pos] = cur_vertex_pt;
				vertex_pt_list.push_back(*pos);
			}
		}
	}
}

#endif
