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


#include "p_astar.h"
#define INF 1e15


typedef std::vector<long> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<double, int> PDI;



typedef struct{
         int NodeID;
	 int NodeIndex;
	 int pid ;
	 float x;
         float y;	                                    
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

typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
typedef std::map<long,LongVector> Long2LongVectorMap;
typedef std::map<long,long> Long2LongMap;
typedef std::vector<GraphNodeInfo> GraphNodeVector;




class P_AStar

{

	public:
		P_AStar(void);
		~P_AStar(void);
		
		
		int p_astar(int start_vertex, int end_vertex, char **err_msg);



	private:
                
		void initall(int source_vertex,int target_vertex);
		void load_partition( int vertex_id);
		bool construct_graph(edge_p_astar_t *edges int edge_count);
		void construct_path(int node_id);
		bool addEdge(edge_p_astar_t edgeIn);
		bool addNode(edge_p_astar_t edgeIn,int node_id);
		void deleteall();
		void explore(int cur_node, double cur_cost,Priority_queue pque);



	private:
		GraphEdgeVector m_vecEdgeVector;
		Long2LongMap m_mapEdgeId2Index;
		Long2LongMap m_mapNodeId2Index;
		GraphNodeVector m_vecNodeVector;

		// a min heap will be required as well // 
		int m_lStartNodeId;
		int m_lEndNodeId;
		double m_MinCost;
		int m_MidNode;

		bool loaded_partition[10000000]={false};

};



#endif
