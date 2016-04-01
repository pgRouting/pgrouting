#include "./Edge_c.h"

/*
Indicates whether an edge is deleted or not.
Returns:
true : the edge is logically deleted
false : otherwise
*/
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

std::string Edge_c::type_str() const {
    switch (type()) {
        case ordinary:
            return "Ordinary Edge";
        case shortcut:
            return "Shortcut Edge";
    }
}


std::ostringstream& operator <<(std::ostringstream& os, const Edge_c& e) {
    os << "{ id: " << e.id << ", type: " << e.get_edge_type();
    os << " removed_vertices: {";
    for (auto removed_vertex : e.removed_vertices.ids()) {
        os << removed_vertex << ", ";
    }
    os << "} }";
    return os;
}