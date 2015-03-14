#ifndef SRC_KSP_SRC_KSPGRAPH_H_
#define SRC_KSP_SRC_KSPGRAPH_H_

#include <string>
#include "GraphElements.h"
#include "Graph.h"

extern "C" {
#include "ksp.h"
}

class KSPGraph : public Graph {
 public:
    ~KSPGraph();
    explicit KSPGraph(const Graph&);
    KSPGraph();
    explicit KSPGraph(const std::string &);
    void AddData(ksp_edge_t  * edges,  UINT total_tuples, bool has_reverse_cost);
    void insertTuple(long edgeId, long source, long target,
                     float cost, float reverse_cost, bool has_reverse_cost);

    void StartLoad();
    void EndLoad();
    void clear();
};
#endif  // SRC_KSP_SRC_KSPGRAPH_H_
