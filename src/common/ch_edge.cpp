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

#include "cpp_common/ch_edge.hpp"

namespace pgrouting {

void CH_edge::set_contracted_vertices(
    Identifiers<int64_t>& contracted_vertices_ids) {
    m_contracted_vertices = contracted_vertices_ids;
}

void
CH_edge::cp_members(const CH_edge &other) {
    this->cost = other.cost;
    this->id = other.id;
    this->source = other.source;
    this->target = other.target;
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

Identifiers<int64_t>&
CH_edge::contracted_vertices() {
    return m_contracted_vertices;
}


void
CH_edge::add_contracted_vertex(CH_vertex& v) {
    m_contracted_vertices += v.id;
    m_contracted_vertices += v.contracted_vertices();
}

void
CH_edge::add_contracted_edge_vertices(CH_edge &e) {
    if (e.has_contracted_vertices()) {
        m_contracted_vertices += e.contracted_vertices();
    }
}

void CH_edge::add_contracted_vertices(Identifiers<int64_t>& ids) {
    m_contracted_vertices += ids;
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
