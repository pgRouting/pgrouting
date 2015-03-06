#include <limits>
#include <string>
#include <deque>
#include <fstream>
#include <iostream>
 #include <algorithm>
#include "GraphElements.h"
#include "Graph.h"


const double Graph::DISCONNECT = (std::numeric_limits<double>::max)();

Graph::Graph(const std::string &file_name) {
        import_from_file(file_name);
}

Graph::Graph(const Graph &graph) {
        m_Vertices.assign(graph.m_Vertices.begin(), graph.m_Vertices.end());
        m_Edges.assign(graph.m_Edges.begin(), graph.m_Edges.end());
}

Graph::Graph(void) {}
Graph::~Graph(void) { clear();}


/*!
The original Id of the vertex is going stored at position ID in the vertices's deque. \n
In other words, the position in the deque is going to identify the vertex in the graph and will be used as
the ID of the vertex.\n

1. when vertex_id already exist in the graph
  - will return the exising ID of the vertex
2. otherwise
  - a new vertex is created and the new ID will be returned

\param[in] vertex_id: Original Id of the vertex
\return ID: of the vertex in the graph.
*/
UINT Graph::getNewVertex(int vertex_id) {
    UINT nodePos;
    if (!exist_vertex(vertex_id)) {
        nodePos = m_Vertices.size();
        BaseVertex vertex(nodePos, vertex_id);
        m_Vertices.push_back(vertex);
        return nodePos;
    }
    nodePos = find_vertex(vertex_id);
    return nodePos;
}


/*!
All edges are different regardless of sharing same start & end vertices.\n
The original Id of the edge is going stored at position ID in the Edge's deque.\n
In other words, the position in the deque is going to identify the edge in the graph and will be used as
the ID of the edge.\n
The FanIn of the ending vertex and FanOut of the starting vertex will include the edge

\param[in] edge_id: Original Id of the vertex
\param[in] startId: ID of the starting vertex
\param[in] endId: ID of the ending vertex
\param[in] edge_weight: weight of the edge
\return ID: of the edge in the graph.
*/
UINT Graph::insertNewEdge(int edge_id,  UINT startId, UINT endId, double edge_weight) {
    UINT edgePos = m_Edges.size();
    BaseEdge edge(edgePos, edge_id, startId, endId, edge_weight);
    m_Edges.push_back(edge);

    m_Vertices[startId].push_FanOut( &m_Edges[edgePos]);
    m_Vertices[endId].push_FanIn(&m_Edges[edgePos]);
    updateBestEdgesSet(&m_Edges[edgePos]);
    return edgePos;
}


void Graph::updateBestEdgesSet(BaseEdge *edgePt) {
    std::set <BaseEdge*, BaseEdge::compBestEdge>::iterator edgeInSet;
    edgeInSet = m_BestEdgesPt.find(edgePt);
    if (edgeInSet == m_BestEdgesPt.end()) {
        m_BestEdgesPt.insert(edgePt);
    } else {
        if ((*edgeInSet)->Weight() < edgePt->Weight()) {
           // the set has the best edge pointer
        } else {
           // swap to the best edge
           m_BestEdgesPt.erase(edgeInSet);
           m_BestEdgesPt.insert(edgePt);
        }
    }
}

double Graph::vertexWeight(UINT vertex_id) const { 
    assert (vertex_id < m_Vertices.size());
    return m_Vertices[vertex_id].Weight();
}
double Graph::edgeWeight(UINT edge_id) const {
    assert (edge_id < m_Edges.size());
    return m_Edges[edge_id].Weight();
}
int Graph::getVertexOriginalID(UINT vertex_id) const {
    assert (vertex_id < m_Vertices.size());
    return m_Vertices[vertex_id].getOriginalID();
}



BaseVertex Graph::getVertex(UINT vertex_id) const {
        assert (vertex_id < m_Vertices.size());
        return m_Vertices[vertex_id];
}



BaseVertex* Graph::getVertexPt(UINT vertex_id) {
        if ( vertex_id >= m_Vertices.size() ) return NULL;
        BaseVertex * vertex_pt = &m_Vertices[vertex_id];
        if (vertex_pt->isRemoved()) return NULL;
        return vertex_pt;
}


void Graph::clear() {
        m_Edges.clear();
        for (UINT pos=0; pos < m_Vertices.size(); pos++)
             m_Vertices[pos].clear();
        m_Vertices.clear();
}
/*!
\warning  Please use Graph::exist_vertex before calling this function otherwise:
  - when 0 is returned:
    - it might indicate not found
    - it might indicate the ID 
\param[in] vertex_id: Original Id of the vertex
\Return 0 if vertex_id is not found
\Return ID: the position/ID of vertex_id when not found
*/
UINT  Graph::find_vertex(int vertex_id) const {
    UINT i;
    if (m_Vertices.size() == 0) return 0;
    for (i = 0; i < m_Vertices.size()
          && m_Vertices[i].getOriginalID() != vertex_id; i++) {}
    if (i >= m_Vertices.size()) return 0;
    return i;
}

bool  Graph::exist_vertex(int vertex_id) const {
    UINT i;
    if (m_Vertices.size() == 0) return false;
    for (i=0; i < m_Vertices.size() && m_Vertices[i].getOriginalID() != vertex_id; i++) {}
    if (i >= m_Vertices.size()) return false;
    return true;
}

void  Graph::remove_edge(UINT edge_id) {
    assert(edge_id < m_Edges.size());
    m_Edges[edge_id].remove();
}

void  Graph::removeVertices(const BasePath &path) {
    if (path.size() == 0) return;
    for (UINT i = 0 ; i  <  path.size() ; i++) {
      m_Vertices[ path[i].getStart() ].remove();
    }
}

void Graph::getFanOutActiveEdgesIds(UINT vertex_id, std::deque<UINT> &edges_set) const {
        assert(vertex_id < m_Vertices.size());
        UINT edgeId, nextNodeId;
        edges_set.clear();
        if (!m_Vertices[vertex_id].isActive()) return;
                
        std::deque<BaseEdge*> FanOut = m_Vertices[vertex_id].getFanOut();
        for (UINT i = 0; i < FanOut.size(); i++) {
               edgeId = FanOut[i]->ID();
               nextNodeId =  FanOut[i]->getEnd();
               if (m_Vertices[ nextNodeId ].isActive()
                   && !m_Vertices[ nextNodeId ].visited()
                   && m_Edges[ edgeId ].isActive())
                       edges_set.push_back(edgeId);
        }
}


void Graph::getFanOutActiveEdges(UINT vertex_id, std::deque<BaseEdge> &edges_set) const {
        assert(vertex_id < m_Vertices.size());
        UINT edgeId, prevNodeId;
        edges_set.clear();
        if (!m_Vertices[vertex_id].isActive()) return;

        std::deque<BaseEdge*> FanOut = m_Vertices[vertex_id].getFanOut();
        for (UINT i = 0; i < FanOut.size(); i++) {
               edgeId = FanOut[i]->ID();
               prevNodeId =  FanOut[i]->getStart();
               if ( m_Vertices[ prevNodeId ].isActive()
                   &&  m_Edges[ edgeId ].isActive())
                       edges_set.push_back(m_Edges[edgeId]);
        }
}

void Graph::getFanInActiveEdges(UINT vertex_id, std::deque<BaseEdge> &edges_set) const{
        assert(vertex_id < m_Vertices.size());
        UINT edgeId, prevNodeId;
        edges_set.clear();
        if (!m_Vertices[vertex_id].isActive()) return;

        std::deque<BaseEdge*> FanIn = m_Vertices[vertex_id].getFanIn();
        for (UINT i = 0; i < FanIn.size(); i++) {
               edgeId = FanIn[i]->ID();
               prevNodeId =  FanIn[i]->getStart();
               if ( m_Vertices[ prevNodeId ].isActive()
                   &&  m_Edges[ edgeId ].isActive())
                       edges_set.push_back(m_Edges[edgeId]);
        }
}

void Graph::PrintOut(std::ostream &out_stream) const {
    out_stream << "Vertices \n";
    for (UINT i = 0; i< m_Vertices.size(); i++) {
        m_Vertices[i].PrintOut(out_stream);
        out_stream << "\n";
    }
    out_stream << "Edges\n";
    for (UINT i = 0; i < m_Edges.size(); i++) {
        m_Edges[i].PrintOut(out_stream);
        out_stream << "\n";
    }
}



void Graph::PrintPath(std::ostream &out_stream, const BasePath &path, int startNode) const {
    if (path.size() == 0) {
        out_stream << "Route 0\n";
        out_stream << startNode << "\t-1 \t0\n";
        return;
    }

    for (UINT i=0 ; i < path.size() ; i++) {
        BaseEdge edge = path[i];
        int start = m_Vertices[ edge.getStart() ].getOriginalID();
        double cost  = edge.Weight();
        int edgeId = edge.originalID();
        out_stream << "Route " << i<< "\n";
        out_stream << start << "\t" << edgeId << "\t" << cost << "\n";
    }
    out_stream<< m_Vertices[ path[path.size()-1 ].getEnd() ].getOriginalID() << "\t-1 \t0\n";
}

/*!
To be used when developing with a standby program
1. Check the validity of the file
2. Cleans the graph
3. Start to read information from the input file.
  - 3.1. The first line indicates if it has a reverse cost (0 = no)
  - 3.2. In the following lines, each line contains a directed edge in the graph:
      - id of the edge, 
      - id of starting point,
      - id of ending point,
      - weight of the edge.
      - Reverse Weight of the edge (when reverse cost is !=0)
  - 3.3 These values are separated by 'white space'.
4. Stops when the id of the edge is -1 and closes the file
\param[in] file_name
*/
void Graph::import_from_file(const std::string &input_file_name) {
        const char* file_name = input_file_name.c_str();

        std::ifstream ifs(file_name);
        if (!ifs) {
                std::cerr << "The file " << file_name << " can not be opened!" << std::endl;
                exit(1);
        }

        clear();

        bool has_reverse_cost;
        ifs >> has_reverse_cost;

        int edge_id, start_id, end_id;
        double edge_weight, reverse_weight;
        UINT startPos;
        UINT endPos;

        while (ifs >> edge_id) {
                if (edge_id == -1)  break;

                ifs >> start_id;
                ifs >> end_id;
                ifs >> edge_weight;
                if (has_reverse_cost) ifs >> reverse_weight;

                startPos = Graph::getNewVertex(start_id);
                endPos = Graph::getNewVertex(end_id);

                if (edge_weight >0)
                       insertNewEdge(edge_id, startPos, endPos, edge_weight);
                if (has_reverse_cost && reverse_weight >0)
                       insertNewEdge(edge_id, endPos, startPos, reverse_weight);
        }
        ifs.close();

}
