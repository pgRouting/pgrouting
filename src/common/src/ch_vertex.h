/*PGR-GNU*****************************************************************
File: ch_vertex.h

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
#ifndef SRC_COMMON_SRC_CH_VERTEX_H_
#define SRC_COMMON_SRC_CH_VERTEX_H_
#include <iostream>
#include <sstream>
#include <vector>

#include "./pgr_types.h"
#include "./identifiers.hpp"
namespace pgrouting {
namespace contraction {


class Vertex {
 public:
    int64_t id;
    Vertex() = default;
    Vertex(const Vertex &) = default;
    Vertex(const pgr_edge_t &other, bool is_source) :
      id(is_source? other.source : other.target)
      {}
    void cp_members(const Vertex &other) {
        this->id = other.id;
    }
    void add_contracted_vertex(Vertex& v, int64_t vid);
    void add_vertex_id(int64_t vid) { m_contracted_vertices += vid; }
    const Identifiers<int64_t>& contracted_vertices() const;
    bool has_contracted_vertices() const;
    void clear_contracted_vertices() { m_contracted_vertices.clear(); }
    friend std::ostream& operator <<(std::ostream& os, const Vertex& v);
 private:
    Identifiers<int64_t> m_contracted_vertices;
};

size_t
check_vertices(std::vector < Vertex > vertices);

std::vector < Vertex >
extract_vertices(
    const pgr_edge_t *data_edges, int64_t count);

std::vector < Vertex >
extract_vertices(
    const std::vector < pgr_edge_t > &data_edges);

}  // namespace contraction
}  // namespace pgrouting
#endif  // SRC_COMMON_SRC_CH_VERTEX_H_
