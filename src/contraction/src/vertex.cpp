#include "./vertex.h"


const Removed_vertices& Vertex_c::removed_vertices() const { 
    return m_removed_vertices; 
}



std::string Vertex_c::type_str() const {
    return m_type.type_str();
}

void Vertex_c::add_contracted_vertex(Vertex_c v) {
}

std::ostringstream& operator <<(std::ostringstream& os, const Vertex_c& v) {
    os << "{ id: " << v.id <<  ", type: " << v.type_str();
    os << ", isDeleted: " << v.isDeleted();
    os << " removed_vertices: {";
    for (auto removed_vertex : v.removed_vertices().ids()) {
        os << removed_vertex << ", ";
    }
    os << "} }";
    os << "\n";
    return os;
}