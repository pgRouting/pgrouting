#include <limits>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GraphElements.h"
#include "KSPGraph.h"
#include "Graph.h"
#include "YenTopKShortestPathsAlg.h"
extern "C" {
#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "fmgr.h"
#include "ksp.h"
	extern void DBG(const char *format, ...);
}
#undef DEBUG
#define DEBUG 1

#ifndef _MSC_VER
#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif
#else // _MSC_VER
extern void DBG(const char *format, ...)
{
#if DEBUG
        va_list ap;
        char msg[256];
        va_start(ap, format);
        _vsprintf_p(msg, 256, format, ap);
        va_end(ap);
        elog(NOTICE, msg);
#endif
}
#endif // _MSC_VER

KSPGraph::KSPGraph(void){
}
KSPGraph::~KSPGraph(void){
}
KSPGraph::KSPGraph(const Graph& g):Graph(g){
}
KSPGraph::KSPGraph(const std::string & f):Graph(f){
}

void KSPGraph::clear(void){
	Graph::clear();
	m_mpEdgeValues.clear();;
	
}
void KSPGraph::StartLoad(const  int len ){
	clear();
	m_nVertexNum=len;
}
/*
 * a rewrite of input_from file, but one line at the time
 */
void KSPGraph::AddData(const int start_vertex,const int end_vertex ,const float edge_weight,const int edge_id){

    DBG("Start %d  Target %d  Edge %f",start_vertex,end_vertex, edge_weight);
    ///3.2.1 construct the vertices
    BaseVertex* start_vertex_pt = get_vertex(start_vertex);
    BaseVertex* end_vertex_pt = get_vertex(end_vertex);

    ///3.2.2 add the edge weight
    //// note that the duplicate edge would overwrite the one occurring before.

    m_mpEdgeCodeWeight[get_edge_code(start_vertex_pt, end_vertex_pt)] = edge_weight;

    ///3.2.3 update the fan-in or fan-out variables
    //// Fan-in
    get_vertex_set_pt(end_vertex_pt, m_mpFaninVertices)->insert(start_vertex_pt);

    //// Fan-out
    get_vertex_set_pt(start_vertex_pt, m_mpFanoutVertices)->insert(end_vertex_pt);

    m_mpEdgeValues[get_edge_code(start_vertex_pt, end_vertex_pt)] = edge_id;
}
void KSPGraph::EndLoad(void){
	m_nVertexNum = m_vtVertices.size();
  	m_nEdgeNum = m_mpEdgeCodeWeight.size();
}

int  KSPGraph::get_edge_value( const BaseVertex* source, const BaseVertex* sink )
{
  std::map<int, int>::const_iterator pos =
    m_mpEdgeValues.find(get_edge_code(source, sink));

  if (pos != m_mpEdgeValues.end())
  {
    return pos->second;
  }else
  {
    return DISCONNECT;
  }
}
