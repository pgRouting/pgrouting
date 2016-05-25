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
namespace pgRouting {
namespace contraction {

class Vertex_c {
 public:

    Vertex_c() = default;
    Vertex_c(const Vertex_c &) = default;
    Vertex_c(const pgr_edge_t &other, bool is_source) :
      id(is_source? other.source : other.target)
      {}

    void cp_members(const Vertex_c &other) {
        this->id = other.id;
    }

#if 0    
    Vertex_c() : m_deleted(false) { }
    std::string type_str() const;
    inline Contraction_type type() const { return m_type; }
    inline bool isDeleted() const { return m_deleted; }
    inline void delete_vertex() { m_deleted = true; }
    inline void recover() { m_deleted = false; }
    inline void set_contraction_type(Contraction_type ctype) { m_type.set_type(ctype); }
    const Removed_vertices& removed_vertices() const;
    void add_contracted_vertex(Vertex_c v);
#endif
    friend std::ostream& operator <<(std::ostream& os, const Vertex_c& v);

    int64_t id;
 private:
#if 0
    Contraction_type m_type;
    Removed_vertices m_removed_vertices;
    bool m_deleted;
#endif
};

size_t
check_vertices(std::vector < Vertex_c > vertices);

std::vector < Vertex_c >
extract_vertices(
    const pgr_edge_t *data_edges, int64_t count);

std::vector < Vertex_c >
extract_vertices(
    const std::vector < pgr_edge_t > &data_edges);

}  // namespace contraction
}  // namespace pgRouting
