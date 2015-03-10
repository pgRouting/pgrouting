#include <limits>
#include <string>
#include <deque>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "vrp_assert.h"
#include "GraphElements.h"
#include "Graph.h"


const double Graph::DISCONNECT = (std::numeric_limits<double>::max)();

Graph::Graph(const std::string &file_name) {
        import_from_file(file_name);
}

#if 0
Graph::Graph(const Graph &graph) {
        m_Vertices.assign(graph.m_Vertices.begin(), graph.m_Vertices.end());
        m_VerticesPt = graph.m_VerticesPt;
        m_Edges.assign(graph.m_Edges.begin(), graph.m_Edges.end());
        m_BestEdgesPt=graph.m_BestEdgesPt;
}
Graph::~Graph(void) { clear();}
#endif
Graph::Graph(void) {}


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
BaseVertex* Graph::getNewVertex(int vertex_id) {
    UINT nodePos;
    nodePos = m_Vertices.size();
    BaseVertex vertex(nodePos, vertex_id);
//vertex.PrintOut(std::cout);
    m_Vertices.push_back(vertex); //optimistic insertion  // TODO maybe not needed
    vSetIt it;
    it = m_VerticesPt.find(&m_Vertices.back());
    if (it == m_VerticesPt.end()) {
//std::cout<<"not found\n";
//assert(true==false);
        m_VerticesPt.insert(&m_Vertices.back());
        return &m_Vertices.back();
    } else { //was already inserted
//std::cout<<"found\n";
//(*it)->PrintOut(std::cout);
//assert(true==false);
        m_Vertices.pop_back();
        return (*it);
    }
    return NULL;
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
BaseEdge* Graph::insertNewEdge(int edge_id,  UINT startId, UINT endId, double edge_weight) {
    UINT edgePos = m_Edges.size();
    BaseEdge edge(edgePos, edge_id, startId, endId, edge_weight);
    m_Edges.push_back(edge);
    BaseEdge *edgePt = &m_Edges.back();

    m_Vertices[startId].push_FanOut(edgePt);
    m_Vertices[endId].push_FanIn(edgePt);
    updateBestEdgesSet(edgePt);
    return edgePt;
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

#if 0
double Graph::vertexWeight(UINT vertex_id) const { 
    assert (vertex_id < m_Vertices.size());
    return m_Vertices[vertex_id].Weight();
}
double Graph::edgeWeight(UINT edge_id) const {
    assert (edge_id < m_Edges.size());
    return m_Edges[edge_id].Weight();
}
#endif
int Graph::getVertexOriginalID(UINT vertex_id) const {
    assert (vertex_id < m_Vertices.size());
    return m_Vertices[vertex_id].getOriginalID();
}
#if 0


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
#endif


void Graph::clear() {
        m_Edges.clear();
        m_Vertices.clear();
        m_VerticesPt.clear();
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

BaseVertex*  Graph::find_vertex(int vertex_id) const {
    UINT nodePos;
    nodePos = m_Vertices.size();
    BaseVertex vertex(nodePos, vertex_id);
    vSetIt it;
    it = m_VerticesPt.find(&vertex);
    if (it == m_VerticesPt.end()) {
#if 0
std::cout<<"not found\n";
PrintOut(std::cout);
assert(true==false);
#endif
       return NULL;
    } 
#if 0
std::cout<<"found\n";
(*it)->PrintOut(std::cout);
std::cout<<"found\n";
assert(true==false);
#endif
    return (*it);
}

#if 0
bool  Graph::exist_vertex(int vertex_id) const {
    UINT i;
    if (m_Vertices.size() == 0) return false;
    for (i=0; i < m_Vertices.size() && m_Vertices[i].getOriginalID() != vertex_id; i++) {}
    if (i >= m_Vertices.size()) return false;
    return true;
}
#endif

void  Graph::remove_edge(BaseEdge *edgePt) {
    edgePt->remove();
}

void  Graph::removeVertices(const BasePath &path) {
    if (path.size() == 0) return;
//TODO convert to iterator
    for (UINT i = 0 ; i  <  path.size() ; i++) {
      m_Vertices[ path[i]->getStart() ].remove();
    }
}

void  Graph::restoreVertices(const BasePath &path) {
    if (path.size() == 0) return;
//TODO convert to iterator
    for (UINT i = 0 ; i  <  path.size() ; i++) {
      m_Vertices[ path[i]->getStart() ].restore();
    }
}

#if 0
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
#endif

#if 0
void Graph::getFanInActiveEdges(UINT vertex_id, BaseVertex::eSetPt &edges_set) const{
        assert(vertex_id < m_Vertices.size());
        UINT edgeId, prevNodeId;
        edges_set.clear();
        if (!m_Vertices[vertex_id].isActive()) return;

        BaseVertex::eSetPt FanIn = m_Vertices[vertex_id].getFanIn();
        BaseVertex::eSetPtIt it;
        for (it = FanIn.begin(); it != FanIn.end(); ++it) {
               edgeId = (*it)->ID();
               prevNodeId =  (*it)->getStart();
               if ( m_Vertices[ prevNodeId ].isActive()
                   &&  m_Edges[ edgeId ].isActive())
                       edges_set.insert(*it);
        }
}
#endif

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
    out_stream << "Vertices By ID\n";
    for (vSetIt it = m_VerticesPt.begin(); it != m_VerticesPt.end(); ++it) {
        (*it)->PrintOut(out_stream);
        out_stream << "\n";
    }
}



void Graph::PrintPath(std::ostream &out_stream, const BasePath &path, int startNode) const {
    if (path.size() == 0) {
        out_stream << "Route 0\n";
        out_stream << startNode << "\t-1 \t0\n";
        return;
    }
// TODO convert to iterator
    for (UINT i=0 ; i < path.size() ; i++) {
        BaseEdge* edge = path[i];
        //int start = m_Vertices[ edge.getStart() ].getOriginalID();
	int start = edge->getStart();
        double cost  = edge->Weight();
        int edgeId = edge->originalID();
        out_stream << "Route " << i<< "\n";
        out_stream << start << "\t" << edgeId << "\t" << cost << "\n";
    }
    out_stream<< m_Vertices[ path[path.size()-1 ]->getEnd() ].getOriginalID() << "\t-1 \t0\n";
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
        BaseVertex* startPos;
        BaseVertex* endPos;
        int count = 0;

        while (ifs >> edge_id) {
                if (edge_id == -1)  break;

                ifs >> start_id;
                ifs >> end_id;
                ifs >> edge_weight;

//std::cout<<"inserting"<<edge_id<<"\n";
                if (has_reverse_cost) ifs >> reverse_weight;

                startPos = Graph::getNewVertex(start_id);
                endPos = Graph::getNewVertex(end_id);

                if (edge_weight >0)
                       insertNewEdge(edge_id, startPos->ID(), endPos->ID(), edge_weight);
                if (has_reverse_cost && reverse_weight >0)
                       insertNewEdge(edge_id, endPos->ID(), startPos->ID(), reverse_weight);
                count++;
#if 0
if ((count % 1000)== 0) {
std::cout<<"Edges read\n"<<count;
PrintOut(std::cout);
std::cout<<"\n";
assert(true==false);
}
#endif
        }
        ifs.close();
        assert(m_BestEdgesPt.size());
//assert(true==false);
}
