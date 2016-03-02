
enum class contractionType { 
     
     none = 0,
     deadEnd, // C++11 allows the extra comma
};

enum class edgeType {
	
	ordinary = 0,
	shortcut,
};

class Vertex_c {
	
	int64_t id;
	contractionType type;
	std::deque<Vertex_c> removed_vertices;
	void add_contracted_vertex (Vertex_c v);
	void set_contraction_type (contractionType ctype);
	friend std::ostringstream& operator <<(std::ostringstream &os,const Vertex_c& v);	
};

class Edge_c {
	int64_t id;
	edgeType type;
	int64_t source;
	int64_t target;
	double cost;
	std::deque<Vertex_c> removed_vertices;
	void add_contracted_vertex (Vertex_c v);
	void set_contraction_type (edgeType etype);
	friend std::ostringstream& operator << (std::ostringstream &os,const Edge_c& e);
};

void Vertex_c::add_contracted_vertex (Vertex_c v) {
  
  removed_vertices.push_front(v);
}

void Vertex_c::set_contraction_type (contractionType ctype) {

	type = ctype;
}

std::ostringstream& operator <<(std::ostringstream &os,const Vertex_c& v)
{
	os << "{ id: " << v.id << ", type: " << v.type ;
	os << " removed_vertices: {";
	for (auto const& v.removed_vertices : removed_vertex )
	{
		os << removed_vertex << ", ";
	}
	os << "} }";

	return os;
}


void Edge_c::add_contracted_vertex (Vertex_c v) {
  
  removed_vertices.push_front(v);
}

void Edge_c::set_contraction_type (edgeType etype) {

	type = etype;
}

std::ostringstream& operator <<(std::ostringstream &os,const Edge_c& e)
{
	os << "{ id: " << e.id << ", type: " << e.type ;
	os << " removed_vertices: {";
	for (auto const& e.removed_vertices : removed_vertex )
	{
		os << removed_vertex << ", ";
	}
	os << "} }";

	return os;
} 