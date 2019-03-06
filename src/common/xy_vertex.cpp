/*PGR-GNU*****************************************************************
 *

 Copyright (c) 2015 Celia Virginia Vergara Castillo
 vicky_vergara@hotmail.com

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

#include "cpp_common/xy_vertex.h"

#include <vector>
#include <algorithm>

#include "cpp_common/pgr_assert.h"


namespace pgrouting {


std::ostream& operator<<(std::ostream& log, const XY_vertex &v) {
    log << v.id << "-" << bg::wkt(v.point);
    return log;
}

bool
XY_vertex::operator==(const XY_vertex &rhs) const {
    if (&rhs == this) return true;
    return this->id == rhs.id && boost::geometry::equals(point, rhs.point);
}


size_t
check_vertices(
    std::vector < XY_vertex > vertices) {
    auto count(vertices.size());
    std::stable_sort(
        vertices.begin(), vertices.end(),
        [](const XY_vertex &lhs, const XY_vertex &rhs)
        {return lhs.id < rhs.id;});
    vertices.erase(
        std::unique(
            vertices.begin(), vertices.end(),
            [](const XY_vertex &lhs, const XY_vertex &rhs)
            {return lhs.id == rhs.id;}), vertices.end());

    return count - vertices.size();
}

std::vector < XY_vertex >
extract_vertices(
    const std::vector <Pgr_edge_xy_t > &data_edges) {

    std::vector< XY_vertex > vertices;
    if (data_edges.empty()) return vertices;

    vertices.reserve(data_edges.size() * 2);

    for (const auto edge : data_edges) {
        XY_vertex v_source(edge, true);
        vertices.push_back(v_source);

        XY_vertex v_target(edge, false);
        vertices.push_back(v_target);
    }

    /*
     * sort and delete duplicates
     */
    std::stable_sort(
        vertices.begin(), vertices.end(),
        [](const XY_vertex &lhs, const XY_vertex &rhs)
        {return lhs.id < rhs.id;});
    vertices.erase(
        std::unique(
            vertices.begin(), vertices.end(),
            [](const XY_vertex &lhs, const XY_vertex &rhs)
            {return lhs.id == rhs.id;}), vertices.end());
    return vertices;
}

std::vector < XY_vertex >
extract_vertices(
    const Pgr_edge_xy_t *data_edges, int64_t count) {
    return extract_vertices(
        std::vector < Pgr_edge_xy_t >(data_edges, data_edges + count));
}

#if 0
/* the following might be needed when using withPoints */
std::vector < XY_vertex > extract_vertices(
    std::vector < XY_vertex > vertices,
    const std::vector < Pgr_edge_xy_t > data_edges) {
    if (data_edges.empty()) return vertices;

    vertices.reserve(vertices.size() + data_edges.size() * 2);

    for (const auto edge : data_edges) {
        vertices.push_back(XY_vertex(edge.source, edge.x1, edge.y1));
        vertices.push_back(XY_vertex(edge.target, edge.x2, edge.y2));
    }

    /*
     * sort and delete duplicates
     */
    std::stable_sort(vertices.begin(), vertices.end(),
                     [](const XY_vertex &lhs, const XY_vertex &rhs)
                     {return lhs.id < rhs.id;});

    vertices.erase(
        std::unique(vertices.begin(), vertices.end(),
                    [](const XY_vertex &lhs, const XY_vertex &rhs)
                    {return lhs.id == rhs.id;}), vertices.end());
    return vertices;
}
std::vector < XY_vertex > extract_vertices(
    std::vector < XY_vertex > vertices,
    const Pgr_edge_xy_t *data_edges, int64_t count) {
    return extract_vertices(vertices,
            std::vector < Pgr_edge_xy_t >(data_edges, data_edges + count));
}
#endif

}  // namespace pgrouting
