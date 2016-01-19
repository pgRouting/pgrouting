#include "structs.h"
#include "baseGraph.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
int main(int argc, char const *argv[])
{
	/* code */
	typedef Pgr_base_graph<G> Graph;
	Graph g(UNDIRECTED,0);
	Edge *edges=NULL;
	edges=(Edge*)malloc(2*sizeof(Edge));
	edges[0].id=0;
	edges[0].source=0;
	edges[0].target=1;
	edges[0].cost=5;
	edges[0].revcost=-1;

	edges[1].id=1;
	edges[1].source=1;
	edges[1].target=2;
	edges[1].cost=4;
	edges[1].revcost=-1;
	g.graph_insert_data(edges,2);
	//g.remove_vertex(0);
	g.print_graph();
	return 0;
}