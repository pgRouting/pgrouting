#include <deque>
#include "vrp_assert.h"
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
        if (  source !=sink ) {
             BaseVertex* sourcePt =find_vertex(source);
             BaseVertex* sinkPt =find_vertex(sink);
             if ((sourcePt == NULL) || (sinkPt == NULL)) return BasePath();
             return boostDijkstra(sourcePt->ID(), sinkPt->ID());
        }
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
    if (localids == false) return Dijkstra(source, sink);
    if ( source < m_Vertices.size() && sink < m_Vertices.size() &&  source != sink)
         return boostDijkstra(source, sink);
    return BasePath();
}

BasePath DijkstraShortestPathAlg::boostDijkstra(UINT source_id, UINT sink_id) {
    assert(m_BestEdgesPt.size());
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


/*!
Restores the graph for a next iteration so it has:
  - all the vertices logically inserted and unvisited in the graph with
    weight set as max
  - all the edges logically inserted in the graph
/todo TODO this can be improved
*/
void DijkstraShortestPathAlg::clear() {
     for (UINT i = 0; i < m_Vertices.size(); i++)
          m_Vertices[i].restore();
     for (UINT i = 0; i < m_Edges.size(); i++)
          m_Edges[i].reInsert();
}
