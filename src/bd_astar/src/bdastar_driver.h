/*PGR-GNU*****************************************************************
 *
 * Bi Directional A* Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.

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

#ifndef SRC_BD_ASTAR_SRC_BDASTAR_DRIVER_H_
#define SRC_BD_ASTAR_SRC_BDASTAR_DRIVER_H_
#pragma once

#include "stdlib.h"
#include "../../common/src/pgr_types.h"


#ifdef __cplusplus
extern "C" {
#endif

  int bdastar_wrapper(edge_astar_t *edges, size_t count, int maxnode,
                  int source_vertex_id, int target_vertex_id,
                  bool directed, bool has_reverse_cost,
                  path_element_t **path, size_t *path_count, char **err_msg);
#ifdef __cplusplus
}
#endif

#endif  // SRC_BD_ASTAR_SRC_BDASTAR_DRIVER_H_
