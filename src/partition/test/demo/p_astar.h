#ifndef _P_ASTAR_H
#define _p_ASTAR_H

#include "postgres.h"


typedef struct edge_p_astar
{
	int id;
	int source;
	int target;
	int s_pid;
	int t_pid;
	float8 cost ;
}edge_p_astar_t ;




int p_astar_wrapper(edge_p_astar_t *edges,int source_vertex_id,int target_vertex_id); 


#endif
