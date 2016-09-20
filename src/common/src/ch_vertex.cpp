/*PGR-GNU*****************************************************************
File: ch_vertex.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
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
#include <algorithm>
#include <vector>
#include "./ch_vertex.h"

namespace pgrouting {


const Identifiers<int64_t>& CH_vertex::contracted_vertices() const {
    return m_contracted_vertices;
}


bool CH_vertex::has_contracted_vertices() const {
    if (m_contracted_vertices.size() == 0)
        return false;
    return true;
}

void CH_vertex::add_contracted_vertex(CH_vertex& v, int64_t vid) {
    m_contracted_vertices += vid;
    m_contracted_vertices += v.contracted_vertices();
    v.clear_contracted_vertices();
}

std::ostream& operator <<(std::ostream& os, const CH_vertex& v) {
    os << "{\n    id: " << v.id << ",\n";
    os << "    contracted vertices: ";
    os << v.contracted_vertices();
    os << "\n}";
    os << "\n";
    return os;
}

#if 0
size_t
check_vertices(
    std::vector < CH_vertex > vertices) {
    auto count(vertices.size());
    std::stable_sort(
        vertices.begin(), vertices.end(),
        [](const CH_vertex &lhs, const CH_vertex &rhs)
        {return lhs.id < rhs.id;});
    vertices.erase(
        std::unique(
            vertices.begin(), vertices.end(),
            [](const CH_vertex &lhs, const CH_vertex &rhs)
            {return lhs.id == rhs.id;}), vertices.end());
    return count - vertices.size();
}

std::vector < CH_vertex >
extract_vertices(
    const std::vector <pgr_edge_t > &data_edges) {
    std::vector< CH_vertex > vertices;
    if (data_edges.empty()) return vertices;
    vertices.reserve(data_edges.size() * 2);
    for (const auto edge : data_edges) {
        CH_vertex v_source(edge, true);
        vertices.push_back(v_source);

        CH_vertex v_target(edge, false);
        vertices.push_back(v_target);
    }
    /*
     * sort and delete duplicates
     */
    std::stable_sort(
        vertices.begin(), vertices.end(),
        [](const CH_vertex &lhs, const CH_vertex &rhs)
        {return lhs.id < rhs.id;});
    vertices.erase(
        std::unique(
            vertices.begin(), vertices.end(),
            [](const CH_vertex &lhs, const CH_vertex &rhs)
            {return lhs.id == rhs.id;}), vertices.end());
    return vertices;
}

std::vector < CH_vertex >
extract_vertices(
    const pgr_edge_t *data_edges,
    int64_t count) {
    return extract_vertices(
        std::vector < pgr_edge_t >(data_edges, data_edges + count));
}
#endif

}  // namespace pgrouting
