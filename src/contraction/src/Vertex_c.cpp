#include "./Vertex_c.h"

bool Vertex_c::isDeleted() const {
    return deleted;
}

int Vertex_c::get_contraction_type() const {
    return static_cast<int>(type);
}

void Vertex_c::setDeleted() {
    deleted = true;
}

void Vertex_c::set_contraction_type(Contraction_type ctype) {
    type = ctype;
}

Vertex_set Vertex_c::get_removedVertices() const {
    return removed_vertices.get_vertexSet();
}


void Vertex_c::add_contracted_vertex(Vertex_c v) {
}

std::ostringstream& operator <<(std::ostringstream& os, const Vertex_c& v) {
    os << "{ id: " << v.id <<  ", type: " << static_cast<int>(v.type);
    os << " removed_vertices: {";
    for (auto removed_vertex : v.get_removedVertices()) {
        os << removed_vertex << ", ";
    }
    os << "} }";
    return os;
}