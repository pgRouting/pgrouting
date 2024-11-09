/*PGR-GNU*****************************************************************
File: drivedist_driver.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

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

#ifndef INCLUDE_DRIVERS_DRIVING_DISTANCE_DRIVING_DISTANCE_DRIVER_H_
#define INCLUDE_DRIVERS_DRIVING_DISTANCE_DRIVING_DISTANCE_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>
#include <utils/array.h>

#ifdef __cplusplus
}
#endif

#include "cpp_common/undefPostgresDefine.hpp"

#ifdef __cplusplus
#   include <cstdint>
#   include <cstddef>
using MST_rt = struct MST_rt;
#else
#   include <stddef.h>
#   include <stdint.h>
typedef struct MST_rt MST_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void pgr_do_drivingDistance(
        char*,
        ArrayType*,
        double, bool, bool,
        MST_rt**, size_t*,
        char**, char**, char **);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_DRIVING_DISTANCE_DRIVING_DISTANCE_DRIVER_H_
