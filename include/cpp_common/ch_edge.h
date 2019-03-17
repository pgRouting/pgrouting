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

#ifndef INCLUDE_CPP_COMMON_CH_EDGE_H_
#define INCLUDE_CPP_COMMON_CH_EDGE_H_
#pragma once

#include <iostream>
#include <sstream>
#include "./ch_vertex.h"
// #include "./basic_edge.h"

namespace pgrouting {

class CH_edge {
 public:
     CH_edge() = default;

     CH_edge(int64_t eid, int64_t source, int64_t target, double cost) :
         id(eid), source(source),
         target(target), cost(cost) {}

     void cp_members(const CH_edge &other);

     void add_contracted_vertex(CH_vertex& v);
     void add_contracted_edge_vertices(CH_edge& e);

     bool has_contracted_vertices() const;

     void clear_contracted_vertices() {m_contracted_vertices.clear();}
     const Identifiers<int64_t>& contracted_vertices() const;
     Identifiers<int64_t>& contracted_vertices();
     friend std::ostream& operator <<(std::ostream& os, const CH_edge& e);

 public:
     int64_t id;
     int64_t source;
     int64_t target;
     double cost;

 private:
     Identifiers<int64_t> m_contracted_vertices;
};

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_CH_EDGE_H_
