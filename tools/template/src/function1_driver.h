/*PGR-GNU*****************************************************************
File: MY_FUNCTION_NAME_driver.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) YEAR DEVELOPER_NAME
Mail: DEVELOPER_EMAIL

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

#ifndef SRC_MY_FUNCTION_NAME_UPPER_SRC_MY_FUNCTION_NAME_UPPER_DRIVER_H_
#define SRC_MY_FUNCTION_NAME_UPPER_SRC_MY_FUNCTION_NAME_UPPER_DRIVER_H_

#include "./../../common/src/pgr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************
      MY_QUERY_LINE1
     ********************************************************/
    void
        do_pgr_MY_FUNCTION_NAME(
                pgr_edge_t  *data_edges,
                size_t total_tuples,
                int64_t start_vid,
                int64_t  *end_vidsArr,
                int size_end_vidsArr,
                bool directed,
                MY_RETURN_VALUE_TYPE **return_tuples,
                size_t *return_count,
                char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // SRC_MY_FUNCTION_NAME_UPPER_SRC_MY_FUNCTION_NAME_UPPER_DRIVER_H_
