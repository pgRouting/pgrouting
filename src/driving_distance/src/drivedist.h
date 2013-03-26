/*
 * Drivedist algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
 
#ifndef _DRIVEDIST_H
#define _DRIVEDIST_H

#include "postgres.h"
#include "dijkstra.h"

#ifdef __cplusplus
extern "C"
#endif

int boost_dijkstra_dist(edge_t *edges, unsigned int count, 
                   int source_vertex_id, double rdistance,
		   bool directed, bool has_reverse_cost,
                   path_element_t **path, int *path_count, char **err_msg);

#endif
