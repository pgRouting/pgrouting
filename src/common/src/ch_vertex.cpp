#include <algorithm>

#include "./ch_vertex.h"

namespace pgRouting {
namespace contraction {


const Identifiers<int64_t>& Vertex::contracted_vertices() const { 
    return m_contracted_vertices; 
}

#if 0
std::string Vertex_c::type_str() const {
    return m_type.type_str();
}

void Vertex_c::add_contracted_vertex(Vertex_c v) {
}
#endif

std::ostream& operator <<(std::ostream& os, const Vertex& v) {
    os << "{\n    id: " << v.id << ",\n";
    os << "    contracted vertices: ";
    os << v.contracted_vertices();
    // <<  ", type: " << v.type_str();
    //os << ", isDeleted: " << v.isDeleted();
    /*os << " removed_vertices: {";
    for (auto removed_vertex : v.removed_vertices().ids()) {
        os << removed_vertex << ", ";
    }*/
    os << "\n}";
    os << "\n";
    return os;
}

size_t
check_vertices(
    std::vector < Vertex > vertices) {
    auto count(vertices.size());
    std::stable_sort(
        vertices.begin(), vertices.end(),
        [](const Vertex &lhs, const Vertex &rhs)
        {return lhs.id < rhs.id;});
    vertices.erase(
        std::unique(
            vertices.begin(), vertices.end(),
            [](const Vertex &lhs, const Vertex &rhs)
            {return lhs.id == rhs.id;}), vertices.end()
        );

    return count - vertices.size();
}

std::vector < Vertex >
extract_vertices(
    const std::vector <pgr_edge_t > &data_edges) {

    std::vector< Vertex > vertices;
    if (data_edges.empty()) return vertices;

    vertices.reserve(data_edges.size() * 2);

    for (const auto edge : data_edges) {
        Vertex v_source(edge, true);
        vertices.push_back(v_source);

        Vertex v_target(edge, false);
        vertices.push_back(v_target);
    }

    /*
     * sort and delete duplicates
     */
    std::stable_sort(
        vertices.begin(), vertices.end(),
        [](const Vertex &lhs, const Vertex &rhs)
        {return lhs.id < rhs.id;});
    vertices.erase(
        std::unique(
            vertices.begin(), vertices.end(),
            [](const Vertex &lhs, const Vertex &rhs)
            {return lhs.id == rhs.id;}), vertices.end()
        );
    return vertices;
}

std::vector < Vertex >
extract_vertices(
    const pgr_edge_t *data_edges,
    int64_t count) {
    return extract_vertices(
        std::vector < pgr_edge_t >( data_edges, data_edges + count)
        );
}

}  // namespace contraction
}  // namespace pgRouting
