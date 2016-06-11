/*PGR-GNU*****************************************************************
File: contractGraph_driver.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
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

#ifndef SRC_CONTRACTION_SRC_CONTRACTGRAPH_DRIVER_H_
#define SRC_CONTRACTION_SRC_CONTRACTGRAPH_DRIVER_H_

#include "./../../common/src/pgr_types.h"
#include "./structs.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      edges_sql TEXT,
    level BIGINT,
    directed BOOLEAN DEFAULT true
     ********************************************************/
    void
        do_pgr_contractGraph(
               pgr_edge_t  *data_edges,
        size_t total_tuples,
        int64_t *forbidden_vertices,
        size_t size_forbidden_vertices,
        int64_t *contraction_order,
        size_t size_contraction_order,
        int64_t max_cycles,
        bool directed,
        pgr_contracted_blob **return_tuples,
        size_t *return_count,
        char ** err_msg);
    int is_valid_contraction(int number);

#ifdef __cplusplus
}
#endif

#endif  // SRC_CONTRACTION_SRC_CONTRACTGRAPH_DRIVER_H_
