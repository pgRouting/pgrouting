#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "../../../common/src/structs.h"
using namespace std;

int edge_count;
char buff[1024];
Edge *path;
char *err_msg;
int path_count;
void loadGraph(string edgeFile,Edge **)
{

	unsigned int number_of_lines = 0;
    FILE *infile = fopen(edgeFile.c_str(), "r");
    int ch;
    while (EOF != (ch=getc(infile)))
        if ('\n' == ch)
            ++number_of_lines;

    printf("%u\n", number_of_lines);
	// Open file for reading
	freopen(edgeFile.c_str(), "rt", stdin);
	edge_count = 0;
	//vecEdges.clear();

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

		Edge tempEdge;
		tempEdge.id = atoi(vecToken[0].c_str());
		tempEdge.source = atoi(vecToken[1].c_str());
		tempEdge.target = atoi(vecToken[2].c_str());
		tempEdge.cost = atof(vecToken[3].c_str());
		tempEdge.revcost = atof(vecToken[4].c_str());
		/*tempEdge.s_x = atof(vecToken[5].c_str());
		tempEdge.s_y = atof(vecToken[6].c_str());
		tempEdge.t_x = atof(vecToken[7].c_str());
		tempEdge.t_y = atof(vecToken[8].c_str());
                 
		tempEdge.s_pid = atoi(vecToken[9].c_str());
		tempEdge.t_pid = atoi(vecToken[10].c_str());*/

	//	vecEdges.push_back(tempEdge);

	}

	fclose(stdin);
}