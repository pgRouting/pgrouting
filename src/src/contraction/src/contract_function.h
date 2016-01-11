#ifndef CONTRACT_FUNCTION_H_
#define CONTRACT_FUNCTION_H_

#include "../../common/src/structs.h"

#ifdef __cplusplus
extern "C" {
#endif

int fetch_contracted_graph(Edge *edges,int *initial_num_vertices,
							int *final_num_vertices,int initial_num_edges,
							int *final_num_edges,int level,Edge **final_edges,
							char **graphName,char **edgeString);


#ifdef __cplusplus
}
#endif

#endif 