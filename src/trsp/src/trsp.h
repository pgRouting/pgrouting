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
 
#ifndef _TRSP_H
#define _TRSP_H

#define MAX_RULE_LENGTH 5


typedef struct edge
{
    long id;
    long source;
    long target;
    double cost;
    double reverse_cost;
} edge_t;

typedef struct restrict_struct
{
		int target_id;
		double to_cost;
        int via[MAX_RULE_LENGTH];
} 
restrict_t;

typedef struct path_element 
{
    long vertex_id;
    long edge_id;
    double cost;
} path_element_t;

#ifdef __cplusplus
extern "C" {
#endif

int trsp_node_wrapper(
			edge_t *edges, 
			uint32_t edge_count, 
			restrict_t *restricts, 
			uint32_t restrict_count,
			long start_vertex, 
			long end_vertex,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, 
            uint32_t *path_count, 
            char **err_msg
            );

int trsp_edge_wrapper(
			edge_t *edges, 
			uint32_t edge_count, 
			restrict_t *restricts, 
			uint32_t restrict_count,
			long start_edge, 
            double start_pos,
			long end_edge,
            double end_pos,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, 
            uint32_t *path_count, 
            char **err_msg
            );
#ifdef __cplusplus
}
#endif


#endif
