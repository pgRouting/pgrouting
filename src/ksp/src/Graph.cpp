///////////////////////////////////////////////////////////////////////////////
///  Graph.cpp
///  <TODO: insert file description here>
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 8/18/2010
///
///  $Id: Graph.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $
///////////////////////////////////////////////////////////////////////////////

#include <limits>
// #include <set>
// #include <map>
#include <string>
// #include <cassert>
// #include "ksp.h"
#include <deque>
#include <fstream>
#include <iostream>
 #include <algorithm>
#include "GraphElements.h"
#include "Graph.h"


const double Graph::DISCONNECT = (std::numeric_limits<double>::max)();

Graph::Graph(const std::string &file_name) {
        _import_from_file(file_name);
}

Graph::Graph(const Graph &graph) {
        m_vtVertices.assign(graph.m_vtVertices.begin(), graph.m_vtVertices.end());
        m_vtEdges.assign(graph.m_vtEdges.begin(), graph.m_vtEdges.end());
}

Graph::Graph(void) {}
Graph::~Graph(void) { clear();}


POS Graph::getNewVertex(int node_id) {
    POS nodePos;
    if (!exist_vertex(node_id)) {
        nodePos = m_vtVertices.size();
        BaseVertex vertex(nodePos, node_id);
        m_vtVertices.push_back(vertex);
        return nodePos;
    }
    nodePos = find_vertex(node_id);
    return nodePos;
}


void Graph::insertNewEdge(int id,  POS startPos, POS endPos, double edge_weight) {
    // all edges are different regardless of sharing same start & end nodes
    POS edgePos = m_vtEdges.size();
    BaseEdge edge(edgePos, id, startPos, endPos, edge_weight);
    m_vtEdges.push_back(edge);

    m_vtVertices[startPos].push_FanOut(edgePos);
    m_vtVertices[endPos].push_FanIn(edgePos);
}



BaseVertex Graph::GetVertex(int node_id) {
        BaseVertex vertex_pt = m_vtVertices[node_id];
        return vertex_pt;
}


BaseVertex* Graph::get_vertex(int node_id) {
        BaseVertex * vertex_pt = &m_vtVertices[node_id];
        if (vertex_pt->isRemoved())
                return NULL;
        return vertex_pt;
}

void Graph::clear() {
        m_vtEdges.clear();
        for (POS pos=0; pos < m_vtVertices.size(); pos++)
             m_vtVertices[pos].clear();
        m_vtVertices.clear();
}

POS  Graph::find_vertex(int vertex_id) const {
    POS i;
    if (m_vtVertices.size() == 0) return 0;
    for (i = 0; i < m_vtVertices.size()
          && m_vtVertices[i].getOriginalID() != vertex_id; i++) {}
    if (i >= m_vtVertices.size()) return 0;
    return i;
}

POS  Graph::exist_vertex(int vertex_id) const {
    POS i;
    if (m_vtVertices.size() == 0) return false;
    for (i=0; i < m_vtVertices.size() && m_vtVertices[i].getOriginalID() != vertex_id; i++) {}
    if (i >= m_vtVertices.size()) return false;
    return true;
}
bool  Graph::checkVertexIDs(POS nodeNID, int nodeID) {
    if (!(nodeNID < m_vtVertices.size())) return false;
    if (m_vtVertices[nodeNID].getOriginalID() != nodeID) return false;
    return true;
}

void  Graph::remove_edge(POS edge_id) {
    m_vtEdges[edge_id].remove();
}

void  Graph::removeNodes(const BasePath &path) {
    if (path.size() == 0) return;
    for (POS i = 0 ; i  <  path.size() ; i++) {
    m_vtVertices[ path[i].getStart() ].remove();
    }
}



void Graph::get_adjacent_edges(POS vertex_id, std::deque<POS> &edges_set) const {
        POS edgeId, nextNodeId;

        if (m_vtVertices[vertex_id].isActive()) {
           std::deque<POS> FanOut = m_vtVertices[vertex_id].getFanOut();
           for (POS i = 0; i < FanOut.size(); i++) {
               edgeId = FanOut[i];
               nextNodeId =  m_vtEdges[ edgeId ].getEnd();
               if (m_vtVertices[ nextNodeId ].isActive()
                   && m_vtEdges[ edgeId ].isActive())
                       edges_set.push_back(edgeId);
           }
        }
}

void Graph::get_precedent_edges(POS vertex_id, std::deque<BaseEdge> &edges_set) {
        if (m_vtVertices[vertex_id].isActive()) {
           std::deque<POS> FanIn = m_vtVertices[vertex_id].getFanIn();
           for (POS i = 0; i < FanIn.size(); i++) {
               if (m_vtVertices[  m_vtEdges[ FanIn[i] ].getStart() ].isActive()
                   &&  m_vtEdges[ FanIn[i] ].isActive())
                       edges_set.push_back(m_vtEdges[ FanIn[i] ]);
           }
        }
}


void Graph::PrintOut(std::ostream &out_stream) const {
    out_stream << "Vertices \n";
    for (POS i = 0; i< m_vtVertices.size(); i++) {
        m_vtVertices[i].PrintOut(out_stream);
        out_stream << "\n";
    }
    out_stream << "Edges\n";
    for (POS i = 0; i < m_vtEdges.size(); i++) {
        m_vtEdges[i].PrintOut(out_stream);
        out_stream << "\n";
    }
}



void Graph::PrintPath(std::ostream &out_stream, const BasePath &path, int startNode) const {
    if (path.size() == 0) {
        out_stream << "Route 0\n";
        out_stream << startNode << "\t-1 \t0\n";
        return;
    }

    for (POS i=0 ; i < path.size() ; i++) {
        BaseEdge edge = path[i];
        int start = m_vtVertices[ edge.getStart() ].getOriginalID();
        double cost  = edge.Weight();
        int edgeId = edge.originalID();
        out_stream << "Route " << i<< "\n";
        out_stream << start << "\t" << edgeId << "\t" << cost << "\n";
    }
    out_stream<< m_vtVertices[ path[path.size()-1 ].getEnd() ].getOriginalID() << "\t-1 \t0\n";
}

///////////////////////////////////////////////////////////////////////////////
///  private  _import_from_file
///  Construct the graph by importing the edges from the input file.
///
///  @param [in]       file_name const std::string &    The input graph file
///
///  This function doesn't return a value
///
///  @remarks The format of the file is as follows:
///   1. The first line has an integer as the number of vertices of the graph
///   2. Each line afterwards contains a directed edge in the graph:
///                     starting point, ending point and the weight of the edge.
///                 These values are separated by 'white space'.
///
///  @see <TODO: insert text here>
///
///  @author Yan Qi @date 5/29/2010
///////////////////////////////////////////////////////////////////////////////
void Graph::_import_from_file(const std::string &input_file_name) {
        const char* file_name = input_file_name.c_str();

        // 1. Check the validity of the file
        std::ifstream ifs(file_name);
        if (!ifs) {
                std::cerr << "The file " << file_name << " can not be opened!" << std::endl;
                exit(1);
        }

        // 2. Reset the members of the class
        clear();

        // 3. Start to read information from the input file.
        // 3.1 The first line indicates if it has a reverse cost
        bool has_reverse_cost;
        ifs >> has_reverse_cost;

        // 3.2 In the following lines, each line contains a directed edge in the graph:
        ///   id of the edge, the id of starting point, the id of ending point, the weight of the edge.
        ///   These values are separated by 'white space'.
        int edge_id, start_id, end_id;
        double edge_weight, reverse_weight;
        POS startPos;
        POS endPos;

        while (ifs >> edge_id) {
                if (edge_id == -1)  break;

                ifs >> start_id;
                ifs >> end_id;
                ifs >> edge_weight;
                if (has_reverse_cost) ifs >> reverse_weight;

                /// 3.2.1 construct the vertices
                  /// look for existing vertex or create
                startPos = Graph::getNewVertex(start_id);
                endPos = Graph::getNewVertex(end_id);

                /// 3.2.2 add the edge
                if (edge_weight >0)
                       insertNewEdge(edge_id, startPos, endPos, edge_weight);
                if (has_reverse_cost && reverse_weight >0)
                       insertNewEdge(edge_id, endPos, startPos, reverse_weight);
        }

        ifs.close();
}
