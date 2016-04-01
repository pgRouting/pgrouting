
#pragma once
#include <iostream>
#include <string>
#include <functional>
extern "C" {
#include "postgres.h"
}

#include "./contraction_structs.hpp"

#include "./removedVertices_c.hpp"

class Vertex_c {

private:
	contractionType type;
	RemovedVertices removed_vertices;
	bool deleted;

public:
	Vid id;
	bool isDeleted() const;
	int get_contraction_type() const;
	void setDeleted();
	void set_contraction_type(contractionType ctype);
	Vertex_set get_removedVertices() const;
	void add_contracted_vertex(Vertex_c v);
	friend std::ostringstream& operator <<(std::ostringstream& os,const Vertex_c& v);
};

bool Vertex_c::isDeleted() const{
	return deleted;
}

int Vertex_c::get_contraction_type()const {

	return static_cast<int>(type);
}

void Vertex_c::setDeleted()
{
	deleted = true;
}

void Vertex_c::set_contraction_type(contractionType ctype) {

	type = ctype;
}

Vertex_set Vertex_c::get_removedVertices()const {
	return removed_vertices.get_vertexSet();
}


void Vertex_c::add_contracted_vertex(Vertex_c v) {
  

}

std::ostringstream& operator <<(std::ostringstream& os,const Vertex_c& v)
{
	os << "{ id: " << v.id <<  ", type: " << static_cast<int>(v.type) ;
	os << " removed_vertices: {";
	for (auto removed_vertex : v.get_removedVertices() )
	{
		os << removed_vertex << ", ";
	}
	os << "} }";

	return os;
}