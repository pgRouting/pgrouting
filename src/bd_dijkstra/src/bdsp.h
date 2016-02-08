/*PGR-GNU*****************************************************************

 * Bi Directional Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
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
#ifndef _BDSP_H
#define _BDSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../common/src/pgr_types.h"
#include "postgres.h"

typedef pgr_edge_t edge_t;


int bidirsp_wrapper( edge_t *edges, 
			size_t edge_count,
			int64_t maxNode,
			int64_t start_vertex, 
			int64_t end_vertex,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, 
            size_t *path_count, 
            char **err_msg
            );

#ifdef __cplusplus
}
#endif

#endif  // _BDSP_H


