#ifndef SRC_KSP_SRC_KSPDRIVER_H_
#define SRC_KSP_SRC_KSPDRIVER_H_

#include <stdarg.h>
#include <stdio.h>

#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int  doKpaths(pgr_edge_t  * edges, long total_tuples,
                      long  start_vertex, long end_vertex,
                      int no_paths, bool has_reverse_cost, bool directed,
                      ksp_path_element_t **ksp_path, int *path_count,
                      char ** err_msg);

#ifdef __cplusplus
}
#endif

#endif  // SRC_KSP_SRC_KSPDRIVER_H_
