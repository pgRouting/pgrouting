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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
 
#ifndef _TRSP_H
#define _TRSP_H

#define MAX_RULE_LENGTH 5

#include "postgres.h"

typedef struct edge
{
    int id;
    int source;
    int target;
    float8 cost;
    float8 reverse_cost;
} edge_t;

typedef struct restrict_struct
{
		int target_id;
		float8 to_cost;
        int via[MAX_RULE_LENGTH];
} 
restrict_t;

typedef struct path_element 
{
    int vertex_id;
    int edge_id;
    float8 cost;
} path_element_t;

#ifdef __cplusplus
extern "C"
#endif

int trsp_wrapper(
			edge_t *edges, 
			unsigned int edge_count, 
			restrict_t *restricts, 
			int restrict_count,
			int start_vertex, 
			int end_vertex,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, 
            int *path_count, 
            char **err_msg
            );

#endif
