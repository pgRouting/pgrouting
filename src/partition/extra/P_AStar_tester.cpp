


# include "P_AStar.h"
#include "utils.h"
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<string.h>
//#include<cstring>



std::vector<edge_p_astar_t> vecEdges;

int edge_count;
char buff[1024];
path_element_t *path;
char *err_msg;
int path_count;

void loadGraph(std::string edgeFile)

{

	// Open file for reading
	freopen(edgeFile.c_str(), "rt", stdin);
	edge_count = 0;
	vecEdges.clear();

	// Read line by line edge info
	while(gets(buff))
	{
		if(strlen(buff) == 0)
			break;
		edge_count++;
		StringTokenizer token;
		// tokenize using comma
		token.parse(buff, ",");
		std::vector<std::string> vecToken;
		vecToken.clear();
		token.getTokens(vecToken);



		// There should be exactly 9 values: edge_id, start_node_id, end_node_id, 

		// start_node_longitude, start_node_latitude, end_node_longitude, end_node_latitude, cost, reverse_cost
		if(vecToken.size() < 5)

			fprintf(stderr, "Error in %d edge\n", edge_count);


		// Populate Edge_t structure

		edge_p_astar_t tempEdge;
		tempEdge.id = atoi(vecToken[0].c_str());
		tempEdge.source = atoi(vecToken[1].c_str());
		tempEdge.target = atoi(vecToken[2].c_str());
		tempEdge.cost = atof(vecToken[3].c_str());
		tempEdge.reverse_cost = atof(vecToken[4].c_str());
		tempEdge.s_x = atof(vecToken[5].c_str());
		tempEdge.s_y = atof(vecToken[6].c_str());
		tempEdge.t_x = atof(vecToken[7].c_str());
		tempEdge.t_y = atof(vecToken[8].c_str());
                 
		tempEdge.s_pid = atoi(vecToken[9].c_str());
		tempEdge.t_pid = atoi(vecToken[10].c_str());



		vecEdges.push_back(tempEdge);

	}

	fclose(stdin);
}



edge_p_astar_t *
fetch_partition_edges(int pid, int* ptuples)
{


	edge_p_astar_t *edges = NULL;
	int i;
	int cnt=0;
	for(i=0;i<edge_count;i++)
	{
		if(vecEdges[i].s_pid==pid || vecEdges[i].t_pid==pid)
		{
			cnt++;
			if (!edges)
				edges = (edge_p_astar_t *)malloc(cnt * sizeof(edge_p_astar_t));
			else
				edges = (edge_p_astar_t *)realloc(edges, cnt * sizeof(edge_p_astar_t));

			edges[cnt-1]=vecEdges[i];
		}

	}

	*ptuples=cnt;
	return edges;

}

void write_result(std::string fileName, int res)

{
	int i;
	freopen(fileName.c_str(), "wt", stdout);
	if(res < 0)
		printf("%s\n", err_msg);
	else
	{
		for(i = 0; i < path_count; i++)
		{
			printf("%d\t|%d\t|%.6lf\n", path[i].vertex_id, path[i].edge_id, path[i].cost);
		}
	}

	fclose(stdout);
}

//using namespace std;
int main()
{
	int i;
	double cl;

	// The final output will be written in the outFile and the initial input will be read from inFile
	std::string outFile = "output.txt";
	std::string inFile = "input.txt";



	// Create the output file
	FILE *fpout = fopen(outFile.c_str(), "wt");
	fclose(fpout);

	// Open the input file
	FILE *fpin = fopen(inFile.c_str(), "rt");


	while(fgets(buff, 1000, fpin))
	{

		// No data

		if(strlen(buff) == 0)

			continue;
		StringTokenizer token;

		// tokeniize using space
		token.parse(buff, " \n\r");

		std::vector<std::string> vecToken;
		token.getTokens(vecToken);
		int totParam = vecToken.size();

		// Not enough parameters

		if(totParam < 4)
			continue;


		// First token is the graph file name
		std::string graphFile = vecToken[0];

		// 2nd and 3rd tokens are start and end node id , 4th and 5th are partion id's respectively
		int startNode = atoi(vecToken[1].c_str());
		int endNode = atoi(vecToken[2].c_str());
		
		int s_pid = atoi(vecToken[3].c_str());
		int t_pid = atoi(vecToken[4].c_str());

		// 6th Token is the result file for this query

		std::string pathFile = vecToken[5];
		int ind = pathFile.length() - 1;
		while(pathFile[ind] < 32)
		{
			pathFile[ind] = '\0';
			ind--;
		}



		// Load edge information from graph file
		loadGraph(graphFile);

		// Use partition  AStar to get the route/	
		P_AStar gdef;
		cl = clock();

	   //     int res = gdef.p_astar( startNode, endNode,s_pid ,t_pid , &path, &path_count, &err_msg);
            //    printf("Yes it got %d\n ",res);
		cl = clock() - cl;
		// Write the route in the result file

//		write_result(pathFile, res);
		free(path);
	}
	return 0;
}


