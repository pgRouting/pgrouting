#include <vector>

#define DEBUG 0

using namespace std;

//-------------------------------------------------------------------------
//A Edge structure, mainly to map edge_id with source and target

typedef struct edge_columns_str
{
    int id;
    int source;
    int target;
    double cost;
    double reverse_cost;
} edge_columns_t;

class edge_wrapper
{
	vector<edge_columns_t> edges;
	
	public:
	edge_wrapper(){};
	edge_wrapper(edge_columns_t * edges, int size)
	{
		this->edges.assign(edges, edges + size);
	}
	
	void insert(edge_columns_t e)
	{
		edges.push_back(e);
	}
	
	int get_source(int edge_id)
	{
		vector<edge_columns_t>::iterator it;
		for( it = edges.begin() ; it!= edges.end() ; it++)
			if(it->id == edge_id)
				return it->source;
		return -1;

	}
	
	int get_target(int edge_id)
	{
		vector<edge_columns_t>::iterator it;
		for( it = edges.begin() ; it!= edges.end() ; it++)
			if(it->id == edge_id)
				return it->target;
		return -1;

	}
	
	int get_edge_id(int source_id, int target_id)
	{
		vector<edge_columns_t>::iterator it;
		for( it = edges.begin() ; it!= edges.end() ; it++)
			if(it->source == source_id && it->target == target_id)
				return it->id;
		return -1;

	}
	
	edge_columns_t get_edge(int index)
	{
		return edges[index];
	}
};
