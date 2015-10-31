/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#ifndef PGR_TYPES_H
#define PGR_TYPES_H

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


typedef struct matrix_cell
{
    int64_t from_vid;
    int64_t to_vid;
    double cost;
} Matrix_cell_t;


typedef struct path_element2
{
    int64_t vertex_id;
    int64_t edge_id;
    float8 cost;
} path_element_t;

typedef struct path_element
{
    int seq;
    int64_t from;
    int64_t to;
    int64_t vertex;
    int64_t edge;
    float8 cost;
    float8 tot_cost;
} General_path_element_t;

typedef struct {
  int64_t id;
  int64_t source;
  int64_t target;
  float8 cost;
  float8 reverse_cost;
} pgr_edge_t;

typedef struct {
  int seq;
  int64_t source;
  int64_t edge;
  float8 cost;
} pgr_path_t;

struct boost_vertex_t {
  int64_t id;
};

struct boost_edge_t{
  int64_t id;
  float8 cost;
  int64_t source_id;
  int64_t target_id;
};

// used in kdijktra
#if 0
typedef struct 
{
   int64_t vertex_id_source;
   int64_t edge_id_source;
   int64_t vertex_id_target;
   int64_t edge_id_target;
   float8 cost;
   float8 totcost;
} dist_fromto_t;


typedef struct 
{
  int64_t vertex_id_source;
  int64_t edge_id_source;
  int64_t vertex_id_target;
  int64_t edge_id_target;
  float8 cost;
  char* the_way;
} path_fromto_t;
#endif

enum graphType { UNDIRECTED= 0, DIRECTED};

#endif // PGR_TYPES_H
