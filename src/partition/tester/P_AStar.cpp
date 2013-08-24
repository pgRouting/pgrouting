

#include "P_AStar.h"


P_AStar::P_AStar(void)
{
}


P_AStar::~P_AStar(void)
{
}

void P_AStar::initall(int source_vertex, int target_vertex )
{
          m_vecNodeVector.clear();
	  m_vecEdgeVector.clear();

          load_partition(source_vertex);
          load_partition(target_vertex); 	  

	  m_MinCost=INF;
	  m_MidNode=-1;
}




void P_AStar::p_astar(int start_vertex,int end_vertex,char **err_msg)
{

        initall(start_vertex,end_vertex);


}

void P_AStar::load_partition(int vertex_id)

{    
	 edge_p_astar_t *edges; 
		 
	 edges=fetch_partition_edges(vertex_id);

	 construct_graph(edges,total_tuples);                  //total tuples is declared globally in header file of c file.

	 
}

bool P_AStar::construct_graph(edge_p_astar_t *edges ,int edge_count)
{             

       int i;

       for(i=0;i<edge_count;i++)
       {
              addEdge( edges[i]);

       }


}



bool P_AStar::addEdge(edge_p_astar_t edgeIn)
{  

	
	// Check if the edge is already processed.
	Long2LongMap::iterator itMap = m_mapEdgeId2Index.find(edgeIn.id);
	if(itMap != m_mapEdgeId2Index.end())	
		return false;

// Create a GraphEdgeInfo using the information of the current edge

	GraphEdgeInfo newEdge;
	
	newEdge.EdgeID = edgeIn.id;
	newEdge.EdgeIndex = m_vecEdgeVector.size();	
	newEdge.StartNode = edgeIn.source;
	newEdge.EndNode = edgeIn.target;
	newEdge.Cost = edgeIn.cost;
       
// check whether source or traget nodes are already present in the node vector ,if present update conncted nodes and conncetd edges index
// if it is not present push it to the node vector.       	
	
	Long2LongMap::iterator it = m_mapNodeId2Index.find(edgeIn.source);
      
	if(it != m_mapNodeId2Index.end())
	{
             
              m_vecNodeVector[it->second].Connected_Nodes.push_back(edgeIn.target);

              m_vecNodeVector[it->second].Connected_Edges_Index.push_back(new_Edge.Edge_Index);
                
        }
	else 
	{
		GraphNodeInfo nodeInfo;
		nodeInfo.Connected_Edges_Index.clear();
		nodeInfo.Connected_Nodes.clear();

		nodeInfo.NodeID = edgeIn.source;
		nodeInfo.NodeIndex = m_vecNodeVector.size();   
		nodeInfo.pid = edgeIn.s_pid;
		nodeInfo.x = edgeIn.s_x;
		nodeInfo.y = edgeIn.s_y;

		nodeInfo.Connected_Edges.push_back(new_Edge.Edge_Index);
		nodeInfo.Connected_Nodes.push_back(edgeIn.target);
		
		m_mapNodeId2Index.insert(std::make_pair(nodeinfo.NodeId, m_vecNodeVector.size()));
		m_vecNodeVector.push_back(nodeInfo);
	}
  

	Long2LongMap::iterator it1 = m_mapNodeId2Index.find(edgeIn.target);
      
	if(it1 != m_mapNodeId2Index.end())
	{
             
              m_vecNodeVector[it->second].Connected_Nodes.push_back(edgeIn.source);

              m_vecNodeVector[it->second].Connected_Edges_Index.push_back(new_Edge.Edge_Index);
                
        }
	else 
	{
		GraphNodeInfo nodeInfo;
		nodeInfo.Connected_Edges_Index.clear();
		nodeInfo.Connected_Nodes.clear();

		nodeInfo.NodeID = edgeIn.target;
		nodeInfo.NodeIndex = m_vecNodeVector.size();   
		nodeInfo.pid = edgeIn.t_pid;
		nodeInfo.x = edgeIn.t_x;
		nodeInfo.y = edgeIn.t_y;

		nodeInfo.Connected_Edges.push_back(new_Edge.Edge_Index);
		nodeInfo.Connected_Nodes.push_back(edgeIn.source);
		
		m_mapNodeId2Index.insert(std::make_pair(nodeinfo.NodeId, m_vecNodeVector.size()));
		m_vecNodeVector.push_back(nodeInfo);
	}
  

	m_mapEdgeId2Index.insert(std::make_pair(newEdge.EdgeID, m_vecEdgeVector.size()));
	m_vecEdgeVector.push_back(newEdge);


}




