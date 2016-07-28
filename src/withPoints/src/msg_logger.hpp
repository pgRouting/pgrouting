/*PGR-GNU*****************************************************************
File: msg_logger.hpp

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

#include <string>
#include <vector>
#include "./../../common/src/pgr_types.h"

void
PGR_LOG_POINTS(
        std::ostringstream &log,
        const std::vector< Point_on_edge_t > &points,
        const std::string &title) {
    log << title << "\n";
    for (const auto &p : points) {
        log << p.pid << "\t"
            << p.edge_id << "\t"
            << p.fraction << "\t"
            << p.side << "\n";
    }
}
