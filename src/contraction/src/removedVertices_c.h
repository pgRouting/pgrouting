#pragma once
#include <iostream>
#include <string>
#include "./contraction_structs.hpp"
class Removed_vertices {
 private:
    Vertex_set removed_vertices;

 public:
    bool isDisjoint(VID id) const;
    bool isDisjoint(Removed_vertices vertices) const;
    void vertexUnion(VID id);
    void vertexUnion(Removed_vertices vertices);
    Vertex_set get_vertexSet() const;
};