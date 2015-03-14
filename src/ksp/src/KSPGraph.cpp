#include <string>
 #include "KSPGraph.h"
extern "C" {
#include "ksp.h"
}

KSPGraph::KSPGraph(void) {}
KSPGraph::~KSPGraph(void) {}
KSPGraph::KSPGraph(const Graph& g):Graph(g) {}
KSPGraph::KSPGraph(const std::string & f):Graph(f) {}
void KSPGraph::StartLoad() { clear();}
void KSPGraph::EndLoad(void) {}
void KSPGraph::clear(void) { Graph::clear();}

void KSPGraph::AddData(ksp_edge_t  * edges,  UINT total_tuples, bool has_reverse_cost) {
       for (UINT i = 0; i < total_tuples; i++) {
            insertTuple(edges[i].id, edges[i].source, edges[i].target,
            edges[i].cost, edges[i].reverse_cost, has_reverse_cost);
       }
}

void KSPGraph::insertTuple(long id, long source, long target,
                float cost, float reverse_cost, bool has_reverse_cost) {
    if (cost >= 0 || (reverse_cost>= 0 && has_reverse_cost)) {
       BaseVertex* sourcePos = getNewVertex(source);
       BaseVertex* targetPos = getNewVertex(target);
       if (cost >= 0) {
           insertNewEdge(id, sourcePos->ID(), targetPos->ID(), cost);
       }
       if (reverse_cost>= 0 && has_reverse_cost) {
           insertNewEdge(id, targetPos->ID(), sourcePos->ID(), reverse_cost);
       }
    }
}



