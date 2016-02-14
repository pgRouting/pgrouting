/*PGR-GNU*****************************************************************
 * File: tsp_driver.h
 *
 * Generated with Template by:
 * Copyright (c) 2015 pgRouting developers
 * Mail: project@pgrouting.org
 *
 * Function's developer:
 * Copyright (c) 2015 Celia Virginia Vergara Castillo
 * Mail: vicky_vergara@hotmail.com
 *
 * ------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  ********************************************************************PGR-GNU*/

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include "../../common/src/pgr_types.h"

int 
do_pgr_tsp(
        Matrix_cell_t *distances, size_t total_distances,
        int64_t start_vid,
        int64_t end_vid,
        General_path_element_t **results,
        size_t *total_results,
        char **err_msg);

#ifdef __cplusplus
}
#endif

