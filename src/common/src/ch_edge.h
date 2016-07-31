/*PGR-GNU*****************************************************************
File: ch_edge.h

Generated with Template by:
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

#ifndef SRC_COMMON_SRC_CH_EDGE_H_
#define SRC_COMMON_SRC_CH_EDGE_H_
#pragma once

#include <iostream>
#include <sstream>
#include "./ch_vertex.h"

namespace pgrouting {
namespace contraction {

class Edge {
 public:
     Edge() = default;
     Edge(int64_t eid, int64_t source, int64_t target, double cost) :
         id(eid), source(source),
         target(target), cost(cost), first(true) {}
     Edge(int64_t eid, int64_t source, int64_t target,
             double cost, bool first) :
         id(eid), source(source),
         target(target), cost(cost), first(first) {}

     void cp_members(const Edge &other, std::ostringstream& log);
     void add_contracted_vertex(Vertex& v, int64_t vid);
     void add_contracted_edge_vertices(Edge& e);
     bool has_contracted_vertices() const;
     void clear_contracted_vertices() { m_contracted_vertices.clear(); }
     const Identifiers<int64_t>& contracted_vertices() const;
     friend std::ostream& operator <<(std::ostream& os, const Edge& e);

 public:
     int64_t id;
     int64_t source;
     int64_t target;
     double cost;
     bool first;
 private:
     Identifiers<int64_t> m_contracted_vertices;
};

}  // namespace contraction
}  // namespace pgrouting

#endif  // SRC_COMMON_SRC_CH_EDGE_H_
