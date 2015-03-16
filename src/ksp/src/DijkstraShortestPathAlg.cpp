#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <deque>
#include <set>
#include <vector>

#include "./vrp_assert.h"
#include "./GraphElements.h"
#include "./Graph.h"
#include "./DijkstraShortestPathAlg.h"

using namespace boost;
struct Vertex {
    long id;
    double cost;
};


/*!
\param[in] source: original id of the source
\param[in] sink: original id of the sink
\Returns BasePath
*/
std::deque<BaseEdge> DijkstraShortestPathAlg::Dijkstra(long source, long sink) {
        std::deque<BaseEdge> emptyPath;
        if (source != sink) {
             BaseVertex* sourcePt = find_vertex(source);
             BaseVertex* sinkPt = find_vertex(sink);
             if ((sourcePt == NULL) || (sinkPt == NULL)) return emptyPath;
             return boostDijkstra(sourcePt->ID(), sinkPt->ID()).Path();
        }
        return emptyPath;
}

/*!
\param[in] source: Graph ID of the source
\param[in] sink: Graph ID of the sink
\param[in] localids: when third parameter exists is identify that the IDs are graph's IDs
(the truth value is ignored)
\Returns BasePath
*/
BasePath DijkstraShortestPathAlg::Dijkstra(UINT source, UINT sink) {
    if ( source < m_Vertices.size() && sink < m_Vertices.size() &&  source != sink)
         return boostDijkstra(source, sink);
    return BasePath();
}

// TODO(vicky) brake this huge function into little more esay to undertand functions
BasePath DijkstraShortestPathAlg::boostDijkstra(UINT source_id, UINT sink_id) {
    assert(m_BestEdgesPt.size());
    BasePath path;
    path.clear();

    typedef adjacency_list < listS, vecS, directedS, no_property, Vertex> graph_t;
    typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
    typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

    const unsigned int numberOfNodes = m_Vertices.size();
    graph_t graph(numberOfNodes);

    // only the non removed edges of the BestSet
    BaseEdge* edgePt;
    std::set <BaseEdge*, BaseEdge::compBestEdge>::iterator it;
    for (it = m_BestEdgesPt.begin(); it != m_BestEdgesPt.end(); ++it) {
            edgePt = *it;
            if (isActive(edgePt)) {
                   assert(edgePt->isMain());
                   edge_descriptor e;
                   bool inserted;
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

    assert(path_vect.size() > 1);
    for (UINT i = 0; i< path_vect.size() - 1; i++) {
        graph_traits < graph_t >::vertex_descriptor v_src;
        graph_traits < graph_t >::vertex_descriptor v_targ;
        graph_traits < graph_t >::edge_descriptor e;
        graph_traits < graph_t >::out_edge_iterator out_i, out_end;
        if ( i == path_vect.size()-1) break;

        v_src = path_vect.at(i);
        v_targ = path_vect.at(i + 1);

        long edge_id = 0;

        for (tie(out_i, out_end) = out_edges(v_src, graph);
             out_i != out_end; ++out_i) {
            graph_traits < graph_t >::vertex_descriptor v, targ;
            e = *out_i;
            v = source(e, graph);
            targ = target(e, graph);
            if (targ == v_targ) {
                 edge_id = graph[*out_i].id;
                 break;
            }
        }
        path.push_back(&m_Edges[edge_id]);
    }
    return path;
}


/*!
book-keeping is done of the removed edges
if the removed edge is a minimal, then its the only one removed
otherwise from all the parallel edges, the minimal is searched and removed
/param[in] edgePt: pointer to the edge to be removed
*/
void DijkstraShortestPathAlg::removeEdgeAndParallels(BaseEdge *edgePt) {
    if (edgePt->isMain() && edgePt->isActive()) {
        edgePt->remove();
        removedEdges.push_back(edgePt);
        m_BestEdgesPt.erase(m_BestEdgesPt.find(edgePt));
        return;
    }
    BaseVertex *startPt = &m_Vertices[ edgePt->getStart() ];
    BaseVertex::eSetPt fanOutSet = startPt->getFanOut();
    BaseVertex::eSetPtIt fIt;
    for (fIt = fanOutSet.begin(); fIt !=fanOutSet.end(); ++fIt) {
        // skiping:
        //   somethingwent really wrong
        //   not a parallel edge
        //   parallel but not the minimal
        //   already removed edges
        if ((*fIt)->getStart() != startPt->ID()) assert(true == false);
        if ((*fIt)->getEnd() != edgePt->getEnd()) continue;
        if ((*fIt)->isParallel()) continue;
        if (!((*fIt)->isActive())) continue;
        // the minimal parallel non-removed edge has being found
        BaseEdge *edgeMain = (*fIt);
        assert(edgeMain->isMain() && edgeMain->isActive());
        m_BestEdgesPt.erase(edgeMain);
        edgeMain->remove();
        removedEdges.push_back(edgeMain);
        break;
    }
}

void DijkstraShortestPathAlg::restoreEdges() {
    while (removedEdges.size()) {
        removedEdges.front()->restore();
        m_BestEdgesPt.insert(removedEdges.front());
        removedEdges.pop_front();
    }
}

/*!
An edge to be active, the follwoing conditions must be met:
  - must be active by itself
  - the start vertex must be active
  - the end vertex must be active
/param[in] edgePt: pointer to the tested edge
*/
bool DijkstraShortestPathAlg::isActive(const BaseEdge *edgePt) const {
    return edgePt->isActive()
        && m_Vertices[edgePt->getStart()].isActive()
        && m_Vertices[edgePt->getEnd()].isActive();
}

/*!
Does not remove the incomming/outgoing edges of the vertex:

When "edgePt" is a pointer to an  incomming or outgoing edge the following
 - isActive(edgePt): will return false because one of the vertices is removed
 - edgePt->isActive(): will return true because the edge is still active

/param[in] path: the path containg the vertices to be removed
*/
void  DijkstraShortestPathAlg::removeVertices(const BasePath &path) {
    if (path.size() == 0) return;
    BasePath::eDequeIt eIt;
    for (eIt =  path.path().begin(); eIt !=  path.path().end(); ++eIt) {
      m_Vertices[ (*eIt)->getStart() ].remove();
    }
}

/*!
/param[in] path: the path containg the vertices to be restored
*/
void  DijkstraShortestPathAlg::restoreVertices(const BasePath &path) {
    if (path.size() == 0) return;
    BasePath::eDequeIt eIt;
    for (eIt =  path.path().begin(); eIt !=  path.path().end(); ++eIt) {
      m_Vertices[ (*eIt)->getStart() ].restore();
    }
}


/*!
Does not:
  - Deletes the vertices
  - Deletes the edges

Restores the graph for a next iteration so it has:
  - all the vertices are logically inserted
  - all the edges are logically inserted
*/
void DijkstraShortestPathAlg::clear() {
     vListIt vIt;
     for (vIt =  m_Vertices.begin(); vIt !=  m_Vertices.end(); ++vIt)
          (*vIt).restore();
     eListIt eIt;
     for (eIt =  m_Edges.begin(); eIt !=  m_Edges.end(); ++eIt)
          (*eIt).restore();
     removedEdges.clear();
}
