/*PGR-GNU*****************************************************************
File: graph_bandwidth_rt.h

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2025 Saloni Kumari
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
#ifndef INCLUDE_C_TYPES_GRAPH_BANDWIDTH_RT_H_
#define INCLUDE_C_TYPES_GRAPH_BANDWIDTH_RT_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int id;
    int bandwidth;
} GraphBandwidth_rt;

#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_C_TYPES_GRAPH_BANDWIDTH_RT_H_