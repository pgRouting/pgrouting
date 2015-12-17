/*PGR-GNU*****************************************************************
File: pgr_types.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#pragma once

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

typedef struct Routes_element
{
    int64_t route_id;
    int64_t path_id;
    int64_t path_seq;
    int64_t start_vid;
    int64_t end_vid;
    int64_t node;
    int64_t edge;
    float8 cost;
    float8 agg_cost;
    float8 route_agg_cost;
} Routes_t;


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

// Restrictions used in pgr_turnRestrictions

#define  MAX_RULE_LENGTH 5
typedef struct 
{
    int64_t target_id;
    float8 to_cost;
    int64_t via[MAX_RULE_LENGTH];
    char *via_path;
}
Restrict_t;

typedef struct {
    int64_t pid;
    int64_t edge_id;
    char side;  // 'r', 'l', 'b' (default is both)
    float8 fraction;
    int64_t vertex_id; // number is negative and is used for processing
} Point_on_edge_t;

// used for getting the data
typedef
enum {
    ANY_INTEGER,
    ANY_NUMERICAL,
    TEXT,
    CHAR
} expectType;

typedef
struct {
    int colNumber;
    int type;
    bool strict;
    char *name;
    expectType eType;

} Column_info_t;

// used in boost
struct boost_vertex_t {
    int64_t id;
};

struct boost_edge_t{
    int64_t id;
    float8 cost;
    int64_t source;
    int64_t target;
    bool first;  // originally was true (source, target) false (target, source)
};


enum graphType { UNDIRECTED= 0, DIRECTED};

