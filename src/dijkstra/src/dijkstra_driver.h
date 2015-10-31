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

#ifndef SRC_DIJKSTRA_SRC_DIJKSTRA_DRIVER_H_
#define SRC_DIJKSTRA_SRC_DIJKSTRA_DRIVER_H_

#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int  do_pgr_dijkstra_many_to_many(pgr_edge_t  *data_edges, int64_t total_tuples,
        int64_t  *start_vertex, int s_len, int64_t  *end_vertex, int e_len,
        bool has_reverse_cost, bool directedFlag,
        General_path_element_t **ret_path, size_t *path_count,
        char ** err_msg);

int  do_pgr_dijkstra_many_to_1(pgr_edge_t  *data_edges, int64_t total_tuples,
        int64_t  *start_vertex, int s_len, int64_t end_vertex,
        bool has_reverse_cost, bool directedFlag,
        General_path_element_t **ret_path, size_t *path_count,
        char ** err_msg);

int  do_pgr_dijkstra_1_to_many(pgr_edge_t  *data_edges, int64_t total_tuples,
        int64_t  start_vertex, int64_t  *end_vertex, int n_end,
        bool has_reverse_cost, bool directedFlag,
        General_path_element_t **ret_path, size_t *path_count,
        char ** err_msg);

int  do_pgr_dijkstra(pgr_edge_t  * edges, int64_t total_tuples,
       int64_t  start_vertex, int64_t end_vertex,
       bool has_rcost, bool directed,
       General_path_element_t **ret_path, size_t *path_count,
       char ** err_msg);

#ifdef __cplusplus
}
#endif

#endif  //  SRC_DIJKSTRA_SRC_DIJKSTRA_DRIVER_H_
