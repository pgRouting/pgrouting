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

#if 0
#include "c_types/iid_t_rt.h"
#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/edges_input.h"
#endif

void process_allpairs(
        char* edges_sql,
        bool directed,
        IID_t_rt **result_tuples,
        size_t *result_count);

#endif  // INCLUDE_ALLPAIRS_PROCESS_ALLPAIRS_H_
