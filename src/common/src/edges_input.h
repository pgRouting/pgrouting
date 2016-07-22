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

//@{

/*! @brief pgr_get_edges_no_id 

Currently used in:
allpairs

no_id_edges_sql_start

Description of the edges_sql query
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`
                                                   - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,
                                                   - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

no_id_edges_sql_end

@param[in] edges_sql
@param[out] edges
@param[out] total_edges
*/
void pgr_get_edges_no_id(
        char *edges_sql,
        pgr_edge_t **edges,
        size_t *total_edges);




/*! @brief pgr_get_edges 

For queries of the type:
~~~~{.c}
SELECT id, source, target, cost, [reverse_cost]
FROM edge_table;
~~~~

basic_edges_sql_start

Description of the edges_sql query
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`
                                                   - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,
                                                   - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

basic_edges_sql_end

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
