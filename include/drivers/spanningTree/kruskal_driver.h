/*PGR-GNU*****************************************************************
File: kruskal_driver.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com

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

#ifndef INCLUDE_DRIVERS_SPANNINGTREE_KRUSKAL_DRIVER_H_
#define INCLUDE_DRIVERS_SPANNINGTREE_KRUSKAL_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
#else
#   include <stddef.h>
#   include <stdint.h>
#endif

typedef struct Edge_t Edge_t;
typedef struct MST_rt MST_rt;

#ifdef __cplusplus
extern "C" {
#endif

    void
        do_pgr_kruskal(
                Edge_t  *data_edges,
                size_t total_edges,

                int64_t *root,
                size_t total_roots,

                char* fn_suffix,

                int64_t max_depth,
                double distance,

                MST_rt **return_tuples,
                size_t *return_count,
                char ** log_msg,
                char ** notice_msg,
                char ** err_msg);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_SPANNINGTREE_KRUSKAL_DRIVER_H_
