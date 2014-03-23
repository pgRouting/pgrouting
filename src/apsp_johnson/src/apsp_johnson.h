/*
 * APSP Johnson algorithm for PostgreSQL
 *
 * Copyright (c) 2011 J Kishore Kumar
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

#ifndef _APSP_JOHNSON_H
#define _APSP_JOHNSON_H

typedef struct {
  int source;
  int target;
  float cost;
} edge_apsp_johnson_t;

#ifdef __cplusplus
extern "C" {
#endif
int boost_apsp_johnson(edge_apsp_johnson_t *edges, int count,
    edge_apsp_johnson_t **output_edges, int *output_count, char **err_msg);
void dbg(const char *format, ...);
#ifdef __cplusplus
  }
#endif

#endif
