#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "./contraction_structs.hpp"
#include "./contraction_type.hpp"


class Vertex_c {
 public:
    Vertex_c() : m_deleted(false) { }
    VID id;
    inline bool isDeleted() const { return m_deleted; }
    inline Contraction_type type() const { return m_type; }
    std::string type_str() const;
    inline void delete_vertex() { m_deleted = true; }
    inline void recover() { m_deleted = false; }
    inline void set_contraction_type(Contraction_type ctype) { m_type.set_type(ctype); }
    const Removed_vertices& removed_vertices() const;
    void add_contracted_vertex(Vertex_c v);
    friend std::ostringstream& operator <<(std::ostringstream& os, const Vertex_c& v);

 private:
    Contraction_type m_type;
    Removed_vertices m_removed_vertices;
    bool m_deleted;
};