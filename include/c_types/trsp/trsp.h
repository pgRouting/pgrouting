/*PGR-GNU*****************************************************************

FILE: trsp.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/

#ifndef INCLUDE_C_TYPES_TRSP_TRSP_H_
#define INCLUDE_C_TYPES_TRSP_TRSP_H_

#define MAX_RULE_LENGTH 5


/* for int64_t */
#ifdef __cplusplus
#   include <cstdint>
typedef int64_t int64;
typedef double float8;
#else
#   include <stdint.h>
#endif

typedef struct edge {
    int64 id;
    int64 source;
    int64 target;
    float8 cost;
    float8 reverse_cost;
} edge_t;

typedef struct restrict_struct {
    int target_id;
    float8 to_cost;
    int via[MAX_RULE_LENGTH];
}
restrict_t;

typedef struct path_element {
    int64 vertex_id;
    int64 edge_id;
    float8 cost;
} path_element_tt;

#ifdef __cplusplus
extern "C" {
#endif

int trsp_edge_wrapper(
    edge_t *edges,
    size_t edge_count,
    restrict_t *restricts,
    size_t restrict_count,
    int64_t start_edge,
    double start_pos,
    int64_t end_edge,
    double end_pos,
    bool directed,
    bool has_reverse_cost,
    path_element_tt **path,
    size_t *path_count,
    char **err_msg
);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_C_TYPES_TRSP_TRSP_H_
