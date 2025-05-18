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

#ifndef INCLUDE_PROCESS_ALLPAIRS_PROCESS_H_
#define INCLUDE_PROCESS_ALLPAIRS_PROCESS_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
using IID_t_rt = struct IID_t_rt;
#else
#include <stddef.h>
#include <stdbool.h>
typedef struct IID_t_rt IID_t_rt;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void pgr_process_allpairs(const char*, bool, int, IID_t_rt **, size_t *);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_PROCESS_ALLPAIRS_PROCESS_H_
