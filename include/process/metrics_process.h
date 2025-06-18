/*PGR-GNU*****************************************************************
File: bandwidth_process.h

Function's developer:
Copyright (c) 2025 Saloni kumari
Mail: chaudharysaloni2510 at gmail.com

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

#ifndef INCLUDE_PROCESS_METRICS_BANDWIDTH_PROCESS_H_
#define INCLUDE_PROCESS_METRICS_BANDWIDTH_PROCESS_H_
#pragma once

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>

#else
#include <stddef.h>
#include <stdbool.h>
#endif

#include "c_types/bandwidth_rt.h"

#ifdef __cplusplus
extern "C" {
#endif

void pgr_process_metrics(
    const char* edges_sql,
    GraphBandwidth_rt** result_tuples,
    size_t* result_count,
    char** log_msg,
    char** notice_msg
);

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_PROCESS_METRICS_BANDWIDTH_PROCESS_H_
