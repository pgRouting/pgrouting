/*PGR-GNU*****************************************************************
File: points_input.h

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

#ifndef SRC_COMMON_SRC_POINTS_INPUT_H_
#define SRC_COMMON_SRC_POINTS_INPUT_H_

#pragma once

#include "./pgr_types.h"

/*! @brief pgr_get_points 

For queries of the type:
~~~~{.c}
SELECT pid, edge_id, fraction, [side]
FROM edge_table;
~~~~

@param[in] points_sql
@param[out] points
@param[out] total_points
*/
void pgr_get_points(
        char *points_sql,
        Point_on_edge_t **points,
        size_t *total_points);


/* ******************** FOR USERS DOCUMENTATION

points_sql_start

Description of the Points SQL query
...............................................................................

:points_sql: an SQL query, which should return a set of rows with the following columns:

============ ================= =================================================
Column            Type              Description
============ ================= =================================================
**pid**      ``ANY-INTEGER``   (optional) Identifier of the point.
                                 - If column present, it can not be NULL. 
                                 - If column not present, a sequential identifier will be given automatically.

**edge_id**  ``ANY-INTEGER``   Identifier of the "closest" edge to the point.
**fraction** ``ANY-NUMERICAL`` Value in <0,1> that indicates the relative postition from the first end point of the edge.
**side**     ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the point is:
                                 - In the right, left of the edge or
                                 - If it doesn't matter with 'b' or NULL.
                                 - If column not present 'b' is considered.
============ ================= =================================================

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

points_sql_end

*/


#endif  // SRC_COMMON_SRC_POINTS_INPUT_H_
