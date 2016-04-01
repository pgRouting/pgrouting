#include "./Vertex_c.h"

std::string Vertex_c::type_str() const {
    switch (type()) {
        case none:
            return "None";
        case deadEnd:
            return "Dead End";
    }
}

void Vertex_c::setDeleted() {
    deleted = true;
}

void Vertex_c::set_contraction_type(Contraction_type ctype) {
    type = ctype;
}

void Vertex_c::add_contracted_vertex(Vertex_c v) {
}

std::ostringstream& operator <<(std::ostringstream& os, const Vertex_c& v) {
    os << "{ id: " << v.id <<  ", type: " << v.type_str();
    os << " removed_vertices: {";
    for (auto removed_vertex : v.removed_vertices().ids() {
        os << removed_vertex << ", ";
    }
    os << "} }";
    return os;
}