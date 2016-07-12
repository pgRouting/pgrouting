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

#pragma once

#include "./pgr_types.h"

/*! @name pgr_get_edges
 *
 *
 * 
 */
//@{

/*! @brief read edges where id is of no interest

Edges: where the id is not of interest

bigint source,
bigint target,
float8 cost,
float8 reverse_cost

Currently used in:
allpairs

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_no_id(
        char *sql,
        pgr_edge_t **edges,
        size_t *total_edges);

/*! @brief read edges 

Edges:

bigint id,
bigint source,
bigint target,
float8 cost
float8 reverse_cost

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges(
        char *sql,
        pgr_edge_t **edges,
        size_t *total_edges);

/*! @name pgr_get_edges_xy
 
  @brief read edges with additional (x,y) for source & target

  Edges with x, y vertices values:

  bigint id,
  bigint source,
  bigint target,
  float8 cost
  float8 reverse_cost
  float8 x1, y1   -- source
  float8 x2, y2   -- target
*/

/*! @brief normal graph

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_xy(
        char *sql,
        Pgr_edge_xy_t **edges,
        size_t *total_edges);

/*! @brief reversed graph (for many to 1)

@param[in] sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_xy_reversed(
        char *sql,
        Pgr_edge_xy_t **edges,
        size_t *total_edges);
