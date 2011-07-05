/*
 * Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
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
 
#ifndef _TDSP_H
#define _TDSP_H

#include "postgres.h"
//#include "edge_wrapper.h"

typedef struct edge_columns_str
{
    int id;
    int source;
    int target;
    //double cost;
    //double reverse_cost;
    float8 cost;
    float8 reverse_cost;
} edge_columns_t;

typedef struct weight_map_element
{
		int edge_id;
		float8 start_time;
		float8 travel_time;
} 
weight_map_element_t;

typedef struct weight_map_columns_str
{
		int edge_id;
		int start_time;
		int travel_time;
} 
weight_map_columns_t;


typedef struct path_element 
{
    int vertex_id;
    int edge_id;
    float8 cost;
} path_element_t;

#ifdef __cplusplus
extern "C"
#endif
int tdsp_wrapper(

			edge_columns_t *edges, 
			unsigned int edge_count, 
			weight_map_element_t *weight_map_elements, 
			int weight_map_element_count,
			int start_vertex, 
			int end_vertex,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, 
            int *path_count, 
            char **err_msg

            );

#endif
