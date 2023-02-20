/*PGR-GNU*****************************************************************
File: process_allpairs.h

Function's developer:
Copyright (c) 2023 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#ifndef INCLUDE_ALLPAIRS_PROCESS_ALLPAIRS_H_
#define INCLUDE_ALLPAIRS_PROCESS_ALLPAIRS_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
using IID_t_rt = struct IID_t_rt;
#else
#include <stddef.h>
#include <stdbool.h>
typedef struct IID_t_rt IID_t_rt;
#endif

void process_allpairs(char*, bool, int, IID_t_rt **, size_t *);

#endif  // INCLUDE_ALLPAIRS_PROCESS_ALLPAIRS_H_
