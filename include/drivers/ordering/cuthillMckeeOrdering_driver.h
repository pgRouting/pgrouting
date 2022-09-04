/*PGR-GNU*****************************************************************
File: cuthillMckeeOrdering_driver.h

Generated with Template by:
Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Shobhit Chaurasia
Mail: 000shobhitchaurasia at gmail.com

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

#ifndef INCLUDE_DRIVERS_ORDERING_CUTHILLMCKEEORDERING_DRIVER_H_
#define INCLUDE_DRIVERS_ORDERING_CUTHILLMCKEEORDERING_DRIVER_H_


#ifdef __cplusplus
#   include <cstddef>
#   include <cstdint>
using Edge_t = struct Edge_t;
using II_t_rt = struct II_t_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct Edge_t Edge_t;
typedef struct II_t_rt II_t_rt;
#endif


#ifdef __cplusplus
extern "C" {
#endif

void
do_cuthillMckeeOrdering(
        Edge_t*,  size_t,   // edges

        II_t_rt**,  size_t*,  // results

        char **,  // log msg
        char **,  // notice msg
        char **);  // err msg


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_ORDERING_CUTHILLMCKEEORDERING_DRIVER_H_
