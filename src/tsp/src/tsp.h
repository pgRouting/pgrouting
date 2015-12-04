/*PGR-GNU*****************************************************************

 * Traveling Salesman Problem solution algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
 * Copyright (c) 2013 Stephen Woodbridge, iMaptools.com.

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
 
#define _TSP_H

// define the type of object for the distance matrix
#define DTYPE double

#include "postgres.h"
#include "../../common/src/pgr_types.h" 

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
  int find_tsp_solution(int num, DTYPE *dist, int *p_ids, int source, int end, DTYPE *fit, char* err_msg);
#ifdef __cplusplus
}
#endif
