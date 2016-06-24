#ifndef CH_EDGE_H
#define CH_EDGE_H

#include <iostream>
#include <sstream>
#include "ch_vertex.h"
namespace pgRouting
{
namespace contraction {
class Edge {
 public:
    Edge() = default;
    Edge(int64_t eid, int64_t source, int64_t target, double cost) :
    id(eid), source(source),
    target(target), cost(cost), first(true){}
    Edge(int64_t eid, int64_t source, int64_t target, double cost, bool first) :
    id(eid), source(source),
    target(target), cost(cost), first(first){}
    void cp_members(const Edge &other, std::ostringstream& log);
    int64_t id;
    int64_t source;
    int64_t target;
    double cost;
    bool first;
    void add_contracted_vertex(Vertex& v, int64_t vid);
    void add_contracted_edge_vertices(Edge& e);
    bool has_contracted_vertices() const;
    void clear_contracted_vertices() { m_contracted_vertices.clear(); }
    const Identifiers<int64_t>& contracted_vertices() const;
    friend std::ostream& operator <<(std::ostream& os, const Edge& e);
 private:
    Identifiers<int64_t> m_contracted_vertices;
};

}  // namespace contraction
}  // namespace pgRouting
#endif