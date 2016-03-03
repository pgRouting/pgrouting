#include "graphMinimizer.hpp"
#include <iostream>
#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include "contract_function.h"

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int64_t fetch_contracted_graph(Edge *edges,int64_t *initial_num_vertices,
							int64_t *final_num_vertices,int64_t initial_num_edges,
							int64_t *final_num_edges,int64_t level,const char **graphName,
							const char **edgeString,const char **psuedoEString,
							const char **removedVString,const char **removedEString)
{
	int64_t final_edge_count=0;
	ostringstream graphnameStream,edgeStream,removedEStream,removedVStream,psuedoEStream;
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,*initial_num_vertices);
	g.initialize_graph_minimizer(edges,initial_num_edges);
	*initial_num_vertices=g.numb_vertices;
	g.contract_to_level(level);
	final_edge_count=g.getreducedGraph_string(edgeStream);
	*final_num_vertices=g.reduced_graph->numb_vertices;
	g.getGraphName(graphnameStream,level);
	g.getRemovedE_string(removedEStream);
	g.getRemovedV_string(removedVStream);
	g.getPsuedoE_string(psuedoEStream);
	*graphName=graphnameStream.str().c_str();
	*edgeString=edgeStream.str().c_str();
	*psuedoEString=psuedoEStream.str().c_str();
	*removedVString=removedVStream.str().c_str();
	*removedEString=removedEStream.str().c_str();
	*final_num_edges=final_edge_count;
	return 0;
}