/*PGR-GNU*****************************************************************
File: astar_process.h

Copyright (c) 2007-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2026 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#ifndef INCLUDE_PROCESS_ASTAR_PROCESS_H_
#define INCLUDE_PROCESS_ASTAR_PROCESS_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
using Path_rt = struct Path_rt;
using ArrayType = struct ArrayType;
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct Path_rt Path_rt;
typedef struct ArrayType ArrayType;
#endif

#include "c_common/enums.h"

#ifdef __cplusplus
extern "C" {
#endif

void pgr_process_astar(
        const char*, const char*,
        ArrayType*, ArrayType*,

        bool, bool, bool,

        int, double, double,

        enum Which,
        Path_rt**, size_t*);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_PROCESS_ASTAR_PROCESS_H_
