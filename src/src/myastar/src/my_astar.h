
#ifndef MY_ASTAR_H
#define MY_ASTAR_H

#include <unistd.h>
#include "postgres.h"
#include "../../common/src/structs.h"


#ifdef __cplusplus
extern "C"
{
#endif
  int compute_astar(int64_t source,int64_t target,Edge *edges,int64_t num_vertices,int64_t num_edges,Edge **path);
#ifdef __cplusplus
}
#endif

#endif
