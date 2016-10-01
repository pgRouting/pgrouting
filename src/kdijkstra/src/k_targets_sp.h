 /*
 * Bi Directional Shortest path algorithm for PostgreSQL
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef _TOMANYSP_H
#define _TOMANYSP_H

//#include "MyWrapper.h"

#include "postgres.h"

typedef struct edge
{
    int id;
    int source;
    int target;
    float8 cost;
    float8 reverse_cost;
} edge_t;


typedef struct 
{
    int vertex_id;
    int edge_id;
    float8 cost;
} path_element_t;

#define PGR_MERGE
#ifdef PGR_MERGE
typedef struct
{
    int seq;
    int id1;
    int id2;
    float8 cost;
} pgr_cost_t;

typedef struct
{
    int seq;
    int id1;
    int id2;
    int id3;
    float8 cost;
} pgr_cost3_t;
#endif

typedef struct 
{
	int vertex_id_source;
	int edge_id_source;
	int vertex_id_target;
	int edge_id_target;
	float8 cost;
} dist_fromto_t;


typedef struct 
{
	int vertex_id_source;
	int edge_id_source;
	int vertex_id_target;
	int edge_id_target;
	float8 cost;
	char* the_way;
} path_fromto_t;

#ifdef __cplusplus
extern "C" {
#endif

int onetomany_dijkstra_boostdist(
        edge_t *edges, unsigned int count, int start_vertex,
        int *end_vertices, int nb_targets,
	    bool directed, bool has_reverse_cost,
#ifdef PGR_MERGE
	    pgr_cost_t **dists,
#else
	    dist_fromto_t **dists,
#endif
        char **err_msg );
    
  

int onetomany_dijkstra_boostpath(
        edge_t *edges, unsigned int count, int start_vertex,
        int *end_vertices, int nb_targets,
	    bool directed, bool has_reverse_cost,
#ifdef PGR_MERGE
	    pgr_cost3_t **dists,
        int *path_count,
#else
	    path_fromto_t **pathdists,
#endif
        char **err_msg );

#ifdef __cplusplus
}
#endif

#endif




