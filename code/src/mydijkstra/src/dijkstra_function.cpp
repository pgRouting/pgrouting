
#include "myDijkstra.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include "dijkstra_function.h"

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
//calculates the shortest path,returns the pathsize and also the path as an array of edges
int compute_dijkstra(int source,int target,Edge *edges,int num_vertices,int num_edges,Edge **path)
{
	int path_size=0;
	typedef My_dijkstra<G> Graph;
	Graph g(DIRECTED,num_vertices);
	g.initialize_graph(edges,num_edges);
	g.perform_dijkstra(source,target,path,path_size);
	return path_size;
}