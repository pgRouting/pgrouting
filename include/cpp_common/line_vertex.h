/*PGR-GNU*****************************************************************
File: line_vertex.h

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_LINE_VERTEX_H_
#define INCLUDE_CPP_COMMON_LINE_VERTEX_H_
#pragma once

#include <vector>
#include <ostream>

#include "c_types/pgr_edge_t.h"



namespace pgrouting {

class Line_vertex {
 public:
     Line_vertex() :
         id(0) {}

     explicit Line_vertex(const pgr_edge_t& edge) :
         id(edge.id),
         vertex_id(edge.id),
         source(edge.source),
         target(edge.target),
         cost(edge.cost) {}

     Line_vertex(const Line_vertex &v) :
         id(v.id),
         vertex_id(v.vertex_id),
         source(v.source),
         target(v.target),
         cost(v.cost) {}

     void cp_members(const Line_vertex &other) {
         this->id = other.id;
         this->vertex_id = other.vertex_id;
         this->cost = other.cost;
         this->source = other.source;
         this->target = other.target;
     }

     void cp_members(int64_t _id, int64_t _source) {
         this->id = _id;
         this->vertex_id = -1;
         this->cost = 0;
         this->source = _source;
         this->target = -1;
     }

     friend std::ostream& operator<<(std::ostream& log, const Line_vertex &v) {
         log << "\nid = " << v.id;
         log << " | vertex_id = " << v.vertex_id;
         log << " | source = " << v.source;
         log << " | target = " << v.target;
         log << " | cost = " << v.cost;
         return log;
     }

 public:
     int64_t id;
     int64_t vertex_id;
     int64_t source;
     int64_t target;
     double cost;
};

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_LINE_VERTEX_H_
