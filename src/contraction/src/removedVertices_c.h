#pragma once
#include <iostream>
#include <string>
#include "./contraction_structs.hpp"
#include "Identifiers.hpp"
class Removed_vertices {
 private:
    Identifiers<VID> removed_vertices;
 public:
    bool isDisjoint(VID id) const;
    bool isDisjoint(Removed_vertices vertices) const;
    void vertexUnion(VID id);
    void vertexUnion(Removed_vertices vertices);
    const Identifiers<VID>& get_identifiers() const;
    const std::set<VID>& ids() const;
};