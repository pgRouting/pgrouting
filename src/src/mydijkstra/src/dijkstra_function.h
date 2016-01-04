#ifndef DIJKSTRA_FUNCTION_H_
#define DIJKSTRA_FUNCTION_H_

#include "../../common/src/structs.h"

#ifdef __cplusplus
extern "C" {
#endif

int compute_dijkstra(int64_t source,int64_t target,Edge *edges,int64_t num_vertices,int64_t num_edges,Edge **path);


#ifdef __cplusplus
}
#endif

#endif 