#ifndef CONTRACT_FUNCTION_H_
#define CONTRACT_FUNCTION_H_

#include "../../common/src/structs.h"

#ifdef __cplusplus
extern "C" {
#endif

int compute_contracted_graph(Edge *edges,int num_vertices,int num_edges,int level,Edge **final_edges);



#ifdef __cplusplus
}
#endif

#endif 