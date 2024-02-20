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

#ifndef INCLUDE_DRIVERS_TRSP_TRSP_CORE_H_
#define INCLUDE_DRIVERS_TRSP_TRSP_CORE_H_

#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
#else
#   include <stddef.h>
#   include <stdint.h>
#include <stdbool.h>
#endif

typedef struct restrict_t restrict_t;
typedef struct Edge_rt Edge_t;
typedef struct path_element_tt path_element_tt;

#ifdef __cplusplus
extern "C" {
#endif

int trsp_edge_wrapper(
    Edge_t *edges,
    size_t edge_count,
    restrict_t *restricts,
    size_t restrict_count,
    int64_t start_edge,
    double start_pos,
    int64_t end_edge,
    double end_pos,
    bool directed,
    bool has_reverse_cost,
    path_element_tt **path,
    size_t *path_count,
    char **err_msg
);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_TRSP_TRSP_CORE_H_
