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
	g.print_Vertex_Degree();
	g.print_Degree_Vertices();
	//g.remove_vertex(0);
	g.contract_to_level(0);
	g.reduced_graph->print_graph();
}

int main(int argc, char const *argv[])
{
	Edge* edges;
	int num_edges=9;
	int num_vertices=10;
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
	edges[2].id=1;
	edges[2].source=1;
	edges[2].target=3;
	edges[2].cost=4;
	edges[2].revcost=-1;
	edges[3].id=1;
	edges[3].source=3;
	edges[3].target=4;
	edges[3].cost=4;
	edges[3].revcost=-1;
	edges[4].id=1;
	edges[4].source=3;
	edges[4].target=5;
	edges[4].cost=4;
	edges[4].revcost=-1;
	edges[5].id=1;
	edges[5].source=5;
	edges[5].target=6;
	edges[5].cost=4;
	edges[5].revcost=-1;
	edges[6].id=1;
	edges[6].source=5;
	edges[6].target=7;
	edges[6].cost=4;
	edges[6].revcost=-1;
	edges[7].id=1;
	edges[7].source=7;
	edges[7].target=8;
	edges[7].cost=4;
	edges[7].revcost=-1;
	edges[8].id=1;
	edges[8].source=7;
	edges[8].target=9;
	edges[8].cost=4;
	edges[8].revcost=-1;
	check(edges,num_vertices,num_edges);

	return 0;
}