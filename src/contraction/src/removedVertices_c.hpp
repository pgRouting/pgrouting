#pragma once
#include <iostream>
#include <string>
extern "C" {
#include "postgres.h"
}


#include "./contraction_structs.hpp"
class RemovedVertices {

private:
	Vertex_set removed_vertices;

public:
	bool isDisjoint(Vid id) const;
	bool isDisjoint(RemovedVertices vertices) const;
	void vertexUnion(Vid id);
	void vertexUnion(RemovedVertices vertices);
	Vertex_set get_vertexSet() const;
};


bool RemovedVertices::isDisjoint(Vid id) const {
	return (removed_vertices.find(id)==removed_vertices.end());
}


bool RemovedVertices::isDisjoint(RemovedVertices vertices) const {
	for(auto vertex: removed_vertices)
	{
		if (!isDisjoint(vertex))
		{
			return false;	
		}
	} 
} 

void RemovedVertices::vertexUnion(Vid id)
{
	removed_vertices.insert(id);
} 

void RemovedVertices::vertexUnion(RemovedVertices vertices)
{
	removed_vertices.insert(vertices.get_vertexSet().begin(),vertices.get_vertexSet().end());
}


Vertex_set RemovedVertices::get_vertexSet() const {
	return removed_vertices;
}

