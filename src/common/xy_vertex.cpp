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
#if BOOST_Geometry_VERSION_OK
#include <boost/geometry/io/wkt/write.hpp>
#else
#include <boost/bgeometry/io/wkt/write.hpp>
#endif

#include <vector>
#include <limits>
#include <algorithm>

#include "cpp_common/pgr_assert.h"

namespace {

template<typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
        // unless the result is subnormal
        || std::abs(x-y) < std::numeric_limits<T>::min();
}

}  // namespace

namespace pgrouting {


std::ostream& operator<<(std::ostream& log, const XY_vertex &v) {
    log << v.id << "-" << bg::wkt(v.point);
    return log;
}

bool
XY_vertex::operator==(const XY_vertex &rhs) const {
    if (&rhs == this) return true;
    return this->id == rhs.id &&
        almost_equal(point.x(), rhs.point.x(), 2) && almost_equal(point.y(), rhs.point.y(), 2);
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
