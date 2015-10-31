/*PGR

file: dijkstra_driver.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#ifndef SRC_WARSHALL_SRC_WARSHALL_DRIVER_H_
#define SRC_WARSHALL_SRC_WARSHALL_DRIVER_H_

#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int do_pgr_warshall(
    pgr_edge_t  *data_edges,  // array of id, source, target, cost, reverse_cost
    size_t total_tuples,     // size of data_edges
    bool directedFlag,        // to choose undirected or directed

    // return values
    Matrix_cell_t **ret_matrix,
    size_t *return_tuple_count,
    char ** err_msg); 

#ifdef __cplusplus
}
#endif


#endif  // SRC_WARSHALL_SRC_WARSHALL_DRIVER_H_
