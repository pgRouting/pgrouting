#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "./contraction_structs.hpp"
#include "./removedVertices_c.h"
#include "./pgr_contract.hpp"


class Vertex_c {
 public:
    VID id;
    bool isDeleted() const;
    inline int get_contraction_type() const;
    inline void setDeleted();
    inline void set_contraction_type(Contraction_type ctype);
    inline Vertex_set get_removedVertices() const;
    inline void add_contracted_vertex(Vertex_c v);
    friend std::ostringstream& operator <<(std::ostringstream& os, const Vertex_c& v);
 
 private:
    Contraction_type type;
    Removed_vertices removed_vertices;
    bool deleted;

};