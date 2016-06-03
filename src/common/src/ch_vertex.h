#pragma once
#include <iostream>
#include <string>
#include <vector>
#if 0
#include <functional>
#include "../../contraction/src/contraction_structs.hpp"
#include "../../contraction/src/contraction_type.hpp"
#endif

#include "pgr_types.h"
#include "identifiers.hpp"
namespace pgRouting {
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
    const Identifiers<int64_t>& contracted_vertices() const;
    void clear_contracted_vertices() { m_contracted_vertices.clear(); }
    template < typename G >
    std::ostream& print_vertex(std::ostream& os, G &graph);
    friend std::ostream& operator <<(std::ostream& os, const Vertex& v);
#if 0    
    Vertex() : m_deleted(false) { }
    std::string type_str() const;
    inline Contraction_type type() const { return m_type; }
    inline bool isDeleted() const { return m_deleted; }
    inline void delete_vertex() { m_deleted = true; }
    inline void recover() { m_deleted = false; }
    inline void set_contraction_type(Contraction_type ctype) { m_type.set_type(ctype); }
#endif
 private:
    Identifiers<int64_t> m_contracted_vertices;
#if 0
    Contraction_type m_type;
    bool m_deleted;
#endif
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
