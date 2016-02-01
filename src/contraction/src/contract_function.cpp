#include "graphMinimizer.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include "contract_function.h"

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int64_t fetch_contracted_graph(Edge *edges,int64_t *initial_num_vertices,
							int64_t *final_num_vertices,int64_t initial_num_edges,
							int64_t *final_num_edges,int64_t level,char **graphName,
							char **edgeString,char **psuedoEString,
							char **removedVString,char **removedEString)
{
	int64_t final_edge_count=0;
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,*initial_num_vertices);
	g.initialize_graph_minimizer(edges,initial_num_edges);
	*initial_num_vertices=g.numb_vertices;
	g.contract_to_level(level);
	final_edge_count=g.getreducedGraph_string(edgeString);
	*final_num_vertices=g.reduced_graph->numb_vertices;
	g.getGraphName(graphName,level);
	//g.getEdgeString(final_edges,final_edge_count,edgeString);
	g.getRemovedE_string(removedEString);
	g.getRemovedV_string(removedVString);
	g.getPsuedoE_string(psuedoEString);
	*final_num_edges=final_edge_count;
	return 0;
}