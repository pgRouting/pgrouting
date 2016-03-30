#include "./removedVertices_c.h"


bool Removed_vertices::isDisjoint(VID id) const {
    return (removed_vertices.find(id) == removed_vertices.end());
}


bool Removed_vertices::isDisjoint(Removed_vertices vertices) const {
    for (auto vertex : removed_vertices) {
        if (!isDisjoint(vertex)) {
            return false;
        }
    }
}

void Removed_vertices::vertexUnion(VID id) {
    removed_vertices.insert(id);
}

void Removed_vertices::vertexUnion(Removed_vertices vertices) {
    removed_vertices.insert(vertices.get_vertexSet().begin(), vertices.get_vertexSet().end());
}


Vertex_set Removed_vertices::get_vertexSet() const {
    return removed_vertices;
}

