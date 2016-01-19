
#include "myDijkstra.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include "dijkstra_function.h"
using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int compute_dijkstra(int64_t source,int64_t target,Edge *edges,int64_t num_vertices,int64_t num_edges,Edge **path)
{
	int64_t path_size=0;
	typedef My_dijkstra<G> Graph;
	Graph g(DIRECTED,num_vertices);
	g.initialize_graph(edges,num_edges);
	g.perform_dijkstra(source,target,path,path_size);
	return path_size;
}



int dijkstra_on_contracted(int64_t source,int64_t target,
	pgr_contracted_blob **graphInfo,bool has_rcost)
{
	//int64_t path_size=0;
	//typedef My_dijkstra<G> Graph;
	//Graph g(DIRECTED,num_vertices);
	//fetch edges,removed vertices,removed edges,psuedo edges from string
}