#include "./edge.h"
namespace pgRouting
{
void Edge_c::cp_members(const Edge_c &other) {
        this->id = other.id;
        this->cost = other.cost;
        //this.set_edge_type(other.type());
    }
void Edge_c::cp_members(const Basic_edge &other) {
        this->id = other.id;
        this->cost = other.cost;
        //this.set_edge_type(other.type());
    }

const Removed_vertices& Edge_c::removed_vertices() const { 
    return m_removed_vertices; 
}


void Edge_c::add_contracted_vertex(Vertex_c v) {
}

const std::string Edge_c::type_str() const {
    switch (Edge_c::type()) {
        case Edge_c::Edge_type::ordinary:
            return "Ordinary Edge";
        case Edge_c::Edge_type::shortcut:
            return "Shortcut Edge";
        default:
            return "Undefined Edge";
    }
}


std::ostream& operator <<(std::ostream& os, const Edge_c& e) {
    os << "{ id: " << e.id; //<< ", type: " << e.type_str();
    //os << ", isDeleted: " << e.isDeleted();
    /*os << ", removed_vertices: {";
    for (auto removed_vertex : e.removed_vertices().ids()) {
        os << removed_vertex << ", ";
    }
    os << "} }";*/
    os << '\n';
    return os;
}
}