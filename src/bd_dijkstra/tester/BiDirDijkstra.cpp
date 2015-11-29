/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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
/******************************************************************************
* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

******************************************************************************
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies of this Software or works derived from this Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.

*****************************************************************************/

#include "BiDirDijkstra.h"

BiDirDijkstra::BiDirDijkstra(void)
{
}

BiDirDijkstra::~BiDirDijkstra(void)
{
}

void BiDirDijkstra::init()
{
	//max_edge_id = 0;
	//max_node_id = 0;
	
}

void BiDirDijkstra::initall(int maxNode)
{
	int i;
	m_pFParent = new PARENT_PATH[maxNode + 1];
	m_pRParent = new PARENT_PATH[maxNode + 1];

	m_pFCost = new double[maxNode + 1];
	m_pRCost = new double[maxNode + 1];

	for(i = 0; i <= maxNode; i++)
	{
		m_pFParent[i].par_Node = -2;
		m_pRParent[i].par_Node = -2;
		m_pFCost[i] = INF;
		m_pRCost[i] = INF;
		
	}
	m_MinCost = INF;
	m_MidNode = -1;
}

void BiDirDijkstra::deleteall()
{
	delete [] m_pFParent;
	delete [] m_pRParent;
	delete [] m_pFCost;
	delete [] m_pRCost;
}

double BiDirDijkstra::getcost(int node_id, int dir)
{
	if(dir == 1)
	{
		return(m_pFCost[node_id]);
	}
	else
	{
		return(m_pRCost[node_id]);
	}
}

void BiDirDijkstra::setcost(int node_id, int dir, double c)
{
	if(dir == 1)
	{
		m_pFCost[node_id] = c;
	}
	else
	{
		m_pRCost[node_id] = c;
	}
}

void BiDirDijkstra::setparent(int node_id, int dir, int parnode, int paredge)
{
	if(dir == 1)
	{
		m_pFParent[node_id].par_Node = parnode;
		m_pFParent[node_id].par_Edge = paredge;
	}
	else
	{
		m_pRParent[node_id].par_Node = parnode;
		m_pRParent[node_id].par_Edge = paredge;
	}
}

void BiDirDijkstra::fconstruct_path(int node_id)
{
	if(m_pFParent[node_id].par_Node == -1)
		return;
	fconstruct_path(m_pFParent[node_id].par_Node);
	path_element_t pt;
	pt.vertex_id = m_pFParent[node_id].par_Node;
	pt.edge_id = m_pFParent[node_id].par_Edge;
	pt.cost = m_pFCost[node_id] - m_pFCost[m_pFParent[node_id].par_Node];
	m_vecPath.push_back(pt);
}

void BiDirDijkstra::rconstruct_path(int node_id)
{
	path_element_t pt;
	if(m_pRParent[node_id].par_Node == -1)
	{
		pt.vertex_id = node_id;
		pt.edge_id = -1;
		pt.cost = 0.0;
		return;
	}
	pt.vertex_id = node_id;
	pt.cost = m_pRCost[node_id] - m_pRCost[m_pRParent[node_id].par_Node];
	pt.edge_id = m_pRParent[node_id].par_Edge;
	m_vecPath.push_back(pt);
	rconstruct_path(m_pRParent[node_id].par_Node);
}

void BiDirDijkstra::explore(int cur_node, double cur_cost, int dir, std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > &que)
{
	int i;
	int con_edge = m_vecNodeVector[cur_node].Connected_Edges_Index.size();
	double edge_cost;
	for(i = 0; i < con_edge; i++)
	{
		int edge_index = m_vecNodeVector[cur_node].Connected_Edges_Index[i];
		GraphEdgeInfo edge = m_vecEdgeVector[edge_index];
		int new_node = m_vecNodeVector[cur_node].Connected_Nodes[i];
		int mult;
		
		if(edge.Direction == 0)
			mult = 1;
		else
			mult = dir;
		if(cur_node == edge.StartNode)
		{
			if(dir > 0)
				edge_cost = edge.Cost;
			else
				edge_cost = edge.ReverseCost;
			if(edge.Direction == 0 || edge_cost >= 0.0)
			{
				//edge_cost = edge.Cost * mult;
				
				if(cur_cost + edge_cost < getcost(new_node, dir))
				{
					setcost(new_node, dir, cur_cost + edge_cost);
					setparent(new_node, dir, cur_node, edge.EdgeID);
					que.push(std::make_pair(cur_cost + edge_cost, new_node));

					if(getcost(new_node, dir) + getcost(new_node, dir * -1) < m_MinCost)
					{
						m_MinCost = getcost(new_node, dir) + getcost(new_node, dir * -1);
						m_MidNode = new_node;
					}
				}
			}
		}
		else
		{
			if(dir > 0)
				edge_cost = edge.ReverseCost;
			else
				edge_cost = edge.Cost;
			if(edge.Direction == 0 || edge_cost >= 0.0)
			{
				//edge_cost = edge.ReverseCost * mult;

				if(cur_cost + edge_cost < getcost(new_node, dir))
				{
					setcost(new_node, dir, cur_cost + edge_cost);
					setparent(new_node, dir, cur_node, edge.EdgeID);
					que.push(std::make_pair(cur_cost + edge_cost, new_node));

					if(getcost(new_node, dir) + getcost(new_node, dir * -1) < m_MinCost)
					{
						m_MinCost = getcost(new_node, dir) + getcost(new_node, dir * -1);
						m_MidNode = new_node;
					}
				}
			}
		}
	}
}

int BiDirDijkstra:: bidir_dijkstra(edge_t *edges, unsigned int edge_count, int maxNode, int start_vertex, int end_vertex,
				path_element_t **path, int *path_count, char **err_msg)
{
	max_node_id = maxNode;
	max_edge_id = -1;
	init();
	construct_graph(edges, edge_count, maxNode);
	

	int nodeCount = m_vecNodeVector.size();
		
	std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > fque;
	std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > rque;
	
	m_vecPath.clear();

	int i;
	initall(maxNode);

	m_pFParent[start_vertex].par_Node = -1;
	m_pFParent[start_vertex].par_Edge = -1;
	m_pFCost[start_vertex] = 0.0;
	fque.push(std::make_pair(0.0, start_vertex));

	m_pRParent[end_vertex].par_Node = -1;
	m_pRParent[end_vertex].par_Edge = -1;
	m_pRCost[end_vertex] = 0.0;
	rque.push(std::make_pair(0.0, end_vertex));


	int new_node;
	int cur_node;
	int dir;

	while(!fque.empty() && !rque.empty())
	{
		PDI fTop = fque.top();
		PDI rTop = rque.top();
		if(fTop.first + rTop.first > m_MinCost) //We are done, there is no path with lower cost
			break;

		if(rTop.first < fTop.first) // Explore from reverse queue
		{
			cur_node = rTop.second;
			int dir = -1;
			rque.pop();
			explore(cur_node, rTop.first, dir, rque);
		}
		else                        // Explore from forward queue
		{
			cur_node = fTop.second;
			int dir = 1;
			fque.pop();
			explore(cur_node, fTop.first, dir, fque);
		}
	}

	if(m_MidNode == -1)
	{
		*err_msg = (char *)"Path Not Found";
		deleteall();
		return -1;
	}
	else
	{
		fconstruct_path(m_MidNode);
		rconstruct_path(m_MidNode);
		path_element_t pelement;
		pelement.vertex_id = end_vertex;
		pelement.edge_id = -1;
		pelement.cost = 0.0;
		m_vecPath.push_back(pelement);

		*path = (path_element_t *) malloc(sizeof(path_element_t) * (m_vecPath.size() + 1));
		*path_count = m_vecPath.size();

		for(i = 0; i < *path_count; i++)
		{
			(*path)[i].vertex_id = m_vecPath[i].vertex_id;
			(*path)[i].edge_id = m_vecPath[i].edge_id;
			(*path)[i].cost = m_vecPath[i].cost;
		}
		
	}
	deleteall();
	return 0;
}


bool BiDirDijkstra::construct_graph(edge_t* edges, int edge_count, int maxNode)
{
	int i;
	GraphNodeInfo nodeInfo;
	nodeInfo.Connected_Edges_Index.clear();
	nodeInfo.Connected_Nodes.clear();

	for(i = 0; i <= maxNode; i++)
	{
		nodeInfo.NodeID = i;
		m_vecNodeVector.push_back(nodeInfo);
	}

	for(i = 0; i < edge_count; i++)
	{
		addEdge(edges[i]);
	}

	return true;
}


bool BiDirDijkstra::addEdge(edge_t edgeIn)
{
	long lTest;
	Long2LongMap::iterator itMap = m_mapEdgeId2Index.find(edgeIn.id);
	if(itMap != m_mapEdgeId2Index.end())	
		return false;

	
	GraphEdgeInfo newEdge;
	newEdge.EdgeID = edgeIn.id;
	newEdge.EdgeIndex = m_vecEdgeVector.size();	
	newEdge.StartNode = edgeIn.source;
	newEdge.EndNode = edgeIn.target;
	newEdge.Cost = edgeIn.cost;
	newEdge.ReverseCost = edgeIn.reverse_cost;
	if(newEdge.Cost >= 0.0 && newEdge.ReverseCost >= 0)
	{
		newEdge.Direction = 0;
	}
	else if(newEdge.Cost >= 0.0)
	{
		newEdge.Direction = 1;
	}
	else
	{
		newEdge.Direction = -1;
	}

	if(edgeIn.id > max_edge_id)
	{
		max_edge_id = edgeIn.id;
	}

	if(newEdge.StartNode > max_node_id)
	{
		return false;//max_node_id = newEdge.StartNode;
	}
	if(newEdge.EndNode > max_node_id)
	{
		return false;//max_node_id = newEdge.EdgeIndex;
	}

	m_vecNodeVector[newEdge.StartNode].Connected_Nodes.push_back(newEdge.EndNode);
	m_vecNodeVector[newEdge.StartNode].Connected_Edges_Index.push_back(newEdge.EdgeIndex);

	m_vecNodeVector[newEdge.EndNode].Connected_Nodes.push_back(newEdge.StartNode);
	m_vecNodeVector[newEdge.EndNode].Connected_Edges_Index.push_back(newEdge.EdgeIndex);


	
	//Adding edge to the list
	m_mapEdgeId2Index.insert(std::make_pair(newEdge.EdgeID, m_vecEdgeVector.size()));
	m_vecEdgeVector.push_back(newEdge);

	//
	return true;
}
