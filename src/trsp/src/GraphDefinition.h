/*PGR-GNU*****************************************************************

FILE: GraphDefinition.h

Copyright (c) 2011-2017 pgRouting developers
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
aint64_t with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#ifndef SRC_TRSP_SRC_GRAPHDEFINITION_H_
#define SRC_TRSP_SRC_GRAPHDEFINITION_H_

#include <stdlib.h>

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <utility>
#include <functional>


#include "c_types/trsp_types.h"


typedef std::vector<int64_t> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<int64_t, bool> PIB;
typedef std::pair<double, PIB> PDP;
typedef std::pair<double, std::vector<int64_t> > PDVI;


typedef struct {
    int64_t ed_ind[2];
    int64_t v_pos[2];
} PARENT_PATH;

typedef struct Rule {
    double cost;
    std::vector<int64_t> precedencelist;
    Rule(double c, std::vector<int64_t> p) : cost(c), precedencelist(p) { }
}Rule;

typedef struct {
    double startCost, endCost;
} CostHolder;

typedef std::map<int64_t, std::vector<Rule> > RuleTable;



class GraphEdgeInfo {
 public:
    int64_t m_lEdgeID;
    int64_t m_lEdgeIndex;
    int m_sDirection;
    double m_dCost;
    double m_dReverseCost;
    LongVector m_vecStartConnectedEdge;
    LongVector m_vecEndConnedtedEdge;
    // LongVector m_vecConnectedNode;
    bool m_bIsLeadingRestrictedEdge;
    VectorOfLongVector m_vecRestrictedEdge;

    int64_t m_lStartNode;
    int64_t m_lEndNode;
};




typedef std::vector<GraphEdgeInfo*> GraphEdgeVector;
typedef std::map<int64_t, LongVector> Long2LongVectorMap;
typedef std::map<int64_t, int64_t> Long2LongMap;




class GraphDefinition {
 public:
    GraphDefinition(void);
    ~GraphDefinition(void);

    int my_dijkstra(int64_t start_vertex, int64_t end_vertex,
                    size_t edge_count, char** err_msg);

    int my_dijkstra(edge_t *edges, size_t edge_count,
                    int64_t start_vertex, int64_t end_vertex,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg);

    int my_dijkstra(edge_t *edges, size_t edge_count,
                    int64_t start_vertex, int64_t end_vertex,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    const std::vector<PDVI> &ruleList);

    int my_dijkstra(edge_t *edges, size_t edge_count,
                    int64_t start_edge, double start_part,
                    int64_t end_edge, double end_part,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    const std::vector<PDVI> &ruleList);

    int multi_dijkstra(edge_t *edges, size_t edge_count,
                       std::vector<int> vertices,
                       bool directed, bool has_reverse_cost,
                       path_element_tt **path, size_t *path_count,
                       char **err_msg,
                       const std::vector<PDVI> &ruleList);

    bool construct_graph(const edge_t *edges, size_t edge_count,
                         bool has_reverse_cost, bool directed);


 private:
    double construct_path(int64_t ed_id, int64_t v_pos);
    void explore(int64_t cur_node, const GraphEdgeInfo& cur_edge, bool isStart,
                 const LongVector &vecIndex, std::priority_queue<PDP,
                 std::vector<PDP>, std::greater<PDP> > &que);
    double getRestrictionCost(int64_t cur_node, const GraphEdgeInfo& new_edge,
                              bool isStart);
    bool addEdge(const edge_t edgeIn);
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
    int64_t max_node_id;
    int64_t max_edge_id;
    int64_t m_lStartEdgeId;
    int64_t m_lEndEdgeId;
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

#endif  // SRC_TRSP_SRC_GRAPHDEFINITION_H_
