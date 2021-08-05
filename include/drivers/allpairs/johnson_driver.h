/*PGR-GNU*****************************************************************
File: johnson_driver.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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
#ifndef INCLUDE_DRIVERS_ALLPAIRS_JOHNSON_DRIVER_H_
#define INCLUDE_DRIVERS_ALLPAIRS_JOHNSON_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
using Edge_t = struct Edge_t;
using Matrix_cell_t = struct Matrix_cell_t;
#else
#   include <stddef.h>
typedef struct Edge_t Edge_t;
typedef struct Matrix_cell_t Matrix_cell_t;
#endif



#ifdef __cplusplus
extern "C" {
#endif

//  CREATE OR REPLACE FUNCTION pgr_johnson(edges_sql TEXT, directed BOOLEAN,
void
do_pgr_johnson(
        Edge_t  *data_edges,
        size_t total_tuples,
        bool directed,
        Matrix_cell_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_ALLPAIRS_JOHNSON_DRIVER_H_
