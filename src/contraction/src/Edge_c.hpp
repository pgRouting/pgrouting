
#pragma once
#include <stack>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <string>
#include <functional>
extern "C" {
#include "postgres.h"
}

#include "./contraction_structs.hpp"
#include "./removedVertices_c.hpp"
class Edge_c {

private:
	edgeType type;
	RemovedVertices removed_vertices;
	bool deleted;

public:
	Eid id;
	double cost;
	bool isDeleted() const;
	void add_contracted_vertex (Vertex_c v);
	void set_edge_type(edgeType etype);
	void setDeleted();
	int get_edge_type() const;
	Vertex_set get_removedVertices() const;
	friend std::ostringstream& operator <<(std::ostringstream& os,const Edge_c& e);

};

bool Edge_c::isDeleted()const {
	return deleted;
}

void Edge_c::setDeleted()
{
	deleted = true;
}

void Edge_c::add_contracted_vertex (Vertex_c v) {
  
}

void Edge_c::set_edge_type (edgeType etype) {

	type = etype;
}

int Edge_c::get_edge_type()const {

	return static_cast<int>(type);
}

Vertex_set Edge_c::get_removedVertices()const {
	return removed_vertices.get_vertexSet();
}


std::ostringstream& operator <<(std::ostringstream& os,const Edge_c& e)
{
	os << "{ id: " << e.id << ", type: " << static_cast<int>(e.type) ;
	os << " removed_vertices: {";
	for (auto removed_vertex : e.get_removedVertices() )
	{
		os << removed_vertex << ", ";
	}
	os << "} }";

	return os;
} 