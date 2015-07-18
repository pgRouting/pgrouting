/*
 * KShortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2012 Dave Potts
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
 
#ifndef _KSP_H
#define _KSP_H

#define TUPLIMIT 1000
#define PATH_ALLOC_TOTAL 5

#if defined(_MSC_VER) && _MSC_VER < 1600
#define ELOG_H
#endif
#include "postgres.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct ksp_edge 
{
    int id;
    int source;
    int target;
    float8 cost;
    float8 reverse_cost;
} ksp_edge_t;

typedef struct ksp_path_element 
{
    int route_id;
    int vertex_id;
    int edge_id;
    float8 cost;
} ksp_path_element_t;

typedef struct ksp_edge_columns 
{
  int id;
  int source;
  int target;
  float8  cost;
  float8  reverse_cost;
} ksp_edge_columns_t;


int compute_kshortest_path(char* sql, int start_vertex, 
                                 int end_vertex, int no_paths, 
                                 bool has_reverse_cost, 
                                 ksp_path_element_t **path, int *ksp_path_count) ;

ksp_path_element_t * get_ksp_memory(int,ksp_path_element_t *path);
/*int ksp_finish(int code, int ret);*/

#ifdef _MSC_VER
  void pgr_dbg(const char* format, ...);
#endif // _MSC_VER
#ifdef __cplusplus
}
#endif

#endif // _KSP_H
