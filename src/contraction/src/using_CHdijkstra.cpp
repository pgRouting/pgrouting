#include "../../common/src/structs.h"
#include "CHdijkstra.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
void print_path(Edge *data_edges,int count)
{
	for (int i = 0; i < count; ++i)
	{
		cout << "(" << data_edges[i].id << ", "<< data_edges[i].source << ", "
			<< data_edges[i].target << ", "<< data_edges[i].cost 
			<< ", "<< data_edges[i].revcost << ", "  << " )" << endl;
}
}
int main(int argc, char const *argv[])
{
	/* code */
	typedef CH_dijkstra<G> Graph;
	Graph g(UNDIRECTED,0);
	Edge *edges=NULL;
	string sedges="2,2,3,2,0$3,2,4,6,0$4,3,4,3,0$";
	string rvertices="1,1,1,2,1,0$5,5,4,5,1,0$6,6,5,6,4,0$";
	string redges="1,1,2,1,-1$5,5,4,1,-1$6,6,5,4,-1$";
	string pedges="";
	g.initialize_CH(sedges,rvertices,redges,pedges);
	Edge *path;
	int64_t size;
	g.dijkstra_on_contracted(1,5,&path,size);
	print_path(path,size);

	return 0;
}