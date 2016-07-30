/*PGR-GNU*****************************************************************
File: removed_vertices.h

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
#ifndef SRC_CONTRACTION_SRC_REMOVED_VERTICES_H_
#define SRC_CONTRACTION_SRC_REMOVED_VERTICES_H_
#include <iostream>
#include <string>
#include <set>
#include "./contraction_structs.hpp"
#include "./identifiers.hpp"
class Removed_vertices {
 private:
    Identifiers<VID> removed_vertices;
 public:
    bool isDisjoint(VID id) const;
    bool isDisjoint(Removed_vertices vertices) const;
    void vertexUnion(VID id);
    void vertexUnion(Removed_vertices vertices);
    const Identifiers<VID>& get_identifiers() const;
    const std::set<VID>& ids() const;
};
#endif  // SRC_CONTRACTION_SRC_REMOVED_VERTICES_H_
