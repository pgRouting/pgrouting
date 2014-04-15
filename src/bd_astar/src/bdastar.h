/*
 * Bi Directional A* Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
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
#ifndef _BDASTAR_H
#define _BDASTAR_H

#include "postgres.h"


typedef struct edge_astar 
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
} edge_astar_t;

typedef struct path_element 
{
    int vertex_id;
    int edge_id;
    float8 cost;
} path_element_t;


#ifdef __cplusplus
extern "C"
#endif

  int bdastar_wrapper(edge_astar_t *edges, unsigned int count, int maxnode,
                  int source_vertex_id, int target_vertex_id,
                  bool directed, bool has_reverse_cost,
                  path_element_t **path, int *path_count, char **err_msg);
#ifdef __cplusplus
extern "C"
#endif

#endif
