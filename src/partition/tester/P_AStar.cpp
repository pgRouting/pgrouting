

#include "P_AStar.h"
using namespace std;



bool Compare::operator() ( pq_pair* p1, pq_pair *p2)
{
	if(p1->cost < p2->cost)
		return true;
	return false;
}




P_AStar::P_AStar(void)
{
}


P_AStar::~P_AStar(void)
{
}

//Load the initial edges that correspond to the partition id of the source and target and initialize a few variables.

void P_AStar::initall(int s_pid, int t_pid )
{
          m_vecNodeVector.clear();
	  m_vecEdgeVector.clear();
	  m_pFParent.clear();
	  m_pFCost.clear();
	  for(int i=0;i<10000000;i++)
		  loaded_partition=false;            // we can use a vector here instead of a bool array 

          load_partition(s_pid);
          load_partition(t_pid); 	  

	  m_MinCost=INF;
	  m_MidNode=-1;
}


// check whether the partition in which the node lies is loaded or not ,if not load it .
void P_AStar::check_whether_loaded(int node_id)
{

	Long2LongMap::iterator it = m_mapNodeId2Index.find(node_id);

	//check whether the node exists the node vector or not .

        if(!loaded_partition[m_vecNodeVector[it->second].pid])
	{
                load_partition(m_vecNodeVector[it->second].pid);
	}	


}

double P_AStar::getcost(int node_id)
{
	Long2FloatMap::iterator it =m_PFcost.find(node_id); 
         if(it != m_PFcost.end())
		 return it->second;
	
	return INF; 


}

void P_AStar::setcost(int node_id , double cost )
{

	Long2FloatMap::iterator it =m_PFcost.find(node_id); 
        
       	if(it != m_PFcost.end())
        {
		it->second=cost;
	}
        else
	{
		 m_PFcost.insert(std::make_pair(node_id,cost));

	}	

}

void P_AStar::setparent(int node_id, int par_node , int par_edge)
{
       PARENT_PATH *p2= new PARENT_PATH;
       p2->par_Node=par_node;
       p2->par_Edge=par_edge;
       m_pFParent.insert(std::make_pair(node_id,*p2));

}

double P_AStar::gethcost( int node_id)
{

	  Long2LongMap::iterator it1 = m_mapNodeId2Index.find(node_id);
	  int node_index=it1->second;
	  Long2LongMap::iterator it2 = m_mapNodeId2Index.find(m_lEndNodeId);
	  int target_node_index=it2->second;

	return(dist(m_vecNodeVector[node_index].x, m_vecNodeVector[node_index].y, m_vecNodeVector[target_node_index].x,
			       	m_vecNodeVector[target_node_index].y));



}


double P_AStar::dist(double x1, double y1, double x2, double y2)
{
	double ret = fabs((x1 - x2) + fabs(y1 - y2));
	return(ret * 10);
}

// construct the path from the source node

void P_AStar::construct_path(int node_id)
{
	Long2ParentMap::iterator it=m_pFParent.find(node_id);                     // still to be tested , i dont know if iterators	
	if(it->second.par_Node==-1)                                               // work in recursion or not;  
		return;
	construct_path(it->second.par_Node);                                       
	path_element_t pt;
	pt.vertex_id = it->second.par_Node;
	pt.edge_id = it->second.par_Edge;
	Long2FloatMap:: iterator t1=m_PFcost.find(it->first);
	Long2FloatMap:: iterator t2=m_PFcost.find(it->second.par_Node);
        pt.cost= t1->second - t2->second ;
	m_vecPath.push_back(pt);

}
void P_AStar::deleteall()
{
	m_pFParent.clear();
	m_pFCost.clear();
	m_vecNodeVector.clear();
	m_vecEdgeVector.clear();
}

void P_AStar::explore(int cur_node, float cur_cost, std::priority_queue<pq_pair*, std::vector<pq_pair*>, Compare > &que)
{
                               
          int i ;
	  int con_edge;
          
	  Long2LongMap::iterator it = m_mapNodeId2Index.find(cur_node);
	  int node_index = it->second;
	  
	  con_edge= m_vecNodeVector[node_index].Connected_Edges_Index.size();

	  double edge_cost;

          for(i=0;i<con_edge;i++)
	  {
		  //get the edge
		  int edge_index=m_vecNodeVector[node_index].Connected_Edges_Index[i];
                  GraphEdgeInfo edge = m_vecEdgeVector[edge_index];
                  

		 //get the connected node 
		  int con_node=m_vecNodeVector[node_index].Connected_Nodes[i];


		  if(cur_node==edge.StartNode)
		  {

			  edge_cost=edge.Cost;

			  if(cur_cost + edge_cost < getcost(con_node))
			  {                                                              // explore the node and push it into min heap

                                           setcost(con_node, cur_cost + edge_cost);

					   setparent(con_node,cur_node,edge.EdgeId);

					   pq_pair *p1 =new pq_pair ;
					   p1->node_id=con_node;
					   p1->cost=cur_cost + edge_cost + getHcost(con_node);
					   que.push(p1);

					   if(getcost(con_node)<m_MinCost)
					   {
						   m_MinCost=getcost(con_node);         // minimum cost update so far 
						   m_MidNode=con_node;
					   }
                                              
			  }                                

		  }
		  else
		  {
                                
			  edge_cost=edge.Cost;

			  if(cur_cost + edge_cost < getcost(con_node))
			  {
                                           setcost(con_node, cur_cost + edge_cost);

					   setparent(con_node,cur_node,edge.EdgeId);

					   pq_pair *p1 =new pq_pair ;
					   p1->node_id=con_node;
					   p1->cost=cur_cost + edge_cost + getHcost(con_node);
					   que.push(p1);

					   if(getcost(con_node)<m_MinCost)
					   {
						   m_MinCost=getcost(con_node);
						   m_MidNode=con_node;
					   }
                                              
			  }                                
		  }

	  } 

}

//This is the main solver class where the the shortest path is computed

int P_AStar::p_astar(int start_vertex,int end_vertex,int s_pid, int t_pid ,char **err_msg)
{

	initall(s_pid,t_pid);

	m_lStartNodeId = start_vertex;
	m_lEndNodeId = end_vertex;
        
	m_vecPath.clear();
	std::priority_queue<pq_pair*, std::vector<pq_pair*>, Compare > pque;

	PARENT_PATH *ptr= new PARENT_PATH;
	ptr->par_Node=-1;
	ptr->Par_Edge=-1;

	m_pFParent.insert(std::make_pair(start_vertex,*ptr));

	m_PFcost.insert(std::make_pair(start_vertex,0.0)); 

	pq_pair *ptr1=new pq_pair;                                         // push start vertex along with the cost into min heap

	ptr1->node_id=start_vertex;
	ptr1->cost=0.0;
	pque.push(ptr1);	

	int new_node;
	int cur_node;

// the algorithm starts from here , breaks when it reaches the target node 	

// still to be tested , point out flaws if you see any .

       while(!pque.empty())
       {
                 pq_pair *ptr3;
		 ptr3=pque.top();

		 check_whether_loaded(ptr3->node_id);

		 Long2FloatMap::iterator it = m_PFcost.find(ptr3->node_id);
                
                 if(  m_MidNode==end_vertex)
		 {
			 //  reached target
			 construct_path(m_MidNode);
	                       break;
		 }  		 
                 if(ptr3->cost > m_MinCost)
			break;
		 cur_node=ptr3->node_id;
		 pque.pop();
		 delete ptr3;

		 explore(cur_node,it->second,pque); 
	}
	
       
       if(m_MidNode==-1)
       {
	       *err_msg = (char *)"Path Not Found";
	       deleteall();
	       return -1;
       }
       
       else 
       {


	       // Transfer data path to path_element_t format and allocate memory and populate the pointer
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


//  This fuction fetches edges using partion id 
void P_AStar::load_partition(int pid)

{    
	 edge_p_astar_t *edges; 
         int total_tuples=-1;		                            
	 
	 edges=fetch_partition_edges(pid, &total_tuples);

	
         if(total_tuples!=-1 && edges!=NULL)
		 loaded_partition[pid]=true ;                 // check for total tuples and edges and set the correspondin loaded partition                                                             //    value as true.   

	 
	 construct_graph(edges,total_tuples);                 

	 
}

bool P_AStar::construct_graph(edge_p_astar_t *edges ,int edge_count)
{             

       int i;

       for(i=0;i<edge_count;i++)
       {
              addEdge( edges[i]);

	      free(edge[i]);

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




