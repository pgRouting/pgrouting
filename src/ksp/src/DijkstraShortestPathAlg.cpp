#include <deque>
#include <cassert>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"

BasePath DijkstraShortestPathAlg::Dijkstra(int source, int sink) {
        if (  source !=sink  && exist_vertex(source) && exist_vertex(sink) )
             return get_shortest_path( find_vertex(source), find_vertex(sink) );
        return BasePath();
}

BasePath DijkstraShortestPathAlg::Dijkstra(UINT source, UINT sink, bool localids) {
    if ( source < m_Vertices.size() && sink < m_Vertices.size() &&  source != sink)
         return get_shortest_path( source, sink );
    return BasePath();
}

BasePath DijkstraShortestPathAlg::get_shortest_path(UINT source_id, UINT sink_id) {
        BasePath path;
        m_CandidateVertices.clear();

        determine_shortest_paths(source_id, sink_id);
//        std::deque<BaseEdge> edges_list;

        double weight = vertexWeight(sink_id);


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




void DijkstraShortestPathAlg::determine_shortest_paths(UINT source_id, UINT sink_id) {
        m_Vertices[source_id].Weight(0);  // the source starts with 0 as cost
        m_CandidateVertices.insert(&m_Vertices[source_id]);
        improve2vertex(sink_id);  // start searching for the shortest path
}

void DijkstraShortestPathAlg::improve2vertex(UINT sink_id ) {
     while (!m_CandidateVertices.empty()) {
        int current_id = selectBestCandidate();

        // 1. get the neighboring edges (vertices are stiored on the vertices)
        std::deque<UINT> neighbor_edges_list;
        getFanOutActiveEdgesIds(current_id, neighbor_edges_list);

        // 2. update the distance passing on the current vertex
        BaseEdge *edge_pt;
        BaseVertex *curr_vertex_pt;
        BaseVertex *next_vertex_pt;
        double distance;
        for (UINT i = 0; i < neighbor_edges_list.size(); i++) {
                edge_pt = &m_Edges[ neighbor_edges_list[i] ];
                curr_vertex_pt =  getVertexPt(current_id);
                next_vertex_pt =  getVertexPt(edge_pt->getEnd());

               // 2.1 skip if it has been visited before
               if (next_vertex_pt->visited()) continue;

               InsertIntoCandidate(next_vertex_pt->ID());

               // 2.2 calculate the distance
               distance = curr_vertex_pt->Weight() + edge_pt->Weight();

               // 2.3 update the distance if necessary (comparison and assignment in BaseVertex)
                next_vertex_pt->Weight(distance);
                curr_vertex_pt->setAsVisited();  // mark as Visited
                if (curr_vertex_pt->ID() == sink_id) return;
        }
        neighbor_edges_list.clear();
      }
}

UINT DijkstraShortestPathAlg::selectBestCandidate() {
    BaseVertex *vertex = (*m_CandidateVertices.begin());
    UINT best_id = vertex->ID();
    m_CandidateVertices.erase(m_CandidateVertices.begin());
    return best_id;
}
    
void DijkstraShortestPathAlg::InsertIntoCandidate(UINT node_id) {
    m_CandidateVertices.insert(&m_Vertices[node_id]);
}

void DijkstraShortestPathAlg::clear() {
     for (UINT i = 0; i < m_Vertices.size(); i++)
          m_Vertices[i].restore();
     for (UINT i = 0; i < m_Edges.size(); i++)
          m_Edges[i].reInsert();
}

