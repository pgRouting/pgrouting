/*PGR-GNU*****************************************************************

FILE: pgr_trspHandler.h

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
#include <limits>


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

class CostHolder {
 public:
    CostHolder() {
       endCost =  startCost = (std::numeric_limits<double>::max)();
    }

 public:
    double startCost;
    double endCost;

};

typedef std::map<int64_t, std::vector<Rule> > RuleTable;



class GraphEdgeInfo {
 public:
     GraphEdgeInfo(
             edge_t edgeIn,
             size_t edgeIndex) :
         m_lEdgeID(edgeIn.id),
         m_lEdgeIndex(edgeIndex),
         m_dCost(edgeIn.cost),
         m_dReverseCost(edgeIn.reverse_cost),

         m_vecStartConnectedEdge(),
         m_vecEndConnedtedEdge(),
         m_vecRestrictedEdge(),

         m_lStartNode(edgeIn.source),
         m_lEndNode(edgeIn.target) {}


 public:
    int64_t m_lEdgeID;
    size_t m_lEdgeIndex;
    int m_sDirection;
    double m_dCost;
    double m_dReverseCost;

    LongVector m_vecStartConnectedEdge;
    LongVector m_vecEndConnedtedEdge;
    VectorOfLongVector m_vecRestrictedEdge;

    bool m_bIsLeadingRestrictedEdge;

    int64_t m_lStartNode;
    int64_t m_lEndNode;
};




typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
typedef std::map<int64_t, LongVector> Long2LongVectorMap;
typedef std::map<int64_t, int64_t> Long2LongMap;




class Pgr_trspHandler {
 public:
    Pgr_trspHandler(
            edge_t *edges,
            const size_t edge_count,
            const int64_t start_vertex,
            const int64_t end_vertex,
            const bool directed,
            const bool has_reverse_cost,
            const std::vector<PDVI> &ruleList);

    Pgr_trspHandler(void);
    ~Pgr_trspHandler(void) = default;

    int my_dijkstra1(int64_t start_vertex, int64_t end_vertex,
                    size_t edge_count, char** err_msg);


    int initializeAndProcess(
            edge_t *edges,
            size_t edge_count,

            const std::vector<PDVI> &ruleList,

            const int64_t start_vertex,
            const int64_t end_vertex,

            bool directed,
            bool has_reverse_cost,

            path_element_tt **path,
            size_t *path_count,
            char **err_msg);


    int my_dijkstra4(edge_t *edges, size_t edge_count,
                    int64_t start_edge, double start_part,
                    int64_t end_edge, double end_part,
                    bool directed, bool has_reverse_cost,
                    path_element_tt **path, size_t *path_count,
                    char **err_msg,
                    const std::vector<PDVI> &ruleList);

    bool construct_graph(edge_t *edges, const size_t edge_count,
                         const bool has_reverse_cost, const bool directed);

    void clear();

 private:
    int initialize_restrictions(
                    const std::vector<PDVI> &ruleList);

    void initialize_que();

    int process_trsp(size_t edge_count,
                    path_element_tt **path,
                    size_t *path_count,
                    char **err_msg);

    GraphEdgeInfo* dijkstra_exploration(
            GraphEdgeInfo* cur_edge,
            int64_t &cur_node);



    void explore(int64_t cur_node, const GraphEdgeInfo cur_edge, bool isStart,
                 const LongVector &vecIndex);

    double getRestrictionCost(int64_t cur_node, const GraphEdgeInfo &new_edge,
                              bool isStart);
    bool addEdge(const edge_t edgeIn);
    bool connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo& secondEdge,
                     bool bIsStartNodeSame);
    bool get_single_cost(double total_cost, path_element_tt **path,
                         size_t *path_count);
    double construct_path(int64_t ed_id, int64_t v_pos);
    void init();

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
    int64_t m_start_vertex;
    int64_t m_end_vertex;

    std::vector <path_element_tt> m_vecPath;
    std::vector<PARENT_PATH> parent;
    std::vector<CostHolder> m_dCost;
    RuleTable m_ruleTable;
    bool m_bIsturnRestrictOn;
    bool m_bIsGraphConstructed;
    std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > que;
};

#endif  // SRC_TRSP_SRC_GRAPHDEFINITION_H_
