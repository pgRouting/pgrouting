#include "./Edge_c.h"


bool Edge_c::isDeleted() const {
    return deleted;
}

void Edge_c::setDeleted() {
    deleted = true;
}

void Edge_c::add_contracted_vertex(Vertex_c v) {
}

void Edge_c::set_edge_type(Edge_type etype) {
    type = etype;
}

int Edge_c::get_edge_type() const {
    return static_cast<int>(type);
}

Vertex_set Edge_c::get_removedVertices() const {
    return removed_vertices.get_vertexSet();
}

std::ostringstream& operator <<(std::ostringstream& os, const Edge_c& e) {
    os << "{ id: " << e.id << ", type: " << static_cast<int>(e.type);
    os << " removed_vertices: {";
    for (auto removed_vertex : e.get_removedVertices()) {
        os << removed_vertex << ", ";
    }
    os << "} }";
    return os;
}