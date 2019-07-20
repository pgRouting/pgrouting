/*PGR-GNU*****************************************************************
File: mst_common.h

Copyright (c) 2018 Vicky Vergara
Mail: vicky at georepublic dot de

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

#ifndef INCLUDE_DRIVERS_SPANNINGTREE_MST_COMMON_H_
#define INCLUDE_DRIVERS_SPANNINGTREE_MST_COMMON_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief defines ordering
 *
 * @param[in] fn_suffix
 * @param[in] err_msg
 *
 * @return
 * 0 = no matter
 * 1 = DFS
 * 2 = BFS
 */
int
get_order(char * fn_suffix, char ** err_msg);

char *
get_name(int fn_id, char * fn_suffix, char ** err_msg);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_SPANNINGTREE_MST_COMMON_H_
