/*PGR-GNU*****************************************************************
File: structs.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

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
#ifndef SRC_CONTRACTION_SRC_STRUCTS_H_
#define SRC_CONTRACTION_SRC_STRUCTS_H_

#ifndef __cplusplus
#include "postgres.h"
#endif
#include <stdint.h>

#if 0
// represents an edge
typedef struct {
    int64_t source, target, id, type;
    float cost, reverse_cost;
    float s_x, s_y, t_x, t_y;
    bool first;
} Edge;

// represents an vertex
typedef struct {
    int64_t id;
    int64_t degree;
    int64_t contractions;
    float x;
    float y;
} Vertex;

// represents an edge
typedef struct {
    int64_t seq;
    int64_t source;
    int64_t target;
    float cost;
    float tot_cost;
} PathElement;
#endif

typedef struct {
    int64_t seq;
    int64_t id;
    char *type;
    int64_t source;
    int64_t target;
    double cost;
    int64_t *contracted_vertices;
    int contracted_vertices_size;
} pgr_contracted_blob;

// represents the type of graph
// already defined in postgres_connection.h
// enum graphType { UNDIRECTED= 0, DIRECTED};

#endif  // SRC_CONTRACTION_SRC_STRUCTS_H_
