/*PGR-GNU*****************************************************************
File: edges_input.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef INCLUDE_C_COMMON_EDGES_INPUT_H_
#define INCLUDE_C_COMMON_EDGES_INPUT_H_
#pragma once

/* for size-t */
#include <stddef.h>
#include "c_types/pgr_costFlow_t.h"
#include "c_types/pgr_flow_t.h"
#include "c_types/pgr_basic_edge_t.h"
#include "c_types/pgr_edge_t.h"
#include "c_types/pgr_edge_xy_t.h"


/*! @brief edges_sql without id parameter

~~~~{.c}
SELECT source, target, cost, [reverse_cost]
FROM edge_table;
~~~~

Currently used in: allpairs

@param[in] edges_sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_no_id(
        char *edges_sql,
        pgr_edge_t **edges,
        size_t *total_edges);



/*! @brief basic edge_sql

For queries of the type:
~~~~{.c}
SELECT id, source, target, cost, [reverse_cost]
FROM edge_table;
~~~~

@param[in] edges_sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges(
        char *edges_sql,
        pgr_edge_t **edges,
        size_t *total_edges);
void pgr_get_edges_reversed(
        char *edges_sql,
        pgr_edge_t **edges,
        size_t *total_edges);



/*! @brief Edges with x, y vertices values

For queries of the type:
~~~~{.c}
SELECT id, source, target, cost, [reverse_cost], x1, y1, x2, y2
FROM edge_table;
~~~~

@param[in] edges_sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_xy(
        char *edges_sql,
        Pgr_edge_xy_t **edges,
        size_t *total_edges);

/*! @brief for many to 1 on aStar

Used internally

Transforms queries of the type:
~~~~{.c}
SELECT id, source, target, cost, [reverse_cost], x1, y1, x2, y2
FROM edge_table;
~~~~

to
~~~~{.c}
SELECT id, target, source, cost, [reverse_cost], x1, y1, x2, y2
FROM edge_table;
~~~~

@param[in] edges_sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_xy_reversed(
        char *edges_sql,
        Pgr_edge_xy_t **edges,
        size_t *total_edges);


/*! @brief read edges for flow

Edges:

bigint id,
bigint source,
bigint target,
bigint capacity,
bigint reverse_capacity

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_flow_edges(
    char *sql,
    pgr_edge_t **edges,
    size_t *total_edges);

/*! @brief read edges for cost flow

Edges:

bigint id,
bigint source,
bigint target,
bigint capacity,
bigint reverse_capacity
float cost,
float reverse_cost

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_costFlow_edges(
    char *sql,
    pgr_costFlow_t **edges,
    size_t *total_edges);

/*! @brief read basic edges

For queries of the type:
~~~~{.c}
SELECT id, source, target, going, [coming]
FROM edge_table;
~~~~

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_basic_edges(
    char *sql,
    pgr_basic_edge_t **edges,
    size_t *total_edges);



#endif  // INCLUDE_C_COMMON_EDGES_INPUT_H_
