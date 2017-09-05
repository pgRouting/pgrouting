/*
 * Shortest path with turn restrictions algorithm for PostgreSQL
 *
 * Copyright (c) 2011 Stephen Woodbridge
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef SRC_TRSP_SRC_TRSP_H_
#define SRC_TRSP_SRC_TRSP_H_

#define MAX_RULE_LENGTH 5

#ifdef __cplusplus

#include <cstddef>

#else  // __cplusplus

// for bool
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-pedantic"
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



typedef struct  {
    int64_t id;
    int64_t source;
    int64_t target;
    double cost;
    double reverse_cost;
} edge_t;

typedef struct restrict_struct {
    int target_id;
    double to_cost;
    int via[MAX_RULE_LENGTH];
}
restrict_t;

typedef struct path_element {
    int64_t vertex_id;
    int64_t edge_id;
    double cost;
} path_element_tt;


#endif  // SRC_TRSP_SRC_TRSP_H_
