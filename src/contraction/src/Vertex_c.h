#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "./contraction_structs.hpp"
#include "./pgr_contract.hpp"


class Vertex_c {
 public:
    VID id;
    inline bool isDeleted() const { return m_deleted; }
    inline Contraction_type type() const { return m_type; }
    inline std::string type_str() const;
    inline void setDeleted();
    inline void set_contraction_type(Contraction_type ctype);
    inline const Removed_vertices& removed_vertices() const { return m_removed_vertices; }
    inline void add_contracted_vertex(Vertex_c v);
    friend std::ostringstream& operator <<(std::ostringstream& os, const Vertex_c& v);

 private:
    Contraction_type m_type;
    Removed_vertices m_removed_vertices;
    bool m_deleted;
};