/*PGR-MIT*****************************************************************

* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/

#include "BiDirAStar.h"
#include "utils.h"
#include<math.h>
#include<time.h>
#include<stdio.h>
#include<string.h>

#define EPS 1e-8


std::vector<edge_astar_t> vecEdges;
edge_astar_t *edges;
int edge_count, maxNode;
char buff[1024];
path_element_t *path;
char *err_msg;
int path_count;
int kase;

/*
	This method load the edge information from a csv file and put them in the edge_t array that can be passed to the algorithm to find route
*/

void loadGraph(std::string edgeFile)
{
	// Open file for reading
	freopen(edgeFile.c_str(), "rt", stdin);

	edge_count = 0;
	vecEdges.clear();
	maxNode = -1;

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
		edge_astar_t tempEdge;
		tempEdge.id = atoi(vecToken[0].c_str());
		tempEdge.source = atoi(vecToken[1].c_str());
		tempEdge.target = atoi(vecToken[2].c_str());
		tempEdge.cost = atof(vecToken[3].c_str());
		tempEdge.reverse_cost = atof(vecToken[4].c_str());
		tempEdge.s_x = atof(vecToken[5].c_str());
		tempEdge.s_y = atof(vecToken[6].c_str());
		tempEdge.t_x = atof(vecToken[7].c_str());
		tempEdge.t_y = atof(vecToken[8].c_str());

		// Update max_node_id
		if(tempEdge.source > maxNode)
			maxNode = tempEdge.source;
		if(tempEdge.target > maxNode)
			maxNode = tempEdge.target;
		
		vecEdges.push_back(tempEdge);
	}

	edges = new edge_astar_t[edge_count];
	int i;

	for(i = 0; i < edge_count; i++)
	{
		edges[i] = vecEdges[i];
	}
	fclose(stdin);
}
/*
	Write the route in the path file
*/
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

/*
	Match output with answer file and write result in the result file
*/

void match(std::string fileName1, std::string fileName2, std::string outFile, double ttime)
{
	// Open the first file
	freopen(fileName1.c_str(), "rt", stdin);

	// Initialization
	std::vector<int> nodeList1;
	nodeList1.clear();
	double totCost1, totCost2;
	int nid, eid;
	double cost;
	totCost1 = 0.0;
	
	// Read paths push node_id, edge_id inthe vector and update total cost
	while(gets(buff))
	{
		if(sscanf(buff, "%d |%d |%lf", &nid, &eid, &cost) != 3)
		{
			totCost1 = -1;
			break;
		}
		nodeList1.push_back(nid);
		nodeList1.push_back(eid);
		totCost1 += cost;
	}
	fclose(stdin);
	bool flag = true;
	// Open the second file
	freopen(fileName2.c_str(), "rt", stdin);
	totCost2 = 0.0;
	int pos = 0;

	// Read paths compare with previously constructed vector of node-id, edge_id and updte total cost
	while(gets(buff))
	{
		if(sscanf(buff, "%d |%d |%lf", &nid, &eid, &cost) != 3)
		{
			totCost2 = -1;
			break;
		}
		if(pos >= nodeList1.size() || nodeList1[pos] != nid)
		{
			flag = false;
		}
		pos++;
		if(pos >= nodeList1.size() || nodeList1[pos] != eid)
		{
			flag = false;
		}
		pos++;
		totCost2 += cost;
	}
	fclose(stdin);

	// Open output file to write
	freopen(outFile.c_str(), "a+", stdout);
	printf("Case %d: ", kase);

	// Both costs matches
	if(fabs(totCost1 - totCost2) < EPS)
	{
		// Path also matches
		if(flag == true)
		{
			printf("Perfect Match!!!\n");
		}
		else	// path mismatch
		{
			printf("Cost same, but path differs!!!\n");
		}
	}
	else // Cost mispatch
	{
		printf("Cost differs, %s costs %lf and %s costs %lf\n", fileName1.c_str(), totCost1, fileName2.c_str(), totCost2);
	}
	printf("Query time: %lf sec\n\n", ttime);
	fclose(stdout);
}

int main()
{
	int i;
	double cl;
	kase = 1;

	// The final output will be written in the outFile and the initial input will be read from inFile
	std::string outFile = "output.txt";
	std::string inFile = "input.txt";

	// Create the output file
	FILE *fpout = fopen(outFile.c_str(), "wt");
	fclose(fpout);

	// Open the input file
	FILE *fpin = fopen(inFile.c_str(), "rt");

	// Reading each of the cases, There may be two types of cases, with 4 parameters, with 5 parameters
	// There may also be comments that starts with #
	while(fgets(buff, 1000, fpin))
	{
		// No data
		if(strlen(buff) == 0)
			continue;
		// Comment
		if(buff[0] == '#')
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

		// 2nd and 3rd tokens are start and end node id respectively
		int startNode = atoi(vecToken[1].c_str());
		int endNode = atoi(vecToken[2].c_str());

		// 4th Token is the result file for this query
		std::string pathFile = vecToken[3];
		int ind = pathFile.length() - 1;
		while(pathFile[ind] < 32)
		{
			pathFile[ind] = '\0';
			ind--;
		}

		// Load edge information from graph file
		loadGraph(graphFile);

		// Use bidirectional AStar to get the route
		BiDirAStar gdef;
		cl = clock();
		int res = gdef.bidir_astar(edges, edge_count, maxNode, startNode, endNode, &path, &path_count, &err_msg);
		cl = clock() - cl;

		// Write the route in the result file
		write_result(pathFile, res);
		
		// There is an answer file
		if(totParam > 4)
		{
			std::string ansFile = vecToken[4];
			ind = ansFile.length() - 1;
			while(ansFile[ind] < 32)
			{
				ansFile[ind] = '\0';
				ind--;
			}
			// Match and write result in the final output file
			match(pathFile, ansFile, outFile, cl / CLOCKS_PER_SEC);
		}
		else
		{
			// Provide information that the route is generated in path file.
			fpout = fopen(outFile.c_str(), "a+");
			fprintf(fpout, "Case %d: Path Written to file %s", kase, pathFile.c_str());
			fprintf(fpout, "Query Time: %lf sec\n\n", cl / CLOCKS_PER_SEC);
			fclose(fpout);
		}
		kase++;
		free(path);
		delete [] edges;
	}
	return 0;
}
