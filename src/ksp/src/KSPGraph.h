
#ifndef __KSPGraph_h
#define __KSPGraph_h

#include "GraphElements.h"
#include "Graph.h"

class KSPGraph : public Graph {
public:
	~KSPGraph();
	KSPGraph(const Graph&);
	KSPGraph();
	KSPGraph(const std::string &);
	void AddData(const int source , const int target , const float cost,const int id);
	void StartLoad(const int );
	void EndLoad();
	void clear();

	int get_edge_value(const BaseVertex* source, const BaseVertex* sink ) ;
protected:
	std::map <int,int> m_mpEdgeValues;

};
#endif //__KSPGraph
