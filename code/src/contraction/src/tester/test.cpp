#include "../graphMinimizer.hpp"
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
using namespace std;
using namespace boost;
int loadGraph(string edgeFile,Edge **edges)
{

	int number_of_lines = 0;
	string line;
	ifstream myfile(edgeFile.c_str());
	ifstream myfile1(edgeFile.c_str());
	//ifstream myfile;
	//myfile.open("ways.txt");
	if (!myfile)
	{
		//throw new std::exception("Rule file could not be opened");
		cout << "cannot open" << endl;
	}
	while (getline(myfile, line))
		++number_of_lines;
	//cout << "line count " << number_of_lines << endl; 
	*edges=(Edge*)malloc(sizeof(Edge)*number_of_lines);
	int colid=0;
	int count=0;
	while(getline(myfile1,line))
	{
		std::stringstream  lineStream(line);
		std::string        cell;
		while(getline(lineStream,cell,','))
		{
			switch(colid)
			{
				case 0:
				(*edges)[count].id=atoi(cell.c_str());
				break;
				case 2:
				(*edges)[count].source=atoi(cell.c_str());
				break;
				case 3:
				(*edges)[count].target=atoi(cell.c_str());
				break;
				case 1:
				(*edges)[count].cost=atof(cell.c_str());
				break;
				case 4:
				(*edges)[count].revcost=atof(cell.c_str());
				break;
			}
			colid++;
			colid=colid%5;
		}
		count++;
		//cout<< line << endl;
	}
	myfile.close();
	myfile1.close();
	return number_of_lines;
}

typedef adjacency_list<vecS, vecS, undirectedS, Vertex,Edge> G;
void check(Edge *edges,int num_vertices,int num_edges,int level)
{
	FILE *fp;
	string path="./contracted_ways";
	//path += boost::lexical_cast<std::string>(level);
	path+=".txt";
	fp=fopen(path.c_str(),"w+");
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,num_vertices);
	g.initialize_graph(edges,num_edges);
	//g.print_Degree_Vertices();
	//cout << "Contracting...." << endl;
	g.contract_to_level(level);
	int deg;
	//g.get_degree(144,deg);
	//cout << "degree of 144 is " << deg << endl;
	Edge *reduced=NULL;
	int final_edges=g.getreducedGraph(&reduced);
	//cout << "final_edges " << final_edges << endl;
	for (int i = 0; i < final_edges && reduced[i].id>0; ++i)
	{
		//cout << "id:- " << reduced[i].id << ", " << "source:- " << reduced[i].source << ", target:- " << reduced[i].target << endl;
		//fprintf(fp, "%d,%d,%d,%f\n"
		//			,reduced[i].id,reduced[i].source,reduced[i].target,reduced[i].cost);

	}
	//g.print_removed_vertices();
	//g.print_psuedo_edges();
	long int close_id;
	deque<Edge> ids;
	/*g.find_closest_vertex(5,close_id,ids);
	cout << "closest Vertex " << close_id << endl;
	while(!ids.empty())
	{
		Edge e=ids.front();
		cout << "id:- " << e.id << ", " << "source:- " << e.source << ", target:- " << e.target << endl;
		ids.pop_front();
	}*/
	//Edge **path=NULL;
	//int path_size=0;
	//g.dijkstra_on_contracted(6,10,path,path_size);
	//g.print_path(path,path_size);
}

int main(int argc, char const *argv[])
{
	Edge* edges;
	//int num_edges=18;
	int num_vertices=1251;
	//edges=(Edge*)malloc(sizeof(Edge)*num_edges);
	//string filename="/home/rohith/mystuff/labwork/3-1/codes/data/txt/berlin.txt";
	string filename="berlin.txt";
	cout << "file " << filename << endl;
	int num_edges=loadGraph(filename,&edges);
 	//check(edges,num_vertices,num_edges,0);
 	//check(edges,num_vertices,num_edges,1);
 	check(edges,num_vertices,num_edges,1);

	return 0;
}




