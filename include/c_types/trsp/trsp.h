/*PGR-GNU*****************************************************************

FILE: trsp.h

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

#ifndef INCLUDE_C_TYPES_TRSP_TRSP_H_
#define INCLUDE_C_TYPES_TRSP_TRSP_H_

#define MAX_RULE_LENGTH 5


/* for int64_t */
#ifdef __cplusplus
#   include <cstdint>
typedef int64_t int64;
#else
#   include <stdint.h>
#endif

struct restrict_t {
    int target_id;
    double to_cost;
    int via[MAX_RULE_LENGTH];
};

struct path_element_tt {
    int64_t vertex_id;
    int64_t edge_id;
    double cost;
};


#endif  // INCLUDE_C_TYPES_TRSP_TRSP_H_
