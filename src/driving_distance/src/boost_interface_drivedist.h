/*PGR-GNU*****************************************************************
File: boost_interface_drivedist.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef SRC_DRIVING_DISTANCE_SRC_BOOST_INTERFACE_DRIVEDIST_H_
#define SRC_DRIVING_DISTANCE_SRC_BOOST_INTERFACE_DRIVEDIST_H_

#include "stdlib.h"
#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    void do_pgr_driving_many_to_dist(
            pgr_edge_t  * edges, size_t total_tuples,
            int64_t  *start_vertex, size_t s_len,
            double distance,
            bool directed,
            bool equicost,
            General_path_element_t **ret_path, size_t *path_count,
            char ** err_msg);

    void do_pgr_driving_distance(
            pgr_edge_t  * edges, size_t total_tuples,
            int64_t  start_vertex, double distance,
            bool directed,
            General_path_element_t **ret_path, size_t *path_count,
            char ** err_msg);

#ifdef __cplusplus
}
#endif

#endif  //  SRC_DRIVING_DISTANCE_SRC_BOOST_INTERFACE_DRIVEDIST_H_
