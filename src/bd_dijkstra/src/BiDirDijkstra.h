/*PGR-MIT*****************************************************************

* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
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

********************************************************************PGR-MIT*/

#ifndef BIDIRDIJKSTRA_H
#define BIDIRDIJKSTRA_H

#include <vector>
#include <map>
#include <queue>
#include <utility>
#include <functional>

#include "../../common/src/pgr_types.h"
#include "bdsp.h"

#define INF 1e15



typedef std::vector<long> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<double, int> PDI;

typedef struct{
    int par_Node;
    int64_t par_Edge;
}PARENT_PATH;

typedef struct{
    int NodeID;
    std::vector<int64_t> Connected_Nodes;
    std::vector<int> Connected_Edges_Index;
}GraphNodeInfo;

struct GraphEdgeInfo
{
public:
    int64_t EdgeID;
    int EdgeIndex;
    int Direction;
    double Cost;
    double ReverseCost;
    int64_t StartNode;
    int64_t EndNode;
};

typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
typedef std::map<long,LongVector> Long2LongVectorMap;
typedef std::map<long,long> Long2LongMap;
typedef std::vector<GraphNodeInfo*> GraphNodeVector;


class BiDirDijkstra
{
public:
    BiDirDijkstra(void);
    ~BiDirDijkstra(void);
    
    int bidir_dijkstra(edge_t *edges, size_t edge_count, int64_t maxNode, int64_t start_vertex, int64_t end_vertex,
        path_element_t **path, size_t *path_count, char **err_msg);
    

private:
    bool construct_graph(edge_t *edges, size_t edge_count, int64_t maxNode);
    void fconstruct_path(int node_id);
    void rconstruct_path(int node_id);
    bool addEdge(edge_t edgeIn);
    bool connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo& secondEdge, bool bIsStartNodeSame);
    void init();
    void initall(int64_t maxNode);
    void deleteall();
    void explore(int cur_node, double cur_cost, int dir, std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > &que);
    double getcost(int node_id, int dir);
    void setcost(int node_id, int dir, double c);
    void setparent(int node_id, int dir, int parnode, int64_t paredge);

private:
    GraphEdgeVector m_vecEdgeVector;
    Long2LongMap m_mapEdgeId2Index;
    Long2LongVectorMap m_mapNodeId2Edge;
    GraphNodeVector m_vecNodeVector;
    int64_t max_node_id;
    int64_t max_edge_id;
    int m_lStartNodeId;
    int m_lEndNodeId;

    double m_MinCost;
    int m_MidNode;
    std::vector <path_element_t> m_vecPath;
    PARENT_PATH *m_pFParent;
    PARENT_PATH *m_pRParent;
    double *m_pFCost;
    double *m_pRCost;
};

#endif
