#include "graphMinimizer.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
void check(Edge *edges,int num_vertices,int num_edges)
{
	int path_size=0;
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,num_vertices);
	g.initialize_graph(edges,num_edges);
	g.reduced_graph->print_graph();
	//g.print_Vertex_Degree();
	//g.print_Degree_Vertices();
	//g.remove_vertex(0);
	g.remove_1_degree_vertices();
	g.reduced_graph->print_graph();
}

int main(int argc, char const *argv[])
{
	Edge* edges;
	int num_edges=2;
	int num_vertices=3;
	edges=(Edge*)malloc(sizeof(Edge)*num_edges);
	edges[0].id=0;
	edges[0].source=0;
	edges[0].target=1;
	edges[0].cost=0.625;
	edges[0].revcost=-1;
	edges[1].id=1;
	edges[1].source=1;
	edges[1].target=2;
	edges[1].cost=4;
	edges[1].revcost=-1;
	check(edges,num_vertices,num_edges);

	return 0;
}