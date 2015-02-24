
#ifndef __KSPGraph_h
#define __KSPGraph_h

#include "GraphElements.h"
#include "Graph.h"
extern "C" {
#include "ksp.h"
}

class KSPGraph : public Graph {
public:
	~KSPGraph();
	KSPGraph(const Graph&);
	KSPGraph();
	KSPGraph(const std::string &);
        void AddData( ksp_edge_t  * edges,  int total_tuples, bool has_reverse_cost);
        void insertTuple( int edgeId, int source, int target, float cost, float reverse_cost, bool has_reverse_cost);

//	void AddData(const int source , const int target , const float cost, const int id);
	void StartLoad();
	void EndLoad();
	void clear();

	//int get_edge_value(const BaseVertex* source, const BaseVertex* sink ) ;
//protected:
	//std::map <int,int> m_mpEdgeValues;

};
#endif //__KSPGraph
