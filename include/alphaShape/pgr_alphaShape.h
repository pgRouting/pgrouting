/*PGR-GNU*****************************************************************
file: pgr_alphaShape.h

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef INCLUDE_ALPHASHAPE_PGR_ALPHASHAPE_H_
#define INCLUDE_ALPHASHAPE_PGR_ALPHASHAPE_H_
#pragma once

#ifndef __cplusplus
#error This is C++ header file
#endif

#include <vector>
#include <map>
#include <set>
#include <iosfwd>

#include "c_types/pgr_edge_xy_t.h"
#include "cpp_common/pgr_messages.h"

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/bline.hpp"


namespace pgrouting {
namespace alphashape {

using BG = boost::adjacency_list<
        boost::setS, boost::vecS,
        boost::undirectedS,
        XY_vertex, Basic_edge >;
using G = graph::Pgr_base_graph <BG, XY_vertex, Basic_edge>;
using E = boost::graph_traits<BG>::edge_descriptor;
using V = boost::graph_traits<BG>::vertex_descriptor;

using Triangle = std::set<E>;


class Pgr_alphaShape : public Pgr_messages {
 public:
     Pgr_alphaShape() = default;
     explicit Pgr_alphaShape(const std::vector<Pgr_edge_xy_t> &edges);

     std::vector<Bpoly> operator() (double alpha) const;

     friend std::ostream& operator<<(std::ostream&, const Pgr_alphaShape&);

 private:
     void make_triangles();
     double radius(const Triangle t) const;
     bool faceBelongs(
             const Triangle face,
             double alpha) const;
     void recursive_build(
             const Triangle face,
             std::set<Triangle> &used,
             std::set<E> &border_edges,
             double alpha) const;
     std::vector<Bpoly> build_best_alpha() const;

     struct EdgesFilter {
         std::set<E> edges;
         bool operator()(E e) const { return edges.count(e); }
         void clear() { edges.clear(); }
     };

 private:
     /*! sides graph */
     G graph;
     /*! t -> {adj_t} */
     std::map<Triangle, std::set<Triangle>> m_adjacent_triangles;
};


}  // namespace alphashape
}  // namespace pgrouting

#endif  // INCLUDE_ALPHASHAPE_PGR_ALPHASHAPE_H_
