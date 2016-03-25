/*****************************************************
 *  ADD HERE CONTRACTION NAME & NUMBER 
<enum>
*****************************************************/

#pragma once

#include <set>
enum class contractionType { 
     
     none = 0,
     deadEnd, // C++11 allows the extra comma
};

enum class edgeType {
	
	ordinary = 0,
	shortcut,
};


typedef int64_t Vid;

typedef int64_t Eid;


//typedef std::set<Vid> RemovedVertices;


typedef std::set<Vid> Vertex_set;




