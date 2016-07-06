#ifndef STRUCTS_H_
#define STRUCTS_H_

//represents an edge
typedef struct
{
	int64_t source,target,id,type;
	float cost, reverse_cost;
	float s_x,s_y,t_x,t_y;
	bool first;
} Edge;

//represents an vertex
typedef struct
{
	int64_t id;
	int64_t degree;
	int64_t contractions;
	float x;
	float y;
} Vertex;

//represents an edge
typedef struct
{
	int64_t seq;
    int64_t source;
    int64_t target;
    float cost;
    float tot_cost;

} PathElement;

typedef struct 
{
	int64_t seq;
	int64_t id;
	char *type;
	int64_t source;
	int64_t target;
	double cost;
	char *contracted_vertices;
} pgr_contracted_blob;

//represents the type of graph
// already defined in postgres_connection.h
//enum graphType { UNDIRECTED= 0, DIRECTED};

#endif