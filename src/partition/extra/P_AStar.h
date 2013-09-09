#ifndef P_ASTAR_H
#define P_ASTAR_H
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>


//#include "p_astar.h"
#define INF 1e15

typedef struct edge 
{
	int id;
	int source ;
	int target;
	int s_pid;
	int t_pid;
	double cost;
	double reverse_cost;
	double s_x;
	double s_y;
	double t_x;
	double t_y;
}edge_p_astar_t;

typedef struct path_element
{
 int vertex_id;
 int edge_id;
 double cost;
}path_element_t;




typedef std::vector<long> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<double, int> PDI;



typedef struct{
         int NodeID;
	 int NodeIndex;
	 int pid ;
	 double x;
         double y;	                                    
	 std::vector<int> Connected_Nodes;
         std::vector<int> Connected_Edges_Index;
}GraphNodeInfo;



struct GraphEdgeInfo

{

public:
	int EdgeID;
	int EdgeIndex;
	double Cost;
	int StartNode;
	int EndNode;
	int S_pid;
	int E_pid;
};

typedef struct{
	int par_Node;
	int par_Edge;
}PARENT_PATH;



typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
typedef std::map<long,LongVector> Long2LongVectorMap;
typedef std::map<long,long> Long2LongMap;
typedef std::vector<GraphNodeInfo> GraphNodeVector;
typedef std::map<long,PARENT_PATH>  Long2ParentMap;
typedef std::map<long,float> Long2FloatMap;

typedef struct
{
	float cost;
	int node_id;
}pq_pair;


class Compare
{
 public:
	 bool operator()(pq_pair* p1, pq_pair*p2);
};

edge_p_astar_t *
fetch_partition_edges(int pid , int* ptuples ); 


class P_AStar

{

	public:
		P_AStar(void);
		~P_AStar(void);
		
		
		int p_astar(int start_vertex, int end_vertex,int s_pid ,int t_pid,path_element_t **path , int *path_count,char **err_msg);



	private:
                
		void initall(int s_pid,int t_pid);
		void load_partition( int pid);
		bool construct_graph(edge_p_astar_t *edges ,int edge_count);
		void construct_path(int node_id);
		bool addEdge(edge_p_astar_t edgeIn);
		bool addNode(edge_p_astar_t edgeIn,int node_id);
		void deleteall();
		void explore(int cur_node, float cur_cost, std::priority_queue<pq_pair*, std::vector<pq_pair*>, Compare > &que);
		void check_whether_loaded(int node_id);  
                double getcost(int node_id );
		void setcost(int node_id,double cost);
		void setparent(int node_id , int par_node ,int par_edge);
		double getHcost(int node_id);
		double dist(double x1,double y1,double x2,double y2);

	private:
		GraphEdgeVector m_vecEdgeVector;
		Long2LongMap m_mapEdgeId2Index;
		Long2LongMap m_mapNodeId2Index;
		GraphNodeVector m_vecNodeVector;
		Long2ParentMap m_pFParent;
		Long2FloatMap m_pFCost ;

		int m_lStartNodeId;
		int m_lEndNodeId;
		double m_MinCost;
		int m_MidNode;
		std::vector <path_element_t> m_vecPath;
		
		bool loaded_partition[10000000]; 

};



#endif
