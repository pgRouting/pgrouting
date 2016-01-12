#ifndef STRUCTS_H_
#define STRUCTS_H_

//represents an edge
typedef struct
{
	int source,target,id,type;
	float cost, revcost;
	float s_x,s_y,t_x,t_y;
} Edge;

//represents an vertex
typedef struct
{
	int id;
	int degree;
	int contractions;
	float x;
	float y;
} Vertex;

//represents an edge
typedef struct
{
	int source,target,id;
	float cost;

} PathElement;

typedef struct 
{
	char *contracted_graph_name;
	char *contracted_graph_blob;
	char *removedVertices;
	char *removedEdges;
	char *psuedoEdges;
} pgr_contracted_blob;
//represents the type of graph
enum graphType { UNDIRECTED= 0, DIRECTED};

#endif