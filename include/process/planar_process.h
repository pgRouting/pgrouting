/*PGR-GNU*****************************************************************
File: planar_process.h

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
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

#ifndef INCLUDE_PROCESS_PLANAR_PROCESS_H_
#define INCLUDE_PROCESS_PLANAR_PROCESS_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
using IID_t_rt = struct IID_t_rt;
#else
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct IID_t_rt IID_t_rt;
#endif

#include "c_common/enums.h"

#ifdef __cplusplus
extern "C" {
#endif

void pgr_process_planar(
        const char*, bool,
        enum Which,
        IID_t_rt**, size_t*);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_PROCESS_PLANAR_PROCESS_H_
