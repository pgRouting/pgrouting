#pragma once
#include <stack>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <string>
#include <functional>
#include "./contraction_structs.hpp"
#include "./removedVertices_c.h"


class Edge_c {
 public:
    enum class Edge_type{
        ordinary = 0,
        shortcut,
    };
    EID id;
    double cost;
    bool isDeleted() const;
    inline void add_contracted_vertex(Vertex_c v);
    inline void set_edge_type(Edge_type etype);
    inline void setDeleted();
    inline int get_edge_type() const;
    inline Vertex_set get_removedVertices() const;
    friend std::ostringstream& operator <<(std::ostringstream& os, const Edge_c& e);
 private:
    Edge_type type;
    Removed_vertices removed_vertices;
    bool deleted;

};