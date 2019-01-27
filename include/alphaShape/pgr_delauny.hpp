/*PGR-GNU*****************************************************************
file: pgr_delauny.hpp

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

#ifndef INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
#define INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
#pragma once

#include <vector>
#if 0
#include <map>
#endif
#include <iosfwd>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.h"
#include "cpp_common/bline.hpp"

// this one is included before somehow
#include "c_types/pgr_edge_xy_t.h"
#if 0
#include "c_types/delauny_t.h"
#include "cpp_common/bpoint.hpp"
#include "alphaShape/pgr_triangle.hpp"
#endif

namespace pgrouting {
namespace alphashape {

using BG = boost::adjacency_list<
        boost::setS, boost::vecS,
        boost::undirectedS,
        XY_vertex, Basic_edge >;
using G = graph::Pgr_base_graph <BG, XY_vertex, Basic_edge>;


class Pgr_delauny : public Pgr_messages {
#if 0
    using Bline = boost::geometry::model::linestring<Bpoint>;
    using Blines = boost::geometry::model::multi_linestring<Bline>;
#endif

 public:
     Pgr_delauny() = default;
#if 0
     Pgr_delauny(const std::vector<Delauny_t> &triangles);
#endif
     Pgr_delauny(const std::vector<Pgr_edge_xy_t> &edges);

#if 0
     void clear();
#endif

     friend std::ostream& operator<<(std::ostream&, const Pgr_delauny&);

 private:

     std::vector<Bline> alpha_edges(double alpha) const;
#if 0
     void alpha_edges_from_delauny(double alpha) const;
     void save_points_from_delauny_info();
     void save_points_from_graph_info();
#endif

     G graph;

#if 0
     Bpoints   m_points;
     Blines m_lines;
     std::vector<Delauny_t> m_delauny;
     std::vector<Pgr_triangle> m_triangles;
     std::map<size_t, std::vector<size_t> > m_relation;
#endif
};


}  // namespace alphashape
}  // namespace pgrouting

#endif  // INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
