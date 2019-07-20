/*PGR-GNU*****************************************************************

FILE: trsp_types.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

#ifndef INCLUDE_C_TYPES_TRSP_TYPES_H_
#define INCLUDE_C_TYPES_TRSP_TYPES_H_
#pragma once

/* for int64_t */
#ifdef __cplusplus
#   include <cstdint>
#else
#   include <stdint.h>
#endif

typedef struct  {
    int64_t id;
    int64_t source;
    int64_t target;
    double cost;
    double reverse_cost;
} edge_t;

typedef struct restrict_struct {
    int target_id;
    double to_cost;
    int via[5];
}
restrict_t;

typedef struct path_element {
    int64_t vertex_id;
    int64_t edge_id;
    double cost;
} path_element_tt;

#endif  // INCLUDE_C_TYPES_TRSP_TYPES_H_
