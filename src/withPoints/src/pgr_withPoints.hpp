/*PGR-GNU*****************************************************************
File: withPoints_driver.h

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

#pragma once
#include <vector>

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/basePath_SSEC.hpp"

int
check_points(std::vector< Point_on_edge_t > &points,
                std::ostringstream &log);

void
eliminate_details_dd(
        Path &path);

void
eliminate_details(
        Path &path,
        const std::vector< pgr_edge_t > &point_edges);

void
adjust_pids(
        const std::vector< Point_on_edge_t > &points,
        Path &path);


bool
create_new_edges(
        std::vector< Point_on_edge_t > &points,
        const std::vector< pgr_edge_t > &edges,
        char driving_side,
        std::vector< pgr_edge_t > &new_edges);

bool
create_new_edges(
        std::vector< Point_on_edge_t > &points,
        const std::vector< pgr_edge_t > &edges,
        char driving_side,
        std::vector< pgr_edge_t > &new_edges,
        std::ostringstream &log);
