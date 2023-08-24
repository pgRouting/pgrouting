/*PGR-GNU*****************************************************************
File: drivedist_driver.h

Generated with Template by:                                                                                             
Copyright (c) 2023 pgRouting developers                                                                                 
Mail: project AT pgrouting.org   

Copyright (c) 2023 Aryan Gupta
guptaaryan1010 AT gmail.com
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

#ifndef INCLUDE_DRIVERS_DRIVING_DISTANCE_DRIVEDIST_DRIVER_H_
#define INCLUDE_DRIVERS_DRIVING_DISTANCE_DRIVEDIST_DRIVER_H_

/* for size-t */
#ifdef __cplusplus
#   include <cstdint>
#   include <cstddef>
using Edge_t = struct Edge_t;
using Path_rt = struct Path_rt;
using MST_rt = struct MST_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Edge_t Edge_t;
typedef struct Path_rt Path_rt;
typedef struct MST_rt MST_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    void do_pgr_driving_many_to_dist(
            Edge_t* , size_t ,
            int64_t* , size_t ,
            double ,
            bool , bool, bool,
             /* TODO  remove the following line on v4 */
            Path_rt**, size_t*,
            MST_rt** , size_t* r,
            char **, char **, char **);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_DRIVING_DISTANCE_DRIVEDIST_DRIVER_H_
