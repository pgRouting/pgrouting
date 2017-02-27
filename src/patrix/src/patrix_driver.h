/*PGR-GNU*****************************************************************
File: patrix_driver.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Patrick Morales
Mail: 

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

#ifndef SRC_PATRIX_SRC_PATRIX_DRIVER_H_
#define SRC_PATRIX_SRC_PATRIX_DRIVER_H_

#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      edges_sql TEXT,
    start_pid BIGINT,
    end_pids ANYARRAY,
    directed BOOLEAN DEFAULT true,
     ********************************************************/

    // Code standard:
    // Pass the arrays and the sizes on the same line
    void do_pgr_patrix(
    		pgr_edge_t_patrix *data_edges, size_t total_edges,
            int64_t start_vid,
            int64_t  *end_vidsArr, size_t size_end_vidsArr,
            bool directed,
            General_path_element_t **return_tuples, size_t *return_count,
            char ** log_msg,
            char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // SRC_PATRIX_SRC_PATRIX_DRIVER_H_
