/*PGR-GNU*****************************************************************
File: line_graph_rt.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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
/*! @file */

#ifndef INCLUDE_C_TYPES_LINE_GRAPH_RT_H_
#define INCLUDE_C_TYPES_LINE_GRAPH_RT_H_
#pragma once


#ifdef __cplusplus

#include <cstddef>

#else  // __cplusplus

// for bool
#ifdef __GNUC__
#if __GNUC__ > 5
#pragma GCC diagnostic ignored "-Wpedantic"
#else
#pragma GCC diagnostic ignored "-pedantic"
#endif
#endif

#include <postgres.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

// For NULL & size_t
#include <stdlib.h>


#endif  // __cplusplus

// For int64_t etc
#include <stdint.h>


typedef struct {
    int64_t id;
    int64_t source;
    int64_t target;
    double cost;
    double reverse_cost;
} Line_graph_rt;

#endif  // INCLUDE_C_TYPES_LINE_GRAPH_RT_H_
