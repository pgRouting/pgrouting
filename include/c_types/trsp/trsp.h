/*
 * Shortest path with turn restrictions algorithm for PostgreSQL
 *
 * Copyright (c) 2011 Stephen Woodbridge
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef INCLUDE_C_TYPES_TRSP_TRSP_H_
#define INCLUDE_C_TYPES_TRSP_TRSP_H_

#define MAX_RULE_LENGTH 5

#include "postgres.h"
#include <stdint.h>


typedef struct edge {
    long id;
    long source;
    long target;
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
    long vertex_id;
    long edge_id;
    float8 cost;
} path_element_tt;

#ifdef __cplusplus
extern "C" {
#endif

int trsp_node_wrapper(
    edge_t *edges,
    size_t edge_count,
    restrict_t *restricts,
    size_t restrict_count,
    int64_t start_vertex,
    int64_t end_vertex,
    bool directed,
    bool has_reverse_cost,
    path_element_tt **path,
    size_t *path_count,
    char **err_msg
);

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
