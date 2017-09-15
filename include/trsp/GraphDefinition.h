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

typedef std::vector<long> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<long, bool> PIB;
typedef std::pair<double, PIB> PDP;
typedef std::pair<double, std::vector<long> > PDVI;

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
    long ed_ind[2];
    long v_pos[2];
} PARENT_PATH;

typedef struct Rule {
    double cost;
    std::vector<long> precedencelist;
    Rule(double c, std::vector<long> p) : cost(c), precedencelist(p) { }
}Rule;

typedef struct {
    double startCost, endCost;
} CostHolder;

typedef std::map<long, std::vector<Rule> > RuleTable;



class GraphEdgeInfo {
 public:
    long m_lEdgeID;
    long m_lEdgeIndex;
    short m_sDirection;
    double m_dCost;
    double m_dReverseCost;
    LongVector m_vecStartConnectedEdge;
    LongVector m_vecEndConnedtedEdge;
    // LongVector m_vecConnectedNode;
    bool m_bIsLeadingRestrictedEdge;
    VectorOfLongVector m_vecRestrictedEdge;

    long m_lStartNode;
    long m_lEndNode;
};




typedef std::vector<GraphEdgeInfo*> GraphEdgeVector;
typedef std::map<long, LongVector> Long2LongVectorMap;
typedef std::map<long, long> Long2LongMap;




class GraphDefinition {
 public:
    GraphDefinition(void);
    ~GraphDefinition(void);

    int my_dijkstra(long start_vertex, long end_vertex,
                    size_t edge_count, char** err_msg);

    int my_dijkstra(edge_t *edges, size_t edge_count,
                    long start_vertex, long end_vertex,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg);

    int my_dijkstra(edge_t *edges, size_t edge_count,
                    long start_vertex, long end_vertex,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    std::vector<PDVI> &ruleList);

    int my_dijkstra(edge_t *edges, size_t edge_count,
                    long start_edge, double start_part,
                    long end_edge, double end_part,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    std::vector<PDVI> &ruleList);

    int multi_dijkstra(edge_t *edges, size_t edge_count,
                       std::vector<int> vertices,
                       bool directed, bool has_reverse_cost,
                       path_element_tt **path, size_t *path_count,
                       char **err_msg,
                       std::vector<PDVI> &ruleList);

    bool construct_graph(edge_t *edges, size_t edge_count,
                         bool has_reverse_cost, bool directed);


 private:
    double construct_path(long ed_id, long v_pos);
    void explore(long cur_node, GraphEdgeInfo& cur_edge, bool isStart,
                 LongVector &vecIndex, std::priority_queue<PDP,
                 std::vector<PDP>, std::greater<PDP> > &que);
    double getRestrictionCost(long cur_node, GraphEdgeInfo& new_edge,
                              bool isStart);
    bool addEdge(edge edgeIn);
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
    long max_node_id;
    long max_edge_id;
    long m_lStartEdgeId;
    long m_lEndEdgeId;
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
