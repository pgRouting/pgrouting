/*****************************************************
 *  ADD HERE CONTRACTION NAME & NUMBER 
<enum>
*****************************************************/

#pragma once

#include <set>
#include "./Identifiers.hpp"

enum class Contraction_type { 
     none = -2,
     last,
     deadEnd
};


typedef int64_t VID;

typedef int64_t EID;


// typedef std::set<Vid> RemovedVertices;


typedef std::set<VID> Vertex_set;

typedef Identifiers<VID> Removed_vertices;




