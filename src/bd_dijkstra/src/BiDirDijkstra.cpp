/*PGR-MIT*****************************************************************

* $Id$
*
* Project: pgRouting bdsp and bdastar algorithms
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

#if defined(__MINGW32__) ||  defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <queue>
#include <vector>
#include <functional>

#include "BiDirDijkstra.h"
#include "../../common/src/pgr_alloc.hpp"



BiDirDijkstra::BiDirDijkstra(void) {
}

BiDirDijkstra::~BiDirDijkstra(void) {
}

void BiDirDijkstra::init() {
    // max_edge_id = 0;
    // max_node_id = 0;
}

/*
    Initialization and allocation of memories.
*/
void BiDirDijkstra::initall(int maxNode) {
    int i;
    m_vecPath.clear();
    // DBG("BiDirDijkstra::initall: allocating m_pFParent, m_pRParent maxNode: %d\n", maxNode+1);
    m_pFParent = new PARENT_PATH[maxNode + 1];
    m_pRParent = new PARENT_PATH[maxNode + 1];
    // DBG("BiDirDijkstra::initall: allocated m_pFParent, m_pRParent\n");

    // DBG("BiDirDijkstra::initall: allocating m_pFCost, m_pRCost maxNode: %d\n", maxNode+1);
    m_pFCost = new double[maxNode + 1];
    m_pRCost = new double[maxNode + 1];
    // DBG("BiDirDijkstra::initall: allocated m_pFCost, m_pRCost\n");

    for (i = 0; i <= maxNode; i++) {
        m_pFParent[i].par_Node = -2;
        m_pRParent[i].par_Node = -2;
        m_pFCost[i] = INF;
        m_pRCost[i] = INF;
    }
    m_MinCost = INF;
    m_MidNode = -1;

    // DBG("BiDirDijkstra::initall: m_vecNodeVector.reserve(%d)\n", maxNode + 1);
    // reserve space for nodes
    m_vecNodeVector.reserve(maxNode + 1);
    // DBG("           space reserved!\n");
}

/*
    Delete the allocated memories to avoid memory leak.
*/
void BiDirDijkstra::deleteall() {
    std::vector<GraphNodeInfo*>::iterator itNode;
    for (itNode = m_vecNodeVector.begin(); itNode != m_vecNodeVector.end(); itNode++) {
        delete *itNode;
    }
    m_vecNodeVector.clear();
    std::vector<GraphEdgeInfo*>::iterator itEdge;
    for (itEdge = m_vecEdgeVector.begin(); itEdge != m_vecEdgeVector.end(); itEdge++) {
        delete *itEdge;
    }
    m_vecEdgeVector.clear();
    delete [] m_pFParent;
    delete [] m_pRParent;
    delete [] m_pFCost;
    delete [] m_pRCost;
}

/*
    Get the current cost from source to the current node if direction is 1 else the cost to reach target from the current node.
*/
double BiDirDijkstra::getcost(int node_id, int dir) {
    if (dir == 1) {
        return(m_pFCost[node_id]);
    } else {
        return(m_pRCost[node_id]);
    }
}
/*
    Set the forward or reverse cost list depending on dir (1 for forward search and -1 for reverse search.
*/
void BiDirDijkstra::setcost(int node_id, int dir, double c) {
    if (dir == 1) {
        m_pFCost[node_id] = c;
    } else {
        m_pRCost[node_id] = c;
    }
}

void BiDirDijkstra::setparent(int node_id, int dir, int parnode, int paredge) {
    if (dir == 1) {
        m_pFParent[node_id].par_Node = parnode;
        m_pFParent[node_id].par_Edge = paredge;
    } else {
        m_pRParent[node_id].par_Node = parnode;
        m_pRParent[node_id].par_Edge = paredge;
    }
}

/*
    Reconstruct path for forward search. It is like normal dijkstra. The parent array contains the parent of the current node and there is a -1 in the source.
    So one need to recurse upto the source and then add the current node and edge to the list.
*/
void BiDirDijkstra::fconstruct_path(int node_id) {
    if (m_pFParent[node_id].par_Node == -1)
        return;
    fconstruct_path(m_pFParent[node_id].par_Node);
    path_element_t pt;
    pt.vertex_id = m_pFParent[node_id].par_Node;
    pt.edge_id = m_pFParent[node_id].par_Edge;
    pt.cost = m_pFCost[node_id] - m_pFCost[m_pFParent[node_id].par_Node];
    m_vecPath.push_back(pt);
}

/*
    Reconstruct path for the reverse search. In this case the subsequent node is stored in the parent and the target contains a -1. So one need to add the node
    and edge to the list and then recurse through the parent upto hitting a -1.
*/

void BiDirDijkstra::rconstruct_path(int node_id) {
    path_element_t pt;
    if (m_pRParent[node_id].par_Node == -1) {
        pt.vertex_id = node_id;
        pt.edge_id = -1;
        pt.cost = 0.0;
        return;
    }
    pt.vertex_id = node_id;
    pt.cost = m_pRCost[node_id] - m_pRCost[m_pRParent[node_id].par_Node];
    pt.edge_id = m_pRParent[node_id].par_Edge;
    m_vecPath.push_back(pt);
    rconstruct_path(m_pRParent[node_id].par_Node);
}

/*
    This is the main exploration module. The parameter dir indicates whether the exploration will be in forward or reverser direction. The reference to the corresponding
    que is also passed as parameter que. The current node and the current costs are also available as parameter.
*/

void BiDirDijkstra::explore(int cur_node, double cur_cost, int dir, std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > &que) {
    int i;
    // Number of connected edges
    int con_edge = static_cast<int>(m_vecNodeVector[cur_node]->Connected_Edges_Index.size());
    double edge_cost;

    for (i = 0; i < con_edge; i++) {
        int edge_index = m_vecNodeVector[cur_node]->Connected_Edges_Index[i];
        // Get the edge from the edge list.
        GraphEdgeInfo edge = *m_vecEdgeVector[edge_index];
        // Get the connected node
        int new_node = m_vecNodeVector[cur_node]->Connected_Nodes[i];

        if (cur_node == edge.StartNode) {
            // Current node is the startnode of the edge. For forward search it should use forward cost, otherwise it should use the reverse cost,
            // i.e. if the reverse direction is valid then this node may be visited from the end node.
            if (dir > 0)
                edge_cost = edge.Cost;
            else
                edge_cost = edge.ReverseCost;

            // Check if the direction is valid for exploration
            if (edge.Direction == 0 ||  edge_cost >= 0.0) {
                // Check if the current edge gives better result
                if (cur_cost + edge_cost < getcost(new_node, dir)) {
                    // explore the node, and push it in the queue
                    setcost(new_node, dir, cur_cost + edge_cost);
                    setparent(new_node, dir, cur_node, edge.EdgeID);
                    que.push(std::make_pair(cur_cost + edge_cost, new_node));

                    // Update the minimum cost found so far.
                    if (getcost(new_node, dir) + getcost(new_node, dir * -1) < m_MinCost) {
                        m_MinCost = getcost(new_node, dir) + getcost(new_node, dir * -1);
                        m_MidNode = new_node;
                    }
                }
            }
        } else {
            // Current node is the endnode of the edge. For forward search it should use reverse cost, otherwise it should use the forward cost,
            // i.e. if the forward direction is valid then this node may be visited from the start node.
            if (dir > 0)
                edge_cost = edge.ReverseCost;
            else
                edge_cost = edge.Cost;

            // Check if the direction is valid for exploration
            if (edge.Direction == 0 ||  edge_cost >= 0.0) {
                // Check if the current edge gives better result
                if (cur_cost + edge_cost < getcost(new_node, dir)) {
                    setcost(new_node, dir, cur_cost + edge_cost);
                    setparent(new_node, dir, cur_node, edge.EdgeID);
                    que.push(std::make_pair(cur_cost + edge_cost, new_node));

                    // Update the minimum cost found so far.
                    if (getcost(new_node, dir) + getcost(new_node, dir * -1) < m_MinCost) {
                        m_MinCost = getcost(new_node, dir) + getcost(new_node, dir * -1);
                        m_MidNode = new_node;
                    }
                }
            }
        }
    }
}

/*
    This is the entry function that the wrappers should call. Most of the parameters are trivial. maxNode refers to Maximum
    node id. As we run node based exploration cost, parent etc will be based on maximam node id.
*/


int BiDirDijkstra::bidir_dijkstra(edge_t *edges, unsigned int edge_count, int maxNode, int start_vertex, int end_vertex,
                path_element_t **path, int *path_count, char **err_msg) {
    max_node_id = maxNode;
    max_edge_id = -1;

    // Allocate memory for local storage like cost and parent holder
    // DBG("calling initall(maxNode = %d)\n", maxNode);
    initall(maxNode);

    // construct the graph from the edge list, i.e. populate node and edge data structures
    // DBG("Calling construct_graph\n");
    construct_graph(edges, edge_count, maxNode);


    // int nodeCount = m_vecNodeVector.size();
    // DBG("Setting up std::priority_queue\n");
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > fque;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<PDI> > rque;

    // DBG("calling m_vecPath.clear()\n");
    m_vecPath.clear();

    // Initialize the forward search
    m_pFParent[start_vertex].par_Node = -1;
    m_pFParent[start_vertex].par_Edge = -1;
    m_pFCost[start_vertex] = 0.0;
    fque.push(std::make_pair(0.0, start_vertex));

    // Initialize the reverse search
    m_pRParent[end_vertex].par_Node = -1;
    m_pRParent[end_vertex].par_Edge = -1;
    m_pRCost[end_vertex] = 0.0;
    rque.push(std::make_pair(0.0, end_vertex));

    int i;
    // int new_node;
    int cur_node;
    // int dir;

/*
    The main loop. The algorithm is as follows:
    1. IF the sum of the current minimum of both heap is greater than so far found path, we cannot get any better, so break the loop.
    2. IF the reverse heap minimum is lower than the forward heap minimum, explore from reverse direction.
    3. ELSE explore from the forward directtion.
*/

    while (!fque.empty() &&  !rque.empty()) {
        PDI fTop = fque.top();
        PDI rTop = rque.top();
        if (fTop.first + rTop.first > m_MinCost)  // We are done, there is no path with lower cost
            break;

        if (rTop.first < fTop.first) {
            //  Explore from reverse queue
            cur_node = rTop.second;
            int dir = -1;
            rque.pop();
            explore(cur_node, rTop.first, dir, rque);
        } else {
            // Explore from forward queue
            cur_node = fTop.second;
            int dir = 1;
            fque.pop();
            explore(cur_node, fTop.first, dir, fque);
        }
    }

/*
    Path reconstruction part. m_MidNode is the joining point where two searches meet to make a shortest path. It is updated in explore.
    If it contains -1, then no path is found. Other wise we have a shortest path and that is reconstructed in the m_vecPath.
*/
    if (m_MidNode == -1) {
        *err_msg = (char *)"Path Not Found";
        deleteall();
        return -1;
    } else {
        // reconstruct path from forward search
        fconstruct_path(m_MidNode);
        // reconstruct path from backward search
        rconstruct_path(m_MidNode);

        // insert the last row in the path trace (having edge_id = -1 and cost = 0.0)
        path_element_t pelement;
        pelement.vertex_id = end_vertex;
        pelement.edge_id = -1;
        pelement.cost = 0.0;
        m_vecPath.push_back(pelement);

        // Transfer data path to path_element_t format and allocate memory and populate the pointer

        // DBG("BiDirDijkstra::bidir_dijkstra: allocating path m_vecPath.size = %d\n", m_vecPath.size() + 1);
        // *path = (path_element_t *) malloc(sizeof(path_element_t) * (m_vecPath.size() + 1));
        *path = NULL;
        *path = pgr_alloc(m_vecPath.size(), (*path));
        *path_count = static_cast<int>(m_vecPath.size());
        // DBG("BiDirDijkstra::bidir_dijkstra: allocated path\n");

        for (i = 0; i < *path_count; i++) {
            (*path)[i].vertex_id = m_vecPath[i].vertex_id;
            (*path)[i].edge_id = m_vecPath[i].edge_id;
            (*path)[i].cost = m_vecPath[i].cost;
        }
    }
    // DBG("calling deleteall\n");
    deleteall();
    // DBG("back from deleteall\n");
    return 0;
}

/*
    Populate the member variables of the class using the edge list. Basically there is a node list and an edge list. Each node contains the list of adjacent nodes and
    corresponding edge indices from edge list that connect this node with the adjacent nodes.
*/

bool BiDirDijkstra::construct_graph(edge_t* edges, int edge_count, int maxNode) {
    int i;

    /*
    // Create a dummy node
    DBG("Create a dummy node\n");
    GraphNodeInfo nodeInfo;
    DBG("calling nodeInfo.Connected_Edges_Index.clear\n");
    nodeInfo.Connected_Edges_Index.clear();
    DBG("calling nodeInfo.Connected_Nodes.clear\n");
    nodeInfo.Connected_Nodes.clear();
    */

    // Insert the dummy node into the node list. This acts as place holder. Also change the nodeId so that nodeId and node index in the vector are same.
    // There may be some nodes here that does not appear in the edge list. The size of the list is upto maxNode which is equal to maximum node id.
    // DBG("m_vecNodeVector.push_back for 0 - %d\n", maxNode);
    for (i = 0; i <= maxNode; i++) {
        // Create a dummy node
        GraphNodeInfo* nodeInfo = new GraphNodeInfo();
        nodeInfo->Connected_Edges_Index.clear();
        nodeInfo->Connected_Nodes.clear();

        nodeInfo->NodeID = i;
        m_vecNodeVector.push_back(nodeInfo);
    }

    // Process each edge from the edge list and update the member data structures accordingly.
    // DBG("reserving space for m_vecEdgeVector.reserve(%d)\n", edge_count);
    m_vecEdgeVector.reserve(edge_count);
    // DBG("calling addEdge in a loop\n");
    for (i = 0; i < edge_count; i++) {
        addEdge(edges[i]);
    }

    return true;
}

/*
    Process the edge and populate the member nodelist and edgelist. The nodelist already contains upto maxNode dummy entries with nodeId same as index. Now the
    connectivity information needs to be updated.
*/

bool BiDirDijkstra::addEdge(edge_t edgeIn) {
    // long lTest;

    // Check if the edge is already processed.
    Long2LongMap::iterator itMap = m_mapEdgeId2Index.find(edgeIn.id);
    if (itMap != m_mapEdgeId2Index.end())
        return false;


    // Create a GraphEdgeInfo using the information of the current edge
    GraphEdgeInfo *newEdge = new GraphEdgeInfo();
    newEdge->EdgeID = static_cast<int>(edgeIn.id);
    newEdge->EdgeIndex = static_cast<int>(m_vecEdgeVector.size());
    newEdge->StartNode = static_cast<int>(edgeIn.source);
    newEdge->EndNode = static_cast<int>(edgeIn.target);
    newEdge->Cost = edgeIn.cost;
    newEdge->ReverseCost = edgeIn.reverse_cost;

    // Set the direction. If both cost and reverse cost has positive value the edge is bidirectional and direction field is 0. If cost is positive and reverse cost
    // negative then the edge is unidirectional with direction = 1 (goes from source to target) otherwise it is unidirectional with direction = -1 (goes from target
    // to source). Another way of creating unidirectional edge is assigning big values in cost or reverse_cost. In that case the direction is still zero and this case
    // is handled in the algorithm automatically.
    if (newEdge->Cost >= 0.0 &&  newEdge->ReverseCost >= 0) {
        newEdge->Direction = 0;
    } else if (newEdge->Cost >= 0.0) {
        newEdge->Direction = 1;
    } else {
        newEdge->Direction = -1;
    }

    // Update max_edge_id
    if (edgeIn.id > max_edge_id) {
        max_edge_id = static_cast<int>(edgeIn.id);
    }

    // Update max_node_id
    if (newEdge->StartNode > max_node_id) {
        return false;  // max_node_id = newEdge.StartNode;
    }
    if (newEdge->EndNode > max_node_id) {
        return false;  // max_node_id = newEdge.EdgeIndex;
    }

    // update connectivity information for the start node.
    m_vecNodeVector[newEdge->StartNode]->Connected_Nodes.push_back(newEdge->EndNode);
    m_vecNodeVector[newEdge->StartNode]->Connected_Edges_Index.push_back(newEdge->EdgeIndex);

    // update connectivity information for the start node.
    m_vecNodeVector[newEdge->EndNode]->Connected_Nodes.push_back(newEdge->StartNode);
    m_vecNodeVector[newEdge->EndNode]->Connected_Edges_Index.push_back(newEdge->EdgeIndex);



    // Adding edge to the list
    m_mapEdgeId2Index.insert(std::make_pair(newEdge->EdgeID, m_vecEdgeVector.size()));
    m_vecEdgeVector.push_back(newEdge);

    return true;
}
