/*PGR-GNU*****************************************************************
File: contraction_structs.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

#pragma once

#include <set>
#include "./identifiers.hpp"


/*enum class Contraction_type { 
     none = -2,
     last,
     deadEnd
};*/


typedef int64_t VID;

typedef int64_t EID;


// typedef std::set<Vid> RemovedVertices;


typedef std::set<VID> Vertex_set;

typedef Identifiers<VID> Removed_vertices;




