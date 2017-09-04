/*PGR-GNU*****************************************************************
File: ch_edge.cpp

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

#include "cpp_common/ch_edge.h"

namespace pgrouting {

void
CH_edge::cp_members(const CH_edge &other) {
    this->cost = other.cost;
    this->id = other.id;
    this->m_contracted_vertices += other.contracted_vertices();
}


bool
CH_edge::has_contracted_vertices() const {
    return !m_contracted_vertices.empty();
}

const Identifiers<int64_t>&
CH_edge::contracted_vertices() const {
    return m_contracted_vertices;
}


void
CH_edge::add_contracted_vertex(CH_vertex& v, int64_t vid) {
    m_contracted_vertices += vid;
    m_contracted_vertices += v.contracted_vertices();
    v.clear_contracted_vertices();
}

void
CH_edge::add_contracted_edge_vertices(CH_edge &e) {
    m_contracted_vertices += e.contracted_vertices();
    e.clear_contracted_vertices();
}

std::ostream& operator <<(std::ostream& os, const CH_edge& e) {
    os << "{id: " << e.id << ",\t"
        << "source: " << e.source << ",\t"
        << "target: " << e.target << ",\t"
        << "cost: " << e.cost << ",\t"
        << "contracted vertices: "
        << e.contracted_vertices()
        << "}";
    return os;
}

}  // namespace pgrouting
