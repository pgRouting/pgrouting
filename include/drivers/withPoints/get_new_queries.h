/*PGR-GNU*****************************************************************
File: get_new_queries.h

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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
#ifndef INCLUDE_DRIVERS_WITHPOINTS_GET_NEW_QUERIES_H_
#define INCLUDE_DRIVERS_WITHPOINTS_GET_NEW_QUERIES_H_

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    char estimate_drivingSide(char driving_side);

    void get_new_queries(
            char *edges_sql,
            char *points_sql,
            char **edges_of_points_query,
            char **edges_no_points_query);


#ifdef __cplusplus
}
#endif

#endif  // INCLUDE_DRIVERS_WITHPOINTS_GET_NEW_QUERIES_H_
