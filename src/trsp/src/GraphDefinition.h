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
#ifndef GRAPHDEFINITION_H
#define GRAPHDEFINITION_H

#include <vector>
#include <map>
#include <queue>
#if defined(_MSC_VER) && _MSC_VER >= 1700
#include <functional>
#endif
#include <sstream>
#include "trsp_driver.h"


typedef std::pair<double, std::vector<int64_t> > PDVI;


class GraphDefinition {
    typedef std::vector<int64_t> LongVector;
    typedef std::vector<LongVector> VectorOfLongVector;
    typedef std::pair<int, bool> PIB;
    typedef std::pair<double, PIB> PDP;
    typedef struct{
        int64_t ed_ind[2];
        int64_t v_pos[2];
    } PARENT_PATH;

    typedef struct{
        double cost;
        std::vector<int64_t> precedencelist;
    } Rule;

    typedef struct{
        double startCost, endCost;
    } CostHolder;

    typedef std::map<int64_t, std::vector<Rule> > RuleTable;



    class GraphEdgeInfo {
     public:
         GraphEdgeInfo() = default;
         GraphEdgeInfo(int64_t eid, int64_t index, int64_t node1, int64_t node2, double cost, double reverse_cost) :
             m_lEdgeID(eid), m_lEdgeIndex(index), m_lStartNode(node1), m_lEndNode(node2),
             m_dCost(cost), m_dReverseCost(reverse_cost) {
                 m_vecStartConnectedEdge.clear();
                 m_vecEndConnedtedEdge.clear();
                 m_vecRestrictedEdge.clear();
             }


     public:
         int64_t m_lEdgeID;
         int64_t m_lEdgeIndex;
         int64_t m_lStartNode;
         int64_t m_lEndNode;
         double m_dCost;
         double m_dReverseCost;
         short m_sDirection;
         LongVector m_vecStartConnectedEdge;
         LongVector m_vecEndConnedtedEdge;
         //LongVector m_vecConnectedNode;
         bool m_bIsLeadingRestrictedEdge;
         VectorOfLongVector m_vecRestrictedEdge;

    };


    typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
    typedef std::map<int64_t,LongVector> Long2LongVectorMap;
    typedef std::map<int64_t,int64_t> Long2LongMap;




 public:
    ~GraphDefinition(void);
    GraphDefinition(
            edge_t *edges,
            unsigned int edge_count,
            bool directed, 
            bool has_rcost);


    int my_dijkstra(
            int64_t start_vertex, int64_t end_vertex,
            path_element_t **path, size_t *path_count,
            std::ostringstream &log);

    void set_restrictions(
            int64_t start_vertex,
            int64_t end_vertex,
            std::vector<PDVI> &ruleList);

    void add_virtual_vertices(
            int start_edge, double start_part,
            int end_edge, double end_part,
            int64_t &start_vertex, int64_t &end_vertex);


    bool construct_graph(
            edge_t *edges,
            bool has_rcost,
            bool directed);


 private:
    double construct_path(int64_t ed_id, int64_t v_pos);
    void explore(int64_t cur_node, GraphEdgeInfo& cur_edge, bool isStart, LongVector &vecIndex, std::priority_queue<PDP, std::vector<PDP>, std::greater<PDP> > &que);
    double getRestrictionCost(int64_t cur_node, GraphEdgeInfo &new_edge, bool isStart);
    bool addEdge(edge_t edgeIn);
    bool connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo &secondEdge, bool bIsStartNodeSame);
    bool get_single_cost(double total_cost, path_element_t **path, size_t *path_count);
    void init();
    void deleteall();

 private:
    GraphEdgeVector m_vecEdgeVector;
    Long2LongMap m_mapEdgeId2Index;
    Long2LongVectorMap m_mapNodeId2Edge;
    int64_t max_node_id;
    int64_t max_edge_id;
    int m_lStartEdgeId;
    int m_lEndEdgeId;
    double m_dStartpart;
    double m_dEndPart;
    bool isStartVirtual;
    bool isEndVirtual;

    unsigned int m_edge_count;

    std::vector <path_element_t> m_vecPath;
    std::vector < PARENT_PATH > parent;
    std::vector < CostHolder > m_dCost;
    RuleTable m_ruleTable;
    bool m_bIsturnRestrictOn;
    bool m_bIsGraphConstructed;
};

#endif
