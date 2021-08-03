/*PGR-GNU*****************************************************************
File: dijkstraTR_driver.h

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

#ifndef INCLUDE_DRIVERS_YEN_TURNRESTRICTEDPATH_DRIVER_H_
#define INCLUDE_DRIVERS_YEN_TURNRESTRICTEDPATH_DRIVER_H_
#pragma once

/* for size_t and int64_t */
#ifdef __cplusplus
#  include <cstddef>
#  include <cstdint>
#else
#  include <stddef.h>
#  include <stdint.h>
#endif

typedef struct Edge_t Edge_t;
typedef struct Restriction_t Restriction_t;
typedef struct General_path_element_t General_path_element_t;

#ifdef __cplusplus
extern "C" {
#endif

    /*********************************************************

      TEXT, -- edges_sql
      TEXT, -- restrictions_sql
      BIGINT, -- source
      BIGINT, -- target
      directed BOOLEAN DEFAULT true,
      only_cost BOOLEAN DEFAULT false,
      strict BOOLEAN DEFAULT false
     ********************************************************/


    void do_pgr_turnRestrictedPath(
            Edge_t  *data_edges,
            size_t total_edges,

            Restriction_t *restrictions,
            size_t total_restrictions,

            int64_t start_vid,
            int64_t end_vid,

            size_t k,
            bool directed,
            bool heap_paths,
            bool stop_on_first,
            bool strict,

            General_path_element_t **return_tuples,
            size_t *return_count,
            char ** log_msg,
            char ** notice_msg,
            char ** err_msg);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_YEN_TURNRESTRICTEDPATH_DRIVER_H_
