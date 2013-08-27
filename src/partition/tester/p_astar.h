#ifndef _P_ASTAR_H
#define _p_ASTAR_H

#include "postgres.h"




typedef struct edge_p_astar
{
	int id;
	int source;
	int target;                         
	int s_pid;                              /* this will have cordinates as well */
	int t_pid;
	float8 cost ;
	float8 reverse_cost;
	float8 s_x;
	float8 s_y;
	float8 t_x;
	float8 t_y;

}edge_p_astar_t ;


typedef struct path_element
{
	int vertex_id;
	int edge_id;                              /* to return the path, this is used in cpp code to stuff with the path parameters */
	float8 cost ;
} path_element_t;



edge_p_astar_t *
fetch_partition_edges(int vertex_id , int* ptuples );   /* this funtion returns pointer of the stuffed edges fetched using the sql queryy 
						 	inside the c function which takes a node_id as an argument and fetches all 
						        the edges from the edge table using the partition id of that node  */


int p_astar_wrapper(int source_vertex_id,int target_vertex_id,int s_pid ,int t_pid ,char **err_msg);  /* wrapper function which instantiate the solver class */ 




#endif
