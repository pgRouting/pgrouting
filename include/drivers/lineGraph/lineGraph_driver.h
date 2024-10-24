/*PGR-GNU*****************************************************************
File: lineGraph_driver.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

#ifndef INCLUDE_DRIVERS_LINEGRAPH_LINEGRAPH_DRIVER_H_
#define INCLUDE_DRIVERS_LINEGRAPH_LINEGRAPH_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
using Edge_rt = struct Edge_rt;
#else
#   include <stddef.h>
#include <stdbool.h>
typedef struct Edge_rt Edge_rt;
#endif


#ifdef __cplusplus
extern "C" {
#endif

void pgr_do_lineGraph(
        char*,

        bool,

        Edge_rt**, size_t*,
        char**, char**, char**);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_LINEGRAPH_LINEGRAPH_DRIVER_H_
