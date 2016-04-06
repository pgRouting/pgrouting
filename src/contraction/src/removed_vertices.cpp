#include "./removed_vertices.h"


bool Removed_vertices::isDisjoint(VID id) const {
    return (removed_vertices.isDisjoint(id));
}


bool Removed_vertices::isDisjoint(Removed_vertices vertices) const {
    return removed_vertices.isDisjoint(vertices.get_vertexSet());
}

void Removed_vertices::vertexUnion(VID id) {
    removed_vertices.insert(id);
}

void Removed_vertices::vertexUnion(Removed_vertices vertices) {
    removed_vertices.insert(vertices.get_identifiers());
}


const std::set<VID>& Removed_vertices::ids() const {
    return removed_vertices.ids();
}

const Identifiers<VID>& get_identifiers() const {
    return removed_vertices;
}

