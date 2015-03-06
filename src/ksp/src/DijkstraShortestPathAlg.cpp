#include <deque>
#include <cassert>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"

/*!
\param[in] source: original id of the source
\param[in] sink: original id of the sink
\Returns BasePath
*/
BasePath DijkstraShortestPathAlg::Dijkstra(int source, int sink) {
        if (  source !=sink  && exist_vertex(source) && exist_vertex(sink) )
             return get_shortest_path( find_vertex(source), find_vertex(sink) );
        return BasePath();
}

/*!
\param[in] source: Graph ID of the source
\param[in] sink: Graph ID of the sink
\param[in] localids: when third parameter exists is identify that the IDs are graph's IDs
(the truth value is ignored)
\Returns BasePath
*/
BasePath DijkstraShortestPathAlg::Dijkstra(UINT source, UINT sink, bool localids) {
    if ( source < m_Vertices.size() && sink < m_Vertices.size() &&  source != sink)
         return get_shortest_path( source, sink );
    return BasePath();
}

/*!
\param[in] source: Graph ID of the source
\param[in] sink: Graph ID of the sink
\Returns BasePath
*/
BasePath DijkstraShortestPathAlg::get_shortest_path(UINT source_id, UINT sink_id) {
        BasePath path;
        clear();
        m_CandidateVertices.clear();
        m_CandidateVerticesId.clear();

        determine_shortest_paths(source_id, sink_id);

        double weight = vertexWeight(sink_id);

        // 
        UINT curr_vertex;
        BaseEdge *edge_pt;
        if (weight < Graph::DISCONNECT) {
               curr_vertex = sink_id;
               do {
                  edge_pt = bestEdge(curr_vertex);
                  if (!edge_pt) break;
                  path.push_front(*edge_pt);
                  curr_vertex =  edge_pt->getStart();
               } while (curr_vertex != source_id);
        }
        return path;
}

/*!
\param[in] sink: Graph ID of the sink
\Returns BaseEdge pointer
*/
BaseEdge* DijkstraShortestPathAlg::bestEdge(UINT sink_id) {
        std::deque<BaseEdge> incomming_edges_list;
        double curr_weight = vertexWeight(sink_id);
        getFanInActiveEdges(sink_id, incomming_edges_list);

        double prev_weight;
        for (UINT i = 0; i < incomming_edges_list.size(); i++) {
                BaseEdge edge = incomming_edges_list[i];
                prev_weight = vertexWeight(edge.getStart());
                if (curr_weight == prev_weight + edge.Weight())
                    return &m_Edges[edge.ID()];
        }
        return NULL;
}




/*!
\param[in] source_id: Graph ID of the source
\param[in] sink_id: Graph ID of the sink
\Returns BasePath
*/
void DijkstraShortestPathAlg::determine_shortest_paths(UINT source_id, UINT sink_id) {
        m_Vertices[source_id].Weight(0);  // the source starts with 0 as cost

        insertIntoCandidate(source_id);
        //m_CandidateVertices.insert(&m_Vertices[source_id]);
        improve2vertex(sink_id);  // start searching for the shortest path
}



/*!
Uses extensively m_CandidateVertices
\param[in] sink_id: Graph ID of the sink
\Returns BasePath
*/

void DijkstraShortestPathAlg::improve2vertex(UINT sink_id ) {
     while (!m_CandidateVertices.empty()) {
        int current_id = selectBestCandidate();
        BaseVertex *curr_vertex_pt  =  getVertexPt(current_id);

        // 1. get the neighboring edges (vertices are stored on the vertices)
        std::deque<UINT> neighbor_edges_list;
        getFanOutActiveEdgesIds(current_id, neighbor_edges_list);

        // 2. update the distance passing on the current vertex
        BaseEdge *edge_pt;
        BaseVertex *next_vertex_pt;
        double weight;
        for (UINT i = 0; i < neighbor_edges_list.size(); i++) {
                edge_pt = &m_Edges[neighbor_edges_list[i]];
                next_vertex_pt =  getVertexPt(edge_pt->getEnd());

               // 2.1 skip if it has been visited before
               if (next_vertex_pt->visited()) continue;


               // 2.2 calculate the distance
               weight = curr_vertex_pt->Weight() + edge_pt->Weight();
               // 2.3 update the weight if necessary (comparison and assignment in BaseVertex)
               next_vertex_pt->Weight(weight);

               insertIntoCandidate(next_vertex_pt->ID());

               if (curr_vertex_pt->ID() == sink_id) return;
        }
        curr_vertex_pt->setAsVisited();  
        neighbor_edges_list.clear();
      }
}

/*!
The m_CandidateVertices becuase its a set the best candidate is the fist element.\n
At the end removes the vertex as candidate, becuase it eventually will be set
as visited.
\Returns ID of best vertex candidate
*/
UINT DijkstraShortestPathAlg::selectBestCandidate() {
    assert(m_CandidateVertices.size() == m_CandidateVerticesId.size());
    assert(m_CandidateVertices.size());
    //move the smallest to the begining of the deque
    for (UINT i = m_CandidateVertices.size()-1; i > 0; --i) {
         if ( m_CandidateVertices[i]->Weight() < m_CandidateVertices[i-1]->Weight() ) {
           std::swap(m_CandidateVertices[i-1],m_CandidateVertices[i]);
         }
    }
    UINT best_id = m_CandidateVertices[0]->ID();
    //need to remove the id from the set of id's
    m_CandidateVerticesId.erase(m_CandidateVerticesId.find(best_id));
    // and from the deque
    m_CandidateVertices.erase(m_CandidateVertices.begin());
    assert(m_CandidateVertices.size() == m_CandidateVerticesId.size());
    return best_id;
}
    
/*!
Inserts the pointer to the vertex as a candidate
\params[in] vertex_id
*/
void DijkstraShortestPathAlg::insertIntoCandidate(UINT vertex_id) {
     assert(vertex_id < m_Vertices.size());
     
     if ( m_CandidateVerticesId.find(vertex_id) == m_CandidateVerticesId.end()) {
         m_CandidateVerticesId.insert(vertex_id);
         m_CandidateVertices.push_back(&m_Vertices[vertex_id]);
     }
/*
     for (UINT i = m_CandidateVertices.size()-1; i > 0; --i) {
         if ( m_CandidateVertices[i]->Weight() < m_CandidateVertices[i-1]->Weight() ) {
           std::swap(m_CandidateVertices[i-1],m_CandidateVertices[i]);
         }
     }
*/
     assert(m_CandidateVertices.size() == m_CandidateVerticesId.size());
}

/*!
Restores the graph for the next iteration so it has:
  - all the vertices logically inserted and unvisited in the graph with
    weight set as max
  - all the edges logically inserted in the graph
*/
void DijkstraShortestPathAlg::clear() {
     for (UINT i = 0; i < m_Vertices.size(); i++)
          m_Vertices[i].restore();
     for (UINT i = 0; i < m_Edges.size(); i++)
          m_Edges[i].reInsert();
}

