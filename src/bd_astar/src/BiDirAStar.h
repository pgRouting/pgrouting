/*PGR-MIT*****************************************************************

* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/
#ifndef SRC_BD_ASTAR_SRC_BIDIRASTAR_H_
#define SRC_BD_ASTAR_SRC_BIDIRASTAR_H_
#pragma once

#include <vector>
#include <map>
#include <utility>

#include "./MinHeap.h"
#include "./bdastar_driver.h"

#define INF 1e15



typedef std::vector<size_t> LongVector;
typedef std::vector<LongVector> VectorOfLongVector;
typedef std::pair<double, int> PDI;

typedef struct {
    int par_Node;
    int par_Edge;
} PARENT_PATH;

typedef struct {
    int NodeID;
    double xpos;
    double ypos;
    std::vector<int> Connected_Nodes;
    std::vector<size_t> Connected_Edges_Index;
} GraphNodeInfo;

struct GraphEdgeInfo {
 public:
     int EdgeID;
     size_t EdgeIndex;
     int Direction;
     double Cost;
     double ReverseCost;
     int StartNode;
     int EndNode;
};

typedef std::vector<GraphEdgeInfo> GraphEdgeVector;
typedef std::map<size_t, LongVector> Long2LongVectorMap;
typedef std::map<size_t, size_t> Long2LongMap;
typedef std::vector<GraphNodeInfo> GraphNodeVector;


class BiDirAStar {
 public:
     BiDirAStar(void);
     ~BiDirAStar(void);

     int bidir_astar(edge_astar_t *edges, size_t edge_count, int maxNode, int start_vertex, int end_vertex,
             path_element_t **path, size_t *path_count, char **err_msg);

 private:
     bool construct_graph(edge_astar_t *edges, size_t edge_count, int maxNode);
     void fconstruct_path(int node_id);
     void rconstruct_path(int node_id);
     bool addEdge(edge_astar_t edgeIn);
     bool connectEdge(GraphEdgeInfo& firstEdge, GraphEdgeInfo& secondEdge, bool bIsStartNodeSame);
     void init();
     void initall(int maxNode);
     void deleteall();
     // void explore(int cur_node, double cur_cost, int dir, std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > &que);
     void explore(int cur_node, double cur_cost, int dir, MinHeap &que);
     double getcost(int node_id, int dir);
     void setcost(int node_id, int dir, double c);
     void setparent(int node_id, int dir, int parnode, int paredge);
     double gethcost(int node_id, int dir);
     double dist(double x1, double y1, double x2, double y2);

 private:
     GraphEdgeVector m_vecEdgeVector;
     Long2LongMap m_mapEdgeId2Index;
     Long2LongVectorMap m_mapNodeId2Edge;
     GraphNodeVector m_vecNodeVector;
     int max_node_id;
     int max_edge_id;
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

#endif  // SRC_BD_ASTAR_SRC_BIDIRASTAR_H_
