/*
 * Traveling Salesman Problem solution algorithm for PostgreSQL
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
 
#define _TSP_H
#define MAX_TOWNS 40

#include "postgres.h"
#include "dijkstra.h" 

typedef struct point
{
  int id;
  float8 x;
  float8 y;
} point_t;

#ifdef __cplusplus
extern "C"
{
#endif
  int find_tsp_solution(int num, float dist[MAX_TOWNS][MAX_TOWNS], 
			int p_ids[MAX_TOWNS], int source, float* fit, 
			char* err_msg);
#ifdef __cplusplus
}
#endif
