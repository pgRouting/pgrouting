/*****************************************************
 *  ADD HERE CONTRACTION NAME & NUMBER 
<enum>
*****************************************************/

#pragma once

#include <set>

enum class Contraction_type { 
     none = 0,
     deadEnd,
     last
};


typedef int64_t VID;

typedef int64_t EID;


//typedef std::set<Vid> RemovedVertices;


typedef std::set<VID> Vertex_set;




