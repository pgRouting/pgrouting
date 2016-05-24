#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include "./contraction_structs.hpp"
#include "vertex.h"
#include "../../common/src/basic_edge.h"

namespace pgRouting
{
class Edge_c {
 public:
    enum class Edge_type {
        ordinary = 0,
        shortcut,
    };
    Edge_c() : m_deleted(false),m_type(Edge_type::ordinary) { }
    void cp_members(const Basic_edge &other);
    void cp_members(const Edge_c &other);
    EID id;
    VID source;
    VID target;
    double cost;
    bool first;
    inline bool isDeleted() const { return m_deleted; }
    inline Edge_type type() const { return m_type; }
    const std::string type_str() const;
    void add_contracted_vertex(Vertex_c v);
    inline void set_edge_type(Edge_type etype) { m_type = etype; }
    inline void delete_edge() { m_deleted = true; }
    inline void recover() { m_deleted = false; }
    const Removed_vertices& removed_vertices() const;
    friend std::ostream& operator <<(std::ostream& os, const Edge_c& e);
 private:
    Edge_type m_type;
    Removed_vertices m_removed_vertices;
    bool m_deleted;
};
}