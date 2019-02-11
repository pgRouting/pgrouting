// Copyright 2012 steve

#ifndef INCLUDE_TRSP_GRAPHDEFINITION_H_
#define INCLUDE_TRSP_GRAPHDEFINITION_H_

#include <stdlib.h>

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <utility>
#include <functional>


#include "c_types/trsp/trsp.h"

// using namespace std;

typedef std::vector<int64> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<int64, bool> PIB;
typedef std::pair<double, PIB> PDP;
typedef std::pair<double, std::vector<int64> > PDVI;

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
}path_element_tt;
*/

typedef struct {
    int64 ed_ind[2];
    int64 v_pos[2];
} PARENT_PATH;

typedef struct Rule {
    double cost;
    std::vector<int64> precedencelist;
    Rule(double c, std::vector<int64> p) : cost(c), precedencelist(p) { }
}Rule;

typedef struct {
    double startCost, endCost;
} CostHolder;

typedef std::map<int64, std::vector<Rule> > RuleTable;



class GraphEdgeInfo {
 public:
    int64 m_lEdgeID;
    int64 m_lEdgeIndex;
    int64 m_sDirection;
    double m_dCost;
    double m_dReverseCost;
    LongVector m_vecStartConnectedEdge;
    LongVector m_vecEndConnedtedEdge;
    bool m_bIsLeadingRestrictedEdge;
    VectorOfLongVector m_vecRestrictedEdge;

    int64 m_lStartNode;
    int64 m_lEndNode;
};




typedef std::vector<GraphEdgeInfo*> GraphEdgeVector;
typedef std::map<int64, LongVector> Long2LongVectorMap;
typedef std::map<int64, int64> Long2LongMap;




class GraphDefinition {
 public:
    GraphDefinition(void);
    ~GraphDefinition(void);

    int my_dijkstra3(edge_t *edges, size_t edge_count,
                    int64 start_vertex, int64 end_vertex,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg);

    int my_dijkstra2(edge_t *edges, size_t edge_count,
                    int64 start_vertex, int64 end_vertex,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    std::vector<PDVI> &ruleList);

    int my_dijkstra1(edge_t *edges, size_t edge_count,
                    int64 start_edge, double start_part,
                    int64 end_edge, double end_part,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    std::vector<PDVI> &ruleList);

    bool construct_graph(edge_t *edges, size_t edge_count,
                         bool has_reverse_cost, bool directed);


 private:
    double construct_path(int64 ed_id, int64 v_pos);
    void explore(int64 cur_node, GraphEdgeInfo& cur_edge, bool isStart,
                 LongVector &vecIndex, std::priority_queue<PDP,
                 std::vector<PDP>, std::greater<PDP> > &que);
    double getRestrictionCost(int64 cur_node, GraphEdgeInfo& new_edge,
                              bool isStart);
    bool addEdge(edge_t edgeIn);
    bool connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo& secondEdge,
                     bool bIsStartNodeSame);
    bool get_single_cost(double total_cost, path_element_tt **path,
                         size_t *path_count);
    void init();
    void deleteall();

 private:
    GraphEdgeVector m_vecEdgeVector;
    Long2LongMap m_mapEdgeId2Index;
    Long2LongVectorMap m_mapNodeId2Edge;
    int64 max_node_id;
    int64 max_edge_id;
    int64 m_lStartEdgeId;
    int64 m_lEndEdgeId;
    double m_dStartpart;
    double m_dEndPart;
    bool isStartVirtual;
    bool isEndVirtual;

    std::vector <path_element_tt> m_vecPath;
    PARENT_PATH *parent;
    CostHolder *m_dCost;
    RuleTable m_ruleTable;
    bool m_bIsturnRestrictOn;
    bool m_bIsGraphConstructed;
};

#endif  // INCLUDE_TRSP_GRAPHDEFINITION_H_
