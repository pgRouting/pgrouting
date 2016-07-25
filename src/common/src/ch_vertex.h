#ifndef CH_VERTEX_H
#define CH_VERTEX_H
#include <iostream>
#include <sstream>
#include <vector>

#include "pgr_types.h"
#include "identifiers.hpp"
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
}  // namespace pgRouting
#endif
