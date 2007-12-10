/*
 * Shooting* shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2007 Anton A. Patrushev, Orkney, Inc.
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

#define _SHOOTING_STAR_H
#define MAX_RULE_LENGTH 5

#define MAX_NODES 1000000
#define MAX_COST  100000

#include "postgres.h"

typedef struct edge_shooting_star 
{
  int id;
  int source;
  int target;
  float8 cost;
  float8 reverse_cost;
  float8 s_x;
  float8 s_y;
  float8 t_x;
  float8 t_y;
  float8 to_cost;
  int through_id[MAX_RULE_LENGTH];
  int rule[MAX_RULE_LENGTH];
} edge_shooting_star_t;

//I had to redeclare this. I couldn't include dijkstra.h 'cause
//struct edge conflicts with function adjacency_list::edge()
typedef struct path_element
{
  int vertex_id;
  int edge_id;
  float8 cost;
} path_element_t;

#ifdef __cplusplus
extern "C"
{
#endif
  int boost_shooting_star(edge_shooting_star_t *edges, unsigned int count, 
                  int source_edge_id, int target_edge_id,
                  bool directed, bool has_reverse_cost,
                  path_element_t **path, int *path_count, char **err_msg,
		  int e_max_id);
#ifdef __cplusplus
}
#endif
