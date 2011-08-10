/*
 * Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2011 Jay Mahadeokar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
rm ap * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
 
#ifndef _APSP_H
#define _APSP_H

#include "postgres.h"

typedef struct edge 
{
    int id;
    int source;
    int target;
    float8 cost;
    float8 reverse_cost;
} edge_t;

typedef struct apsp_element 
{
    int src_vertex_id;
    int dest_vertex_id;
    float8 cost;
    
} apsp_element_t;

//TODO modify
#ifdef __cplusplus
extern "C"
#endif
int 
boost_apsp(edge_t *edges, unsigned int edge_count, const int node_count,
	       bool directed, bool has_reverse_cost,
	       apsp_element_t **pair, int *pair_count, char **err_msg);

#endif
