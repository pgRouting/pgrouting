/*PGR-GNU*****************************************************************

 * Copyright (c) 2011 Stephen Woodbridge

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
 
#ifndef _TRSP_H
#define _TRSP_H

#define MAX_RULE_LENGTH 5

#include "../../common/src/pgr_types.h"
typedef pgr_edge_t edge_t;
typedef Restrict_t restrict_t;

#if 0
typedef struct restrict_struct
{
		int target_id;
		float8 to_cost;
        int via[MAX_RULE_LENGTH];
} 
restrict_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int trsp_node_wrapper(
			edge_t *edges,         size_t edge_count, 
			restrict_t *restricts, size_t restrict_count,
			int start_vertex, 
			int end_vertex,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, size_t *path_count, 
            char **err_msg
            );


int trsp_edge_wrapper(
			edge_t *edges, size_t edge_count, 
			restrict_t *restricts, size_t restrict_count,
			int start_edge, 
            double start_pos,
			int end_edge,
            double end_pos,
		    bool directed, 
		    bool has_reverse_cost,
            path_element_t **path, size_t *path_count, 
            char **err_msg
            );
#ifdef __cplusplus
}
#endif

#endif
