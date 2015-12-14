
#include "graphMinimizer.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include "contract_function.h"

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int compute_contracted_graph(Edge *edges,int num_vertices,int num_edges,int level,Edge **final_edges)
{
	int final_edge_count=0;
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,num_vertices);
	g.initialize_graph(edges,num_edges);
	g.contract_to_level(level);
	final_edge_count=g.getreducedGraph(final_edges);
	return final_edge_count;
}