/*PGR-GNU*****************************************************************
File: pgr_contract.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

#pragma once
#include <stack>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <string>
#include <functional>
extern "C" {
#include "postgres.h"
}

#include "./pgr_contractionGraph.hpp"
#include "../../common/src/pgr_assert.h"

template < class G > class Pgr_contract;

template < class G >
void pgr_contractGraph(
	G &graph, int64_t *contraction_order,
	size_t size_contraction_order,
	int64_t max_cycles,
	std::ostringstream& contracted_graph_name,
	std::ostringstream& contracted_graph_blob,
	std::ostringstream& removedEdges,
	std::ostringstream& removedVertices,
	std::ostringstream& psuedoEdges,
	std::ostringstream& debug) {
	Pgr_contract< G > fn_contract;
	std::deque<int64_t> contract_order;
	contract_order.push_back(0);
	for (size_t i = 0; i < size_contraction_order; ++i)
	{
		contract_order.push_back(contraction_order[i]);
	}
	
	for (int64_t i = 0; i < max_cycles; ++i)
	{
		int64_t front=contract_order.front();
		debug << "Starting cycle " << i+1 << "\n";
		contract_order.pop_front();
		contract_order.push_back(front);	
		front=contract_order.front();
		while(front!=0)
		{
			switch(front)
			{
				case 0:
				debug << "Finished cycle " << i+1 << "\n";
				break;

				default:
				debug << "contraction "<< front << " asked" << "\n";
				contract_order.pop_front();
				contract_order.push_back(front);
				front=contract_order.front();
			}
		}
	}



	#if 0
    fn_contract.calculateDegrees(graph);
	fn_contract.contract_to_level(graph,level);
	fn_contract.getGraphName(contracted_graph_name,level);
	fn_contract.getGraph_string(graph,contracted_graph_blob);
	fn_contract.getRemovedE_string(graph,removedEdges);
	fn_contract.getRemovedV_string(removedVertices);
	fn_contract.getPsuedoE_string(psuedoEdges);
	#endif


}




template < class G >
class Pgr_contract {
public:
	typedef typename G::V V;
	typedef typename G::E E;
	typedef typename G::V_i V_i;
	typedef typename G::E_i E_i;
	typedef typename G::EO_i EO_i;
	typedef typename G::degree_to_V_i degree_to_V_i;


	#if 0
	void contract_to_level(
		G &graph,
		int64_t level);
	#endif

	void dead_end_contraction(G &graph);

	#if 0
	void remove_2_degree_vertices(G &graph);
	#endif

	

	void calculateDegrees(G &graph);

	void degreeMap(G &graph,std::ostringstream& dmap);

	void getGraphName(std::ostringstream& name,contractionType ctype);

	int64_t getGraph_string(G &graph,std::ostringstream& estring);
	
	void getRemovedE_string(G &graph,std::ostringstream& estring);

	void getRemovedV_string(std::ostringstream& vstring);

	void getPsuedoE_string(std::ostringstream& pstring);
	
	typedef typename std::map<V,std::deque<Edge> > removed_V;
	typedef typename std::map<V,std::deque<Edge> >::iterator removed_V_i;
	typedef typename std::map<int64_t , std::pair<int64_t,int64_t> > psuedo_E;
	typedef typename std::map<int64_t , std::pair<int64_t,int64_t> >::iterator psuedo_E_i;
	typedef std::map< int64_t, std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int> > > degree_to_V;
	typedef typename std::vector<V>::iterator Q_i;
private:
	int64_t last_edge_id;
	removed_V removedVertices;
	psuedo_E psuedoEdges;
	degree_to_V degree_to_V_map;

};


 /******************** IMPLEMENTATION ******************/


 //! \brief Calculates the degree of every vertex in the graph
     /*!
       - A map is generated which maps the degree to a vector of vertices of the particlaur degree 
       - This map changes during the process of contraction 
       */
template < class G >
void
Pgr_contract< G >::calculateDegrees(G &graph) {
	EO_i out, out_end;
	//V_i vi;
	for (auto vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) 
	{
		//graph.graph[(*vi)].degree=boost::out_degree(*vi,graph.graph);
		degree_to_V_map[graph.graph[(*vi)].degree].push((*vi).id);
	}
}


 //! \brief Generates the name of the contracted graph based upon the type of contraction
template < class G >
void
Pgr_contract< G >::getGraphName(std::ostringstream& name,contractionType ctype)
{
	name << "contracted_graph_" << static_cast<int>(ctype);
}


#if 0
template < class G >
void
Pgr_contract< G >::contract_to_level(G &graph,int64_t level)
{
	switch(level)
	{
		case 0:
		remove_1_degree_vertices(graph);
		break;
		case 1:
		remove_2_degree_vertices(graph);
		default:
		// do nothing
		;
	}
}
#endif


#if 0
 //! \brief Returns the *degree_to_V_map* in string format
template < class G >
void
Pgr_contract< G >::degreeMap(G &graph,std::ostringstream& dmap)
{
	//cout << "Printing degree_V map" << endl;
	degree_to_V_i it1;
	dmap << "This is a degree map " ;
	Q_i it2;
	for ( it1=degree_to_V_map.begin(); it1!=degree_to_V_map.end(); ++it1)
	{
		dmap << it1->first << "-->" ;
		for (it2=it1->second.begin(); it2 !=it1->second.end(); ++it2)
		{
			dmap << graph.graph[(*it2)].id << ", ";
		}
		dmap << "\n";
	}
}
#endif

//! \brief Performs dead-end contraction on the graph
     /*!
      
       - Picks up vertices in the order of the vertex id
       - Removes all those vertices with one incoming edge(directed)
       - In case of undirected the vertices having one incoming and one outgoing edge(both of same id)
       	 will be chosen.

       */
template < class G >
void
Pgr_contract< G >::dead_end_contraction(G &graph) {
	E_i in,in_end;
	V front;
	//errors << "first vertex: " << front ;
	//std::vector<V> one_degree_vertices=degree_to_V_map[1];
	degree_to_V_i it;
	//std::cout << "1 degree vertices " << one_degree_vertices.size();
	//std::cout << "Front id" << frontid;
	//graph.m_num_vertices--;	
	while (degree_to_V_map[1].size()>0)
	{
		//std::cout << "Front " << graph.graph[front].id;
		V front=degree_to_V_map[1].top();
		degree_to_V_map[1].pop();
		int64_t frontid=graph.graph[front].id;
		boost::tie(in, in_end) = in_edges(front, graph.graph);
		V s=source(*in, graph.graph);
		V t=target(*in, graph.graph);
		int64_t source_id=graph.graph[s].id;
		int64_t target_id=graph.graph[t].id;
		graph.graph[t].type=1;
		//degree_to_V_map[final_target_degree].push_back(t);
		//Edge removed_edge=graph.graph[*in];
		//graph.graph[s].removed_vertices.
		//removedVertices[frontid].push_front(removed_edge);
		graph.disconnect_vertex_c(target_id);
		graph.m_num_vertices--;
		int source_degree=boost::in_degree(s, graph.graph);
		degree_to_V_map[source_degree].push(s);
		
	}
}

#if 0
template < class G >
void
Pgr_contract< G >::remove_2_degree_vertices(G &graph) {

	EO_i out,out_end;
	std::vector<V> neighbors_desc;
	std::vector<V> two_degree_vertices=degree_to_V_map[2];
	std::vector<V> two_degree_vertices_0;
	for (Q_i it = two_degree_vertices.begin() ; it != two_degree_vertices.end(); ++it)
	{
			//cout << "id" << reduced_graph->graph[*it].id << endl;
			//cout << "contractions" << reduced_graph->graph[*it].contractions << endl;
		if (graph.graph[*it].contractions==0)
		{


			two_degree_vertices_0.push_back(*it);
		}
	}
	degree_to_V_i it;
	//std::cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
	if (two_degree_vertices_0.size()==0)
	{
		return;
	}
	V front=two_degree_vertices_0.front();
	int64_t front_id=-1;
	while (two_degree_vertices_0.size()>0)
	{
			//cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
			//cout << "Edge count" << num_edges(reduced_graph->graph) << endl;
		std::cout << "Front " << graph.graph[front].id ;
		neighbors_desc.clear();
		front=two_degree_vertices_0.front();
		front_id=graph.graph[front].id;
		if (graph.graph[front].contractions==0)
		{


			int count=0;
			int64_t tid1=-1,tid2=-1;
			V  tdesc1=-1,tdesc2=-1;
			float min_distance=0.00000;
			for (boost::tie(out, out_end) = out_edges(front, graph.graph);
				out != out_end; ++out) 
			{
				V t=target(*out, graph.graph);
				min_distance+=graph.graph[*out].cost;
				graph.graph[t].contractions++;
				neighbors_desc.push_back(t);
				count++;
			}
			if (count==2)
			{
				Edge shortcut;
				tdesc1=neighbors_desc[0];
				tdesc2=neighbors_desc[1];
				tid1=graph.graph[tdesc1].id;
				tid2=graph.graph[tdesc2].id;
				E edesc1=edge(tdesc1, front,graph.graph).first;
				E edesc2=edge(front, tdesc2,graph.graph).first;
				int64_t eid1=graph.graph[edesc1].id;
				int64_t eid2=graph.graph[edesc2].id;
				int64_t eid;
				bool b1=edge(tdesc1, tdesc2,graph.graph).second;
				bool b2=edge(tdesc2, tdesc1,graph.graph).second;
				if ( b1 || b2)
				{
					E edesc=edge(tdesc1, tdesc2,graph.graph).first;
					eid=graph.graph[edesc].id;
					float dist=graph.graph[edesc].cost;
					
					if (min_distance-dist>=0.000001)
					{
						min_distance=dist;
					}
					else
					{
						//std::cout << "Updating shortcut " << "(" << eid1 << ", " << eid2 << ")" << " with " << min_distance << endl;
						graph.graph[edesc].cost=min_distance;
						graph.graph[edesc].type=1;
					}
				}
				else
				{
					shortcut.id=++graph.last_edge_id;
					eid=shortcut.id;
					shortcut.source=tid1;
					shortcut.target=tid2; 
					shortcut.cost=min_distance;
					shortcut.type=2;
					graph.graph_add_shortcut(shortcut);
				}
				psuedoEdges[eid]=std::make_pair(eid1,eid2);
				Edge removed_edge;
				for (int i = 0; i < 2; ++i)
				{
					V t=neighbors_desc[i];
					removed_edge=graph.graph[edge(front, t,graph.graph).first];
					removedVertices[front_id].push_front(removed_edge);
				}
				graph.disconnect_vertex(front_id);
				graph.m_num_vertices--;
			}
		}
		two_degree_vertices_0.erase(two_degree_vertices_0.begin(),
			two_degree_vertices_0.begin()+1);

	}
}
#endif

//! \brief Returns the information about the contracted graph in string format
     /*!
       
       - Information about the outgoig edges of a particular vertex gets stored
       - The information includes id,source,target,cost
       - Edge information is delimited by "$" symbol  

       */
template < class G >
int64_t
Pgr_contract< G >::getGraph_string(G &graph,std::ostringstream& estring)
{
	V_i vi;
	EO_i out,out_end;
	int64_t count=0;
	for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
		for (boost::tie(out, out_end) = out_edges(*vi, graph.graph);
			out != out_end; ++out)
		{
			V source_desc=source(*out, graph.graph);
			V target_desc=target(*out, graph.graph);
			int64_t source_id=graph.graph[source_desc].id;
			int64_t target_id=graph.graph[target_desc].id;

			if (target_desc>source_desc)
			{
				estring << graph.graph[*out].id << ","
				<< source_id << "," << target_id << ","
				<< graph.graph[*out].cost << ","
				<< graph.graph[*out].reverse_cost << "$";
				count++;
			}

		}
	}
	return count;
}


//! \brief Returns the information about the removed edges after contraction in string format
     /*!
      
       - The information of the removed edge includes id,source,target,cost
       - Edge information is delimited by "$" symbol  

       */
template < class G >
void
Pgr_contract< G >::getRemovedE_string(G &graph,std::ostringstream& estring)
{
	
	#if 0
	for (removed_E_i iter = graph.removed_edges.begin(); iter != graph.removed_edges.end(); iter++)
	{
		// Edge temp=*iter ;
		estring << iter->id << "," << iter->source << ","
		<< iter->target << "," << iter->cost << "$"; 
		// << "," << iter->reverse_cost << "$";
	}
	#endif

	for (const auto re : graph.removed_edges_c) 
		estring << re.first << "," << re.second.source << ","
	<< re.second.target << "," << re.second.cost << ","
	<<re.second.reverse_cost<< "$";
}
//! \brief Returns the information about the removed vertices after contraction in string format
     /*!
      
       - The information of the removed vertex includes id
       - Edge information is delimited by "$" symbol  

       */
template < class G >
void
Pgr_contract< G >::getRemovedV_string(std::ostringstream& vstring)
{
	for (removed_V_i iter = removedVertices.begin(); iter != removedVertices.end(); iter++)
	{
		int64_t vid=iter->first;
		std::deque<Edge>::iterator edge_iter;
		for (edge_iter = iter->second.begin(); edge_iter != iter->second.end(); edge_iter++)
		{
			Edge temp=*edge_iter ;
			vstring << vid <<","<< temp.id << ","
			<< temp.source << "," << temp.target << ","
			<< temp.cost << "," << temp.reverse_cost << "$";
		}
		//std::cout << endl;
	}		

}

//! \brief Returns the information about the new edges(shortcuts) after contraction in string format
     /*!
      
       - The information of the shortcut includes id,source,target,cost
       - Edge information is delimited by "$" symbol  

       */
template < class G >
void
Pgr_contract< G >::getPsuedoE_string(std::ostringstream& pstring)
{
	for (const auto pe : psuedoEdges)
	{
		pstring << pe.first << "," << pe.second.first << ","
		<< pe.second.second << "$";
	}

}

