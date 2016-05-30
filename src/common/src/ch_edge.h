#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include "../../contraction/src/contraction_structs.hpp"
#include "ch_vertex.h"
#include "basic_edge.h"

namespace pgRouting
{
namespace contraction {
class Edge {
 public:
    #if 0
    enum class Edge_type {
        ordinary = 0,
        shortcut,
    };
    Edge_c() : m_deleted(false),m_type(Edge_type::ordinary) { }
    #endif
    Edge() = default;
    Edge(const Edge &) = default;
    Edge(const pgr_edge_t &other) :
    id(other.id), source(other.source),
    target(other.target), cost(other.cost){}

    void cp_members(const Basic_edge &other);
    void cp_members(const Edge &other);
    EID id;
    VID source;
    VID target;
    double cost;
    bool first;
    void add_contracted_vertex(Vertex& v, int64_t vid);
    #if 0
    inline bool isDeleted() const { return m_deleted; }
    inline Edge_type type() const { return m_type; }
    const std::string type_str() const;
    void add_contracted_vertex(Vertex v);
    inline void set_edge_type(Edge_type etype) { m_type = etype; }
    inline void delete_edge() { m_deleted = true; }
    inline void recover() { m_deleted = false; }
    #endif
    const Identifiers<int64_t>& contracted_vertices() const;
    friend std::ostream& operator <<(std::ostream& os, const Edge& e);
 private:
    Identifiers<int64_t> m_removed_vertices;
    #if 0
    Edge_type m_type;
    bool m_deleted;
    #endif
};

}  // namespace contraction
}  // namespace pgRouting
