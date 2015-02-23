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
	determine_shortest_paths(source, sink, true);

	std::vector<BaseVertex*> vertex_list;
	std::map<BaseVertex*, double>::const_iterator pos = 
		m_mpStartDistanceIndex.find(sink);
	double weight = pos != m_mpStartDistanceIndex.end() ? pos->second : Graph::DISCONNECT;

	if (weight < Graph::DISCONNECT)
	{
		BaseVertex* cur_vertex_pt = sink;
		do 
		{
			vertex_list.insert(vertex_list.begin(), cur_vertex_pt);

			std::map<BaseVertex*, BaseVertex*>::const_iterator pre_pos = 
				m_mpPredecessorVertex.find(cur_vertex_pt);

			if (pre_pos == m_mpPredecessorVertex.end()) break;

			cur_vertex_pt = pre_pos->second;

		} while (cur_vertex_pt != source);

		vertex_list.insert(vertex_list.begin(), source);
	}
	return new BasePath(vertex_list, weight);
}

void DijkstraShortestPathAlg::determine_shortest_paths( BaseVertex* source, BaseVertex* sink, bool is_source2sink )
{
	//1. clear the intermediate variables
	clear();

	//2. initiate the local variables
	BaseVertex* end_vertex = is_source2sink ? sink : source;
	BaseVertex* start_vertex = is_source2sink ? source : sink;
	m_mpStartDistanceIndex[start_vertex] = 0;
	start_vertex->Weight(0);
	m_quCandidateVertices.insert(start_vertex);

	//3. start searching for the shortest path
	while (!m_quCandidateVertices.empty())
	{
		multiset<BaseVertex*, WeightLess<BaseVertex> >::const_iterator pos = m_quCandidateVertices.begin();

		BaseVertex* cur_vertex_pt = *pos; //m_quCandidateVertices.top();
		m_quCandidateVertices.erase(pos);
	
		if (cur_vertex_pt == end_vertex) break;

		m_stDeterminedVertices.insert(cur_vertex_pt->getID());

		improve2vertex(cur_vertex_pt, is_source2sink);
	}
}

void DijkstraShortestPathAlg::improve2vertex( BaseVertex* cur_vertex_pt, bool is_source2sink )
{
	// 1. get the neighboring vertices 
	set<BaseVertex*>* neighbor_vertex_list_pt = new set<BaseVertex*>();
		
	if(is_source2sink)
	{
		m_pDirectGraph->get_adjacent_vertices(cur_vertex_pt, *neighbor_vertex_list_pt);
	}else
	{
		m_pDirectGraph->get_precedent_vertices(cur_vertex_pt, *neighbor_vertex_list_pt);
	}

	// 2. update the distance passing on the current vertex
	for(set<BaseVertex*>::iterator cur_neighbor_pos=neighbor_vertex_list_pt->begin(); 
		cur_neighbor_pos!=neighbor_vertex_list_pt->end(); ++cur_neighbor_pos)
	{
		//2.1 skip if it has been visited before
		if (m_stDeterminedVertices.find((*cur_neighbor_pos)->getID())!=m_stDeterminedVertices.end())
		{
			continue;
		}

		//2.2 calculate the distance
		map<BaseVertex*, double>::const_iterator cur_pos = m_mpStartDistanceIndex.find(cur_vertex_pt);
		double distance =  cur_pos != m_mpStartDistanceIndex.end() ? cur_pos->second : Graph::DISCONNECT;

		distance += is_source2sink ? m_pDirectGraph->get_edge_weight(cur_vertex_pt, *cur_neighbor_pos) : 
			m_pDirectGraph->get_edge_weight(*cur_neighbor_pos, cur_vertex_pt);

		//2.3 update the distance if necessary
		cur_pos = m_mpStartDistanceIndex.find(*cur_neighbor_pos);
		if (cur_pos == m_mpStartDistanceIndex.end() || cur_pos->second > distance)
		{
			m_mpStartDistanceIndex[*cur_neighbor_pos] = distance;
			m_mpPredecessorVertex[*cur_neighbor_pos] = cur_vertex_pt;
			
			(*cur_neighbor_pos)->Weight(distance);

			multiset<BaseVertex*, WeightLess<BaseVertex> >::const_iterator pos = m_quCandidateVertices.begin();
			for(; pos != m_quCandidateVertices.end(); ++pos)
			{
				if ((*pos)->getID() == (*cur_neighbor_pos)->getID())
				{
					break;
				}
			}
			if(pos != m_quCandidateVertices.end())
			{
				m_quCandidateVertices.erase(pos);
			}
			m_quCandidateVertices.insert(*cur_neighbor_pos);
		}
	}
}

void DijkstraShortestPathAlg::clear()
{
	m_stDeterminedVertices.clear();
	m_mpPredecessorVertex.clear();
	m_mpStartDistanceIndex.clear();
	m_quCandidateVertices.clear();
}

BasePath* DijkstraShortestPathAlg::update_cost_forward( BaseVertex* vertex )
{
	double cost = Graph::DISCONNECT;

 	// 1. get the set of successors of the input vertex
	set<BaseVertex*>* adj_vertex_set = new set<BaseVertex*>();
	m_pDirectGraph->get_adjacent_vertices(vertex, *adj_vertex_set);
 
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
		vector<BaseVertex*> vertex_list;
		vertex_list.push_back(vertex);

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
		m_pDirectGraph->get_precedent_vertices(cur_vertex_pt, pre_vertex_set);
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
