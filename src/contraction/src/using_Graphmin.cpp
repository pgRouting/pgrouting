#include "../../common/src/structs.h"
#include "graphMinimizer.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost;
typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
void string_to_edges(string edges,Edge **data_edges,int *count)
{
	cout << "getting edges" << endl;
	std::vector<string> str_edges;
	std::vector<string> str_edge;
	boost::split(str_edges, edges, boost::is_any_of("$"));
	*count=str_edges.size();
	cout << "count " << *count-1 << endl;
	(*data_edges)=(Edge*)malloc((*count-1)*sizeof(Edge));
	for (int i = 0; i < *count-1; ++i)
	{
		cout << "edge:- " << str_edges[i] << endl;
		if (str_edges[i].size()!=0)
		{
			boost::split(str_edge,str_edges[i], boost::is_any_of(","));

			for (int j = 0; j < 5; ++j)
			{
				switch(j)
				{
					case 0:
					(*data_edges)[i].id=atoi(str_edge[j].c_str());
					break;
					case 1:
					(*data_edges)[i].source=atoi(str_edge[j].c_str());
					break;
					case 2:
					(*data_edges)[i].target=atoi(str_edge[j].c_str());
					break;
					case 3:
					(*data_edges)[i].cost=atof(str_edge[j].c_str());
					break;
					case 4:
					(*data_edges)[i].revcost=atof(str_edge[j].c_str());
					break;
				}
			}  
		}   
	}
	*count=*count-1;

}
void print_edges(Edge *data_edges,int count)
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
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,0);
	Edge *edges=NULL;
	int edge_count=6;
	string sedges="2,2,3,2,0$3,2,4,1,0$4,3,4,3,0$";
	//string_to_edges(sedges,&edges,&edge_count);
	//print_edges(edges,edge_count);
	edges=(Edge*)malloc(edge_count*sizeof(Edge));
	edges[0].id=1;
	edges[0].source=1;
	edges[0].target=2;
	edges[0].cost=1;
	edges[0].revcost=-1;

	edges[1].id=2;
	edges[1].source=2;
	edges[1].target=3;
	edges[1].cost=2;
	edges[1].revcost=-3;

	edges[2].id=3;
	edges[2].source=2;
	edges[2].target=4;
	edges[2].cost=1;
	edges[2].revcost=-1;

	edges[3].id=4;
	edges[3].source=3;
	edges[3].target=4;
	edges[3].cost=3;
	edges[3].revcost=-1;

	edges[4].id=5;
	edges[4].source=4;
	edges[4].target=5;
	edges[4].cost=1;
	edges[4].revcost=-2;

	edges[5].id=6;
	edges[5].source=5;
	edges[5].target=6;
	edges[5].cost=4;
	edges[5].revcost=-1;
	g.initialize_graph_minimizer(edges,edge_count);
	//g.print_graph();
	cout << "initial vertices " << g.reduced_graph->numb_vertices << endl;
	g.contract_to_level(1);
	cout << "final contracted vertices " << g.reduced_graph->numb_vertices << endl;
	g.print_removed_vertices();
	g.print_psuedo_edges();
	g.print_removed_edges();
	char *name;
	//g.getGraphName(&name,0);
	//cout << "Name: " << name << endl;
	//g.getEdgeString(&edges,edge_count,&name);
	//cout << "Edge String: " << name << endl;
	//g.remove_vertex(0);
	

	return 0;
}