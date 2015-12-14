#ifndef STRUCTS_H_
#define STRUCTS_H_

//represents an edge
typedef struct
{
	int source,target,id,type;
	float cost, revcost;
} Edge;

//represents an vertex
typedef struct
{
	int id;
	int degree;
	int contractions;
} Vertex;

//represents an edge
typedef struct
{
	int source,target,id;
	float cost;

} PathElement;

//represents the type of graph
enum graphType { UNDIRECTED= 0, DIRECTED};

#endif