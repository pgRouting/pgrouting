#ifndef DIJKSTRA_FUNCTION_H_
#define DIJKSTRA_FUNCTION_H_

#include "../../common/structs.h"

#ifdef __cplusplus
extern "C" {
#endif

int compute_dijkstra(int source,int target,Edge *edges,int num_vertices,int num_edges,Edge **path);


#ifdef __cplusplus
}
#endif

#endif 