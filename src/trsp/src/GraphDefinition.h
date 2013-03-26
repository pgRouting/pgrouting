#ifndef GRAPHDEFINITION_H
#define GRAPHDEFINITION_H

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "trsp.h"

typedef std::vector<long> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<int, bool> PIB;
typedef std::pair<double, PIB> PDP;
typedef std::pair<double, std::vector<int> > PDVI;

/*
typedef struct edge 
{
	int id;
	int source;
	int target;
	double cost;
	double reverse_cost;
} edge_t;

typedef struct path_element 
{
	int vertex_id;
	int edge_id;
	double cost;
}path_element_t;
*/

typedef struct{
	int ed_ind[2];
	int v_pos[2];
}PARENT_PATH;

typedef struct{
	double cost;
	std::vector<int> precedencelist;
}Rule;

typedef struct{
	double startCost, endCost;
}CostHolder;

typedef std::map<int, std::vector<Rule> > RuleTable;



class GraphEdgeInfo
{
public:
	long m_lEdgeID;
	long m_lEdgeIndex;
	short m_sDirection;
	double m_dCost;
	double m_dReverseCost;
	LongVector m_vecStartConnectedEdge;
	LongVector m_vecEndConnedtedEdge;
	//LongVector m_vecConnectedNode;
	bool m_bIsLeadingRestrictedEdge;
	VectorOfLongVector m_vecRestrictedEdge;

	long m_lStartNode;
	long m_lEndNode;
};




typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
typedef std::map<long,LongVector> Long2LongVectorMap;
typedef std::map<long,long> Long2LongMap;




class GraphDefinition
{
public:
	GraphDefinition(void);
	~GraphDefinition(void);
	
	int my_dijkstra(edge_t *edges, unsigned int edge_count, int start_vertex, int end_vertex,
		path_element_t **path, int *path_count, char **err_msg);
	int my_dijkstra(edge_t *edges, unsigned int edge_count, int start_vertex, int end_vertex,
		path_element_t **path, int *path_count, char **err_msg, std::vector<PDVI> &ruleList);
	int my_dijkstra(edge_t *edges, unsigned int edge_count, int start_edge, double start_part, int end_edge, double end_part,
		path_element_t **path, int *path_count, char **err_msg, std::vector<PDVI> &ruleList);
	

private:
	bool construct_graph(edge_t *edges, int edge_count);
	double construct_path(int ed_id, int v_pos);
	void explore(int cur_node, GraphEdgeInfo cur_edge, bool isStart, LongVector &vecIndex, std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > &que);
	double getRestrictionCost(int cur_node, GraphEdgeInfo new_edge, bool isStart);
	bool addEdge(edge edgeIn);
	bool connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo& secondEdge, bool bIsStartNodeSame);
	bool get_single_cost(double total_cost, path_element_t **path, int *path_count);
	void init();

private:
	GraphEdgeVector m_vecEdgeVector;
	Long2LongMap m_mapEdgeId2Index;
	Long2LongVectorMap m_mapNodeId2Edge;
	int max_node_id;
	int max_edge_id;
	int m_lStartEdgeId;
	int m_lEndEdgeId;
	double m_dStartpart;
	double m_dEndPart;
	bool isStartVirtual;
	bool isEndVirtual;
	
	std::vector <path_element_t> m_vecPath;
	PARENT_PATH *parent;
	CostHolder *m_dCost;
	RuleTable m_ruleTable;
	bool m_bIsturnRestrictOn;
	bool m_bIsGraphConstructed;
};

#endif
