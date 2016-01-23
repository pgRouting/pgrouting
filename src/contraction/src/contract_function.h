#ifndef CONTRACT_FUNCTION_H_
#define CONTRACT_FUNCTION_H_

#include "./structs.h"

#ifdef __cplusplus
extern "C" {
#endif

int fetch_contracted_graph(Edge *edges,int *initial_num_vertices,
							int *final_num_vertices,int initial_num_edges,
							int *final_num_edges,int level,char **graphName,
							char **edgeString,char **psuedoEString,
							char **removedVString,char **removedEString);


#ifdef __cplusplus
}
#endif

#endif 