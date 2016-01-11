#include "graphMinimizer.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include "contract_function.h"

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int fetch_contracted_graph(Edge *edges,int *initial_num_vertices,
							int *final_num_vertices,int initial_num_edges,
							int *final_num_edges,int level,Edge **final_edges,
							char **graphName,char **edgeString)
{
	int final_edge_count=0;
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,*initial_num_vertices);
	g.initialize_graph_minimizer(edges,initial_num_edges);
	*initial_num_vertices=g.numb_vertices;
	g.contract_to_level(level);
	final_edge_count=g.getreducedGraph(final_edges);
	*final_num_vertices=g.reduced_graph->numb_vertices;
	g.getGraphName(graphName,level);
	g.getEdgeString(final_edges,final_edge_count,edgeString);
	*final_num_edges=final_edge_count;
	return 0;
}