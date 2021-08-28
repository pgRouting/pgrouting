/*PGR-GNU*****************************************************************
File: minCostMaxFlow_driver.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

#ifndef INCLUDE_DRIVERS_MAX_FLOW_MINCOSTMAXFLOW_DRIVER_H_
#define INCLUDE_DRIVERS_MAX_FLOW_MINCOSTMAXFLOW_DRIVER_H_
#pragma once

/* for size-t */
#ifdef __cplusplus
#   include <cstddef>
using Flow_t = struct Flow_t;
#else
#   include <stddef.h>
typedef struct Flow_t Flow_t;
#endif

#include "c_types/ii_t_rt.h"
#include "c_types/costFlow_t.h"



#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      TEXT,
    BIGINT,
    BIGINT,
     ********************************************************/


    void
        do_pgr_minCostMaxFlow(
                CostFlow_t  *data_edges,
                size_t total_edges,
                II_t_rt  *combinations,
                size_t total_combinations,
                int64_t* source_vertices,
                size_t size_source_verticesArr,
                int64_t* sink_vertices,
                size_t size_sink_verticesArr,
                bool only_cost,
                Flow_t **return_tuples,
                size_t *return_count,
                char ** log_msg,
                char ** notice_msg,
                char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_MAX_FLOW_MINCOSTMAXFLOW_DRIVER_H_
