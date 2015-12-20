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
	string path="/home/rohith/mystuff/labwork/3-1/codes/data/txt/contracted_ways_";
	path += boost::lexical_cast<std::string>(level);
	path+=".txt";
	fp=fopen(path.c_str(),"w+");
	typedef Graph_Minimizer<G> Graph;
	Graph g(UNDIRECTED,num_vertices);
	g.initialize_graph(edges,num_edges);
	cout << "Contracting...." << endl;
	g.contract_to_level(level);
	Edge *reduced=NULL;
	int final_edges=g.getreducedGraph(&reduced);
	cout << "final_edges " << final_edges << endl;
	for (int i = 0; i < final_edges && reduced[i].id>0; ++i)
	{
		//cout << "id:- " << reduced[i].id << ", " << "source:- " << reduced[i].source << ", target:- " << reduced[i].target << endl;
		fprintf(fp, "%d,%d,%d,%f\n"
					,reduced[i].id,reduced[i].source,reduced[i].target,reduced[i].cost);

	}
//g.reduced_graph->print_graph();
}

int main(int argc, char const *argv[])
{
	Edge* edges;
	//int num_edges=18;
	int num_vertices=1251;
	//edges=(Edge*)malloc(sizeof(Edge)*num_edges);
	string filename="/home/rohith/mystuff/labwork/3-1/codes/data/txt/berlin.txt";
	//string filename="ways.txt";
	cout << "file " << filename << endl;
	int num_edges=loadGraph(filename,&edges);
 	check(edges,num_vertices,num_edges,0);
 	check(edges,num_vertices,num_edges,1);
 	check(edges,num_vertices,num_edges,2);
	return 0;
}




/*edges[0].id=0;
edges[0].source=1;
edges[0].target=2;
edges[0].cost=0.625;
edges[0].revcost=-1;

edges[1].id=1;
edges[1].source=2;
edges[1].target=3;
edges[1].cost=4;
edges[1].revcost=-1;

edges[2].id=1;
edges[2].source=3;
edges[2].target=4;
edges[2].cost=4;
edges[2].revcost=-1;

edges[3].id=1;
edges[3].source=4;
edges[3].target=9;
edges[3].cost=4;
edges[3].revcost=-1;

edges[4].id=1;
edges[4].source=9;
edges[4].target=12;
edges[4].cost=4;
edges[4].revcost=-1;

edges[5].id=1;
edges[5].source=12;
edges[5].target=11;
edges[5].cost=4;
edges[5].revcost=-1;

edges[6].id=1;
edges[6].source=11;
edges[6].target=10;
edges[6].cost=4;
edges[6].revcost=-1;

edges[7].id=1;
edges[7].source=10;
edges[7].target=5;
edges[7].cost=4;
edges[7].revcost=-1;

edges[8].id=1;
edges[8].source=5;
edges[8].target=2;
edges[8].cost=4;
edges[8].revcost=-1;

edges[9].source=17;
edges[9].target=16;
edges[9].cost=4;
edges[9].revcost=-1;

edges[10].id=1;
edges[10].source=6;
edges[10].target=3;
edges[10].cost=4;
edges[10].revcost=-1;

edges[11].id=1;
edges[11].source=10;
edges[11].target=13;
edges[11].cost=4;
edges[11].revcost=-1;

edges[12].id=1;
edges[12].source=14;
edges[12].target=15;
edges[12].cost=4;
edges[12].revcost=-1;

edges[13].id=1;
edges[13].source=7;
edges[13].target=8;
edges[13].cost=4;
edges[13].revcost=-1;

edges[14].id=1;
edges[14].source=8;
edges[14].target=5;
edges[14].cost=4;
edges[14].revcost=-1;

edges[15].id=1;
edges[15].source=5;
edges[15].target=6;
edges[15].cost=4;
edges[15].revcost=-1;

edges[16].id=1;
edges[16].source=6;
edges[16].target=9;
edges[16].cost=4;
edges[16].revcost=-1;

edges[17].id=1;
edges[17].source=11;
edges[17].target=6;
edges[17].cost=4;
edges[17].revcost=-1;*/