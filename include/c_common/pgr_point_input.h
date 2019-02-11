/*PGR-GNU*****************************************************************
File: pgr_point_input.h

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

#ifndef INCLUDE_C_COMMON_PGR_POINT_INPUT_H_
#define INCLUDE_C_COMMON_PGR_POINT_INPUT_H_
#pragma once

/* for size-t */
#include <stddef.h>

#include "c_types/pgr_point_t.h"

/*! @brief pgr_point_input

For queries of the type:
~~~~{.c}
SELECT x, y
FROM ...
~~~~

@param[in] points_sql
@param[out] points
@param[out] pointsTotal
*/
void pgr_point_input(
        char *points_sql,
        Pgr_point_t **points,
        size_t *pointsTotal);



#endif  // INCLUDE_C_COMMON_PGR_POINT_INPUT_H_
