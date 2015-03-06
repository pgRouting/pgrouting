#include <deque>
#include <cassert>
#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;
struct Vertex
{
    int id;
    double cost;
};


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
    path.clear();

    typedef adjacency_list < listS, vecS, directedS, no_property, Vertex> graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
    //typedef std::pair<int, int> Edge;

    const unsigned int numberOfNodes = m_Vertices.size();
    graph_t graph(numberOfNodes);

    // only the non removed edges of the BestSet
    BaseEdge* edgePt;
    std::set <BaseEdge*, BaseEdge::compBestEdge>::iterator it;
    for (it = m_BestEdgesPt.begin(); it != m_BestEdgesPt.end(); ++it) {
            edgePt = *it;
            if (edgePt->isActive() 
                && m_Vertices[edgePt->getStart()].isActive()
                && m_Vertices[edgePt->getEnd()].isActive()) {
                   edge_descriptor e;
                   bool inserted;
                   //if (cost < 0) continue;
                   tie(e, inserted) = add_edge(edgePt->getStart(), edgePt->getEnd(), graph);
                   graph[e].cost = edgePt->Weight();
                   graph[e].id = edgePt->ID();
                   //typedef typename graph_traits<graph_t>::vertex_descriptor Vertex;
                   //Vertex s = vertex(edgePt->getStart(), graph);
                   //Vertex t = vertex(edgePt->getEnd(), graph);
            }
    }
    std::vector<vertex_descriptor> predecessors(num_vertices(graph));

    vertex_descriptor _source = vertex(source_id, graph);
    if ((long)_source < 0) return path;

    vertex_descriptor _target = vertex(sink_id, graph);
    if ((long)_target < 0) return path;

    std::vector<double> distances(num_vertices(graph));
    // calling Boost function
    dijkstra_shortest_paths(graph, _source,
                predecessor_map(&predecessors[0]).
                weight_map(get(&Vertex::cost, graph))
                .distance_map(&distances[0]));

    std::deque<int> path_vect;
    //int max = MAX_NODES;
    path_vect.push_back(_target);

    while (_target != _source) {
        if (_target == predecessors[_target]) return path;

        _target = predecessors[_target];

        path_vect.push_front(_target);
    }

    assert (path_vect.size() > 1);
    for(UINT i = 0; i< path_vect.size() - 1; i++)
    {
        graph_traits < graph_t >::vertex_descriptor v_src;
        graph_traits < graph_t >::vertex_descriptor v_targ;
        graph_traits < graph_t >::edge_descriptor e;
        graph_traits < graph_t >::out_edge_iterator out_i, out_end;
        if ( i == path_vect.size()-1) break;
           
        v_src = path_vect.at(i);
        v_targ = path_vect.at(i + 1);
        //double cost = 99999999.9;
        int edge_id = 0;

        for (tie(out_i, out_end) = out_edges(v_src, graph);
             out_i != out_end; ++out_i)
        {
            graph_traits < graph_t >::vertex_descriptor v, targ;
            e = *out_i;
            v = source(e, graph);
            targ = target(e, graph);
            if (targ == v_targ) {
                 edge_id = graph[*out_i].id;
                 break;   
            }
        }
        path.push_back(m_Edges[edge_id]);
    }
    return path;
}
#if 0
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
#endif



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

