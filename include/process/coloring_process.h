/*PGR-GNU*****************************************************************
File: allpairs_process.h

Function's developer:
Copyright (c) 2025 Celia Virginia Vergara Castillo
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

#ifndef INCLUDE_PROCESS_COLORING_PROCESS_H_
#define INCLUDE_PROCESS_COLORING_PROCESS_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
using II_t_rt = struct II_t_rt;
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct II_t_rt II_t_rt;
#endif

#include "c_common/enums.h"

#ifdef __cplusplus
extern "C" {
#endif

void pgr_process_coloring(
        const char*,
        enum Which,
        II_t_rt**, size_t*);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_PROCESS_COLORING_PROCESS_H_
