/*PGR-GNU*****************************************************************
File: removed_vertices.cpp

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
#include <set>
#include "./removed_vertices.h"


bool Removed_vertices::isDisjoint(VID id) const {
    return (removed_vertices.isDisjoint(id));
}


bool Removed_vertices::isDisjoint(Removed_vertices vertices) const {
    return removed_vertices.isDisjoint(vertices.get_vertexSet());
}

void Removed_vertices::vertexUnion(VID id) {
    removed_vertices.insert(id);
}

void Removed_vertices::vertexUnion(Removed_vertices vertices) {
    removed_vertices.insert(vertices.get_identifiers());
}


const std::set<VID>& Removed_vertices::ids() const {
    return removed_vertices.ids();
}

const Identifiers<VID>& get_identifiers() const {
    return removed_vertices;
}

