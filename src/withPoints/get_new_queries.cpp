/*PGR-GNU*****************************************************************
File: get_new_queries.cpp

Generated with Template by:
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

#include "drivers/withPoints/get_new_queries.h"
#include <string.h>
#include <sstream>
#include "cpp_common/pgr_alloc.hpp"

char
estimate_drivingSide(char driving_side) {
    char d_side = static_cast<char>(tolower(driving_side));
    if (!((d_side == 'r')
                || (d_side == 'l'))) {
        d_side = 'b';
    }
    return d_side;
}

void
get_new_queries(
        char *edges_sql,
        char *points_sql,
        char **edges_of_points_query,
        char **edges_no_points_query) {
    std::ostringstream edges_of_points_sql;
    std::ostringstream edges_no_points_sql;

    edges_of_points_sql << "WITH "
        << " edges AS (" << edges_sql << "), "
        << " points AS (" << points_sql << ")"
        << " SELECT DISTINCT edges.* FROM edges JOIN points ON (id = edge_id)";
    *edges_of_points_query = pgr_msg(edges_of_points_sql.str().c_str());

    edges_no_points_sql << "WITH "
        << " edges AS (" << edges_sql << "), "
        << " points AS (" << points_sql << ")"
        << " SELECT edges.*"
        << " FROM edges"
        << " WHERE NOT EXISTS (SELECT edge_id FROM points WHERE id = edge_id)";
    *edges_no_points_query = pgr_msg(edges_no_points_sql.str().c_str());
}

