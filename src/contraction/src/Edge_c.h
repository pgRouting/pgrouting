#pragma once
#include <stack>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <string>
#include <functional>
#include "./contraction_structs.hpp"


class Edge_c {
 public:
    enum class Edge_type {
        ordinary = 0,
        shortcut,
    };
    EID id;
    double cost;
    inline bool isDeleted() const { return m_deleted; }
    inline Edge_type type() const { return m_type; }
    inline std::string type_str() const;
    inline void add_contracted_vertex(Vertex_c v);
    inline void set_edge_type(Edge_type etype);
    inline void setDeleted();
    inline const Removed_vertices& removed_vertices() const { m_removed_vertices; }
    friend std::ostringstream& operator <<(std::ostringstream& os, const Edge_c& e);
 private:
    Edge_type m_type;
    Removed_vertices m_removed_vertices;
    bool m_deleted;
};